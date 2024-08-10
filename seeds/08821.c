/* Adapted from
   https://github.com/stackpath/rxtxcpu/blob/816d86c5d49c4db2ea5649f6b87e96da5af660f1/cpu.c
   which is MIT-licensed.  */

typedef __SIZE_TYPE__ size_t;
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

extern size_t strlen (const char *__s)
  __attribute__ ((__nothrow__ , __leaf__))
  __attribute__ ((__pure__))
  __attribute__ ((__nonnull__ (1)));
extern char *strdup (const char *__s)
  __attribute__ ((__nothrow__ , __leaf__))
  __attribute__ ((__malloc__))
  __attribute__ ((__nonnull__ (1)));
extern char *strsep (char **__restrict __stringp,
		     const char *__restrict __delim)
  __attribute__ ((__nothrow__ , __leaf__))
  __attribute__ ((__nonnull__ (1, 2)));
extern long int strtol (const char *__restrict __nptr,
			char **__restrict __endptr, int __base)
  __attribute__ ((__nothrow__ , __leaf__))
  __attribute__ ((__nonnull__ (1)));
extern void free (void *__ptr)
  __attribute__ ((__nothrow__ , __leaf__));

#define CPU_LIST_BASE 10

int parse_cpu_list(char *cpu_list) {
  if (strlen(cpu_list) == 0) {
    return 0;
  }

  char *endptr;
  char *tofree, *string, *range;

  tofree = string = strdup(cpu_list); /* { dg-message "allocated here" } */

  while ((range = strsep(&string, ",")) != NULL) {
    int first = strtol(range, &endptr, CPU_LIST_BASE);
    if (!*endptr) {
      continue;
    }
    char *save = endptr;
    endptr++;
    int last = strtol(endptr, &endptr, CPU_LIST_BASE);
    if (save[0] != '-' || *endptr || last < first) {
      return -1; /* { dg-warning "leak of 'tofree'" } */
    }
  }
  free(tofree);
  return 0;
}
