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

extern void unknown_fn (void *);

static int only_used_by_test_1;

static void test_1 (void)
{
  int local_1, local_2;
  __analyzer_dump_escaped (); /* { dg-warning "escaped: 0: " } */

  unknown_fn (NULL);
  __analyzer_dump_escaped (); /* { dg-warning "escaped: 0: " } */

  unknown_fn (&local_1);
  __analyzer_dump_escaped (); /* { dg-warning "escaped: 1: 'local_1'" "" { target c } } */
  /* { dg-warning "escaped: 1: 'int local_1'" "" { target c++ } .-1 } */

  /* Should be idempotent.  */
  unknown_fn (&local_1);
  __analyzer_dump_escaped (); /* { dg-warning "escaped: 1: 'local_1'" "" { target c } } */
  /* { dg-warning "escaped: 1: 'int local_1'" "" { target c++ } .-1 } */

  /* Escape a static global.  */
  unknown_fn (&only_used_by_test_1);
  __analyzer_dump_escaped (); /* { dg-warning "escaped: 2: 'local_1', 'only_used_by_test_1'" "" { target c } } */
  /* { dg-warning "escaped: 2: 'int local_1', 'int only_used_by_test_1'" "" { target c++ } .-1 } */
}
