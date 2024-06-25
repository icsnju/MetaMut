#include "afl-gcc-common.h"
#if (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__) >= \
    60200 /* >= version 6.2.0 */
#  include "memmodel.h"
#endif

#include <cxxabi.h>
#include <gcc-plugin.h>
#include <gimple.h>
#include <tree.h>

#include <fcntl.h>
#include <set>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <tuple>
#include <map>
#include <sys/ipc.h>
#include <sys/shm.h>

/* This plugin, being under the same license as GCC, satisfies the
   "GPL-compatible Software" definition in the GCC RUNTIME LIBRARY
   EXCEPTION, so it can be part of an "Eligible" "Compilation
   Process".  */
int plugin_is_GPL_compatible = 1;

namespace {

class CounterDispatcher {
  FILE *bbmap_fp = nullptr;

public:
  void init() {
    /* bbmap dir */
    char *mapdir = getenv("__AFL_COUNTER_MAP_DIR");
    if (!mapdir) return;

    char bbfile_template[strlen(mapdir) + 20];
    sprintf(bbfile_template, "%s/bb.XXXXXX", mapdir);
    int bbfd = mkstemp(bbfile_template);
    bbmap_fp = fdopen(bbfd, "w+");
  }

  ~CounterDispatcher() {
    if (bbmap_fp) fclose(bbmap_fp);
  }

  static int shm_get_bbidx() {
    return shm_get_idx("__AFL_COUNTER_SHMID", 0);
  }

  static int shm_get_fnidx() {
    return shm_get_idx("__AFL_COUNTER_SHMID", 1);
  }

  static int shm_get_idx(const char *env, unsigned index) {
    char *shmid_str = getenv(env);
    if (!shmid_str) return rand();

    int shmid = atoi(shmid_str);
    int *addr = (int *)shmat(shmid, NULL, 0);
    int value = 0;

    asm volatile(
        "lock xaddl %0, %1\n\t"
        : "=r" (value)
        : "m" (addr[index]), "0" (1)
        : "memory"
    );

    shmdt(addr);
    return value;
  }

  static std::string get_qualified_funcname(function *fn) {
    int status = 0;
    tree id = DECL_ASSEMBLER_NAME(fn->decl);
    const char *name = IDENTIFIER_POINTER(id);
    char *demangled_name = abi::__cxa_demangle(name, 0, 0, &status);
    std::string retname = "(null)";
    if (demangled_name) {
      retname = demangled_name;
      free(demangled_name);
    } else if (name)
      retname = name;
    return retname;
  }

  auto get_locs(function *fn, basic_block bb) {
    using tuple_ty = std::tuple<std::string, std::string, unsigned>;
    std::set<tuple_ty> ret;
    gimple_stmt_iterator gsi;
    for (gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi)) {
      gimple stmt = gsi_stmt(gsi);

      location_t loc = gimple_location(stmt);

      const char *filename = LOCATION_FILE(loc);
      std::string function_name = get_qualified_funcname(fn);
      int lineno = LOCATION_LINE(loc);

      ret.insert(tuple_ty{
          filename ?: "(null)", function_name, lineno});
    }
    return ret;
  }

  int get_bb_idx(function *fn, basic_block bb) {
    if (!bbmap_fp) return rand();

    int idx = shm_get_bbidx();
    auto locs = get_locs(fn, bb);

    std::ostringstream oss;
    oss << "[";
    bool is_first = true;
    for (auto it = locs.begin(); it != locs.end(); ++it) {
      if (!is_first) oss << ", ";
      is_first = false;
      oss << "["
          << "\"" << std::get<0>(*it) << "\", ";
      oss << "\"" << std::get<1>(*it) << "\", ";
      oss << std::get<2>(*it) << "]";
    }
    oss << "]";

    std::string info = oss.str();
    fprintf(bbmap_fp, "%d: %s\n", idx, info.c_str());
    return idx;
  }

} counter_dispatcher;

static constexpr struct pass_data afl_pass_data = {
    .type = GIMPLE_PASS,
    .name = "afl",
    .optinfo_flags = OPTGROUP_NONE,
    .tv_id = TV_NONE,
    .properties_required = 0,
    .properties_provided = 0,
    .properties_destroyed = 0,
    .todo_flags_start = 0,
    .todo_flags_finish = (TODO_update_ssa | TODO_cleanup_cfg | TODO_verify_il),

};

