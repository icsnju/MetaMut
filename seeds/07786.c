/* { dg-additional-options "-fanalyzer-checker=taint" } */
// TODO: remove need for this option

#ifndef ANALYZER_DECLS_H
#define ANALYZER_DECLS_H

#ifndef NULL
#ifdef __cplusplus
#if __cplusplus >= 201103L
#define NULL nullptr
#else
#define NULL 0
#endif
#else
#define NULL ((void *)0)
#endif
#endif

/* Function decls with special meaning to the analyzer.
   None of these are actually implemented.  */

/* Trigger a breakpoint in the analyzer when reached.  */
extern void __analyzer_break (void);

/* Emit a warning describing the 2nd argument (which can be of any
   type), at the given verbosity level.  This is for use when
   debugging, and may be of use in DejaGnu tests.  */
extern void __analyzer_describe (int verbosity, ...);

/* Dump copious information about the analyzer’s state when reached.  */
extern void __analyzer_dump (void);

/* Emit a warning describing the size of the base region of (*ptr).  */
extern void __analyzer_dump_capacity (const void *ptr);

/* Dump information about what decls have escaped at this point on the path.  */
extern void __analyzer_dump_escaped (void);

/* Dump information after analysis on all of the exploded nodes at this
   program point.

   __analyzer_dump_exploded_nodes (0);
   will dump just the number of nodes, and their IDs.

   __analyzer_dump_exploded_nodes (1);
   will also dump all of the states within those nodes.  */
extern void __analyzer_dump_exploded_nodes (int);

/* Emit a warning describing what is known about the value of NAME.  */
extern void __analyzer_dump_named_constant (const char *name);

/* Emit a placeholder "note" diagnostic with a path to this call site,
   if the analyzer finds a feasible path to it.  */
extern void __analyzer_dump_path (void);

/* Dump the region_model's state to stderr.  */
extern void __analyzer_dump_region_model (void);

/* Emit a warning describing the state of the 2nd argument
   (which can be of any type) with respect to NAME.
   This is for use when debugging, and may be of use in DejaGnu tests.  */
extern void __analyzer_dump_state (const char *name, ...);

/* Emit a warning with text "TRUE", FALSE" or "UNKNOWN" based on the
   truthfulness of the argument.  */
extern void __analyzer_eval (int);

/* Obtain an "unknown" void *.  */
extern void *__analyzer_get_unknown_ptr (void);

/* Complain if PTR doesn't point to a null-terminated string.
   TODO: eventually get the strlen of the buffer (without the
   optimizer touching it).  */
extern __SIZE_TYPE__ __analyzer_get_strlen (const char *ptr);

#endif /* #ifndef ANALYZER_DECLS_H.  */

int v_start;

__attribute__((tainted_args))
void test (int v_tainted, int v_has_lb, int v_has_ub, int v_stop)
{
  /* Get each var into the 5 different taintedness states.  */
  if (v_has_lb < 10)
    return;
  if (v_has_ub > 100)
    return;
  if (v_stop < 0 || v_stop > 100)
    return;

  /* Verify that we have the taintedness states we expect.  */

  __analyzer_dump_state ("taint", v_start); /* { dg-warning "state: 'start'" } */
  __analyzer_dump_state ("taint", v_tainted); /* { dg-warning "state: 'tainted'" } */
  __analyzer_dump_state ("taint", v_has_lb); /* { dg-warning "state: 'has_lb'" } */
  __analyzer_dump_state ("taint", v_has_ub); /* { dg-warning "state: 'has_ub'" } */
  __analyzer_dump_state ("taint", v_stop); /* { dg-warning "state: 'stop'" } */

  /* Check all combinations of taintedness state.  */
  __analyzer_dump_state ("taint", v_start + v_start); /* { dg-warning "state: 'start'" } */
  __analyzer_dump_state ("taint", v_start + v_tainted); /* { dg-warning "state: 'tainted'" } */
  __analyzer_dump_state ("taint", v_start + v_has_lb); /* { dg-warning "state: 'has_lb'" } */
  __analyzer_dump_state ("taint", v_start + v_has_ub); /* { dg-warning "state: 'has_ub'" } */
  __analyzer_dump_state ("taint", v_start + v_stop); /* { dg-warning "state: 'stop'" } */

  __analyzer_dump_state ("taint", v_tainted + v_start); /* { dg-warning "state: 'tainted'" } */
  __analyzer_dump_state ("taint", v_tainted + v_tainted); /* { dg-warning "state: 'tainted'" } */
  __analyzer_dump_state ("taint", v_tainted + v_has_lb); /* { dg-warning "state: 'tainted'" } */
  __analyzer_dump_state ("taint", v_tainted + v_has_ub); /* { dg-warning "state: 'tainted'" } */
  __analyzer_dump_state ("taint", v_tainted + v_stop); /* { dg-warning "state: 'tainted'" } */

  __analyzer_dump_state ("taint", v_has_lb + v_start); /* { dg-warning "state: 'has_lb'" } */
  __analyzer_dump_state ("taint", v_has_lb + v_tainted); /* { dg-warning "state: 'tainted'" } */
  __analyzer_dump_state ("taint", v_has_lb + v_has_lb); /* { dg-warning "state: 'has_lb'" } */
  __analyzer_dump_state ("taint", v_has_lb + v_has_ub); /* { dg-warning "state: 'tainted'" } */
  __analyzer_dump_state ("taint", v_has_lb + v_stop); /* { dg-warning "state: 'has_lb'" } */

  __analyzer_dump_state ("taint", v_has_ub + v_start); /* { dg-warning "state: 'has_ub'" } */
  __analyzer_dump_state ("taint", v_has_ub + v_tainted); /* { dg-warning "state: 'tainted'" } */
  __analyzer_dump_state ("taint", v_has_ub + v_has_lb); /* { dg-warning "state: 'tainted'" } */
  __analyzer_dump_state ("taint", v_has_ub + v_has_ub); /* { dg-warning "state: 'has_ub'" } */
  __analyzer_dump_state ("taint", v_has_ub + v_stop); /* { dg-warning "state: 'has_ub'" } */

  __analyzer_dump_state ("taint", v_stop + v_start); /* { dg-warning "state: 'stop'" } */
  __analyzer_dump_state ("taint", v_stop + v_tainted); /* { dg-warning "state: 'tainted'" } */
  __analyzer_dump_state ("taint", v_stop + v_has_lb); /* { dg-warning "state: 'has_lb'" } */
  __analyzer_dump_state ("taint", v_stop + v_has_ub); /* { dg-warning "state: 'has_ub'" } */
  __analyzer_dump_state ("taint", v_stop + v_stop); /* { dg-warning "state: 'stop'" } */
}