struct afl_pass : afl_base_pass {
  afl_pass(bool quiet)
      : afl_base_pass(quiet, !!getenv("AFL_DEBUG"), afl_pass_data),
        inst_blocks(0) {
    initInstrumentList();
  }

  unsigned int inst_blocks;

  static unsigned count_basic_blocks(function *fn) {
    basic_block bb;
    unsigned nblocks = 0;
    FOR_EACH_BB_FN(bb, fn) { nblocks ++; }
    return nblocks;
  }

  static unsigned count_instructions(function *fn, basic_block bb) {
    unsigned ninsts = 0;
    gimple_stmt_iterator gsi;
    for (gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi))
      ninsts ++;
    return ninsts;
  }

  void instrument_call(function *fn, basic_block bb) {
    /* Generate the block identifier.  */
    gimple_seq seq = NULL;

    // insert call to __afl_trace_bb
    uint32_t bbidx = counter_dispatcher.get_bb_idx(fn, bb);
    static tree bb_tracer = get_afl_tracer_decl("__afl_trace_bb");
    gcall *bb_tracer_call =
        gimple_build_call(bb_tracer, 1, build_int_cst(sizetype, bbidx));
    gimple_seq_add_stmt(&seq, bb_tracer_call);

    /* Insert the generated sequence.  */
    gimple_stmt_iterator insp = gsi_after_labels(bb);
    gsi_insert_seq_before(&insp, seq, GSI_SAME_STMT);
  }

  void instrument_instructions(function *fn, basic_block bb) {
    gimple_seq seq = NULL;
    static tree afl_bbmap_ptr = get_afl_map_ptr_decl("__afl_bbmap_ptr");

    /**********
     * set bb *
     **********/
    {
      tree uint8_type_node = unsigned_char_type_node;
      uint32_t bbidx = counter_dispatcher.get_bb_idx(fn, bb);

      // 1. Create GIMPLE variables
      tree tv_offset = build_int_cst(sizetype, bbidx / 8);
      tree tv_mask8 =
          build_int_cst(unsigned_char_type_node, 1 << (bbidx & 7));

      tree tv_rref, tv_wref;
      tree tv_map_ptr =
          create_tmp_var(TREE_TYPE(afl_bbmap_ptr), ".tv_map_ptr");
      tree tv_addr = create_tmp_var(TREE_TYPE(afl_bbmap_ptr), ".tv_addr");
      tree tv_load8 = create_tmp_var(unsigned_char_type_node, ".tv_load8");
      tree tv_orv = create_tmp_var(unsigned_char_type_node, ".tv_orv");

      gimple stmt;

      // tv_map_ptr = afl_bbmap_ptr
      stmt = gimple_build_assign(tv_map_ptr, afl_bbmap_ptr);
      gimple_seq_add_stmt(&seq, stmt);

      // tv_addr = tv_map_ptr + id / 8
      stmt = gimple_build_assign(
          tv_addr, POINTER_PLUS_EXPR, tv_map_ptr, tv_offset);
      gimple_seq_add_stmt(&seq, stmt);

      // tv_load8 = *(tv_map_ptr + addr)
      tv_rref = build2(MEM_REF, TREE_TYPE(TREE_TYPE(tv_addr)), tv_addr,
          build_zero_cst(TREE_TYPE(tv_addr)));
      stmt = gimple_build_assign(tv_load8, tv_rref);
      gimple_seq_add_stmt(&seq, stmt);

      // tv_orv = tv_load8 | tv_mask
      stmt = gimple_build_assign(tv_orv, BIT_IOR_EXPR, tv_load8, tv_mask8);
      gimple_seq_add_stmt(&seq, stmt);

      // tv_wef = tv_orv
      tv_wref = build2(MEM_REF, TREE_TYPE(TREE_TYPE(tv_addr)), tv_addr,
          build_zero_cst(TREE_TYPE(tv_addr)));
      stmt = gimple_build_assign(tv_wref, tv_orv);
      gimple_seq_add_stmt(&seq, stmt);
    }

    /* Insert the generated sequence.  */
    gimple_stmt_iterator insp = gsi_after_labels(bb);
    gsi_insert_seq_before(&insp, seq, GSI_SAME_STMT);
  }

  virtual unsigned int execute(function *fn) {
    if (!isInInstrumentList(fn)) return 0;
    const char *srcfile = DECL_SOURCE_FILE(fn->decl) ? : "";
    if (strncmp(srcfile, "/usr", 4) == 0 || strncmp(srcfile, "/etc", 4) == 0)
      return 0;

    basic_block bb;
    int blocks = 0;
    FOR_EACH_BB_FN(bb, fn) {
      if (!instrument_block_p(bb)) continue;
      instrument_call(fn, bb);
      // instrument_instructions(fn, bb);
      blocks++;
    }

    /* Aggregate the instrumented block count.  */
    inst_blocks += blocks;
    return 0;
  }

  /* Decide whether to instrument block BB.  Skip it due to the random
     distribution, or if it's the single successor of all its
     predecessors.  */
  inline bool instrument_block_p(basic_block bb) {
    edge e;
    edge_iterator ei;
    FOR_EACH_EDGE(e, ei, bb->preds)
    if (!single_succ_p(e->src)) return true;
    return false;
  }

  static inline tree get_afl_tracer_decl(const char *tracer_name) {
    tree type =
        build_function_type_list(void_type_node, uint16_type_node, NULL_TREE);
    tree decl = build_fn_decl(tracer_name, type);
    TREE_PUBLIC(decl) = 1;
    DECL_EXTERNAL(decl) = 1;
    DECL_ARTIFICIAL(decl) = 1;
    return decl;
  }

  /* Create and return a declaration for the __afl_prev_loc
     thread-local variable.  */
  static inline tree get_afl_map_ptr_decl(const char *mapname) {
    /* If type changes, the size N in FETCH_ADD_<N> must be adjusted
       in builtin calls above.  */
    tree type = build_pointer_type(unsigned_char_type_node);
    tree decl = build_decl(
        BUILTINS_LOCATION, VAR_DECL, get_identifier(mapname), type);
    TREE_PUBLIC(decl) = 1;
    DECL_EXTERNAL(decl) = 1;
    DECL_ARTIFICIAL(decl) = 1;
    TREE_STATIC(decl) = 1;
    return decl;
  }

  /* This is registered as a plugin finalize callback, to print an
     instrumentation summary unless in quiet mode.  */
  static void plugin_finalize(void *, void *p) {
    opt_pass *op = (opt_pass *)p;
    afl_pass &self = (afl_pass &)*op;
    if (!self.be_quiet) {
      if (!self.inst_blocks)
        WARNF("No instrumentation targets found.");
      else
        OKF("Instrumented %u locations.", self.inst_blocks);
    }
  }
};

static struct plugin_info afl_plugin = {
    .version = "20220420",
    .help = G_("AFL gcc plugin\n\
\n\
Set AFL_QUIET in the environment to silence it.\n\
\n\
Set AFL_INST_RATIO in the environment to a number from 0 to 100\n\
to control how likely a block will be chosen for instrumentation.\n\
\n\
Specify -frandom-seed for reproducible instrumentation.\n\
"),

};

} // namespace

/* This is the function GCC calls when loading a plugin.  Initialize
   and register further callbacks.  */
int plugin_init(
    struct plugin_name_args *info, struct plugin_gcc_version *version) {

#if 0
  if (!plugin_default_version_check(version, &gcc_version))
    FATAL(G_("GCC and plugin have incompatible versions, expected GCC %s, "
             "is %s"),
          gcc_version.basever, version->basever);
#endif
  counter_dispatcher.init();

  /* Show a banner.  */
  bool quiet = false;
  if (isatty(2) && !getenv("AFL_QUIET"))
    SAYF(cCYA "afl-gcc-pass " cBRI VERSION cRST " by <oliva@adacore.com>\n");
  else
    quiet = true;

  /* Initialize the random number generator with GCC's random seed, in
     case it was specified in the command line's -frandom-seed for
     reproducible instrumentation.  */
  srandom(get_random_seed(false));

  const char *name = info->base_name;
  register_callback(name, PLUGIN_INFO, NULL, &afl_plugin);

  afl_pass *aflp = new afl_pass(quiet);
  struct register_pass_info pass_info = {

      .pass = aflp,
      .reference_pass_name = "ssa",
      .ref_pass_instance_number = 1,
      .pos_op = PASS_POS_INSERT_AFTER,

  };

  register_callback(name, PLUGIN_PASS_MANAGER_SETUP, NULL, &pass_info);
  register_callback(
      name, PLUGIN_FINISH, afl_pass::plugin_finalize, pass_info.pass);

  return 0;
}
