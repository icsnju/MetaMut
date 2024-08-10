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


extern int *const p1;

int *const p2 = NULL;

int v3;
extern int *const p3 = &v3; /* { dg-warning "'p3' initialized and declared 'extern'" "C FE warning" { target c } } */

int v4;
int *const p4 = &v4;

int main (void)
{
  __analyzer_describe (0, p1); /* { dg-message "INIT_VAL\\(p1\\)" "" { target c } } */
   /* { dg-message "INIT_VAL\\(int\\* const p1\\)" "" { target c++ } .-1 } */
  __analyzer_eval (p1 == NULL); /* { dg-message "UNKNOWN" } */

  __analyzer_eval (p2 == NULL); /* { dg-message "TRUE" } */

  __analyzer_describe (0, p3); /* { dg-message "&v3" "" { target c } } */
   /* { dg-message "&int v3" "" { target c++ } .-1 } */
  __analyzer_eval (p3 == NULL); /* { dg-message "FALSE" } */

  __analyzer_describe (0, p4); /* { dg-message "&v4" "" { target c } } */
  /* { dg-message "&int v4" "" { target c++ } .-1 } */
  __analyzer_eval (p4 == NULL); /* { dg-message "FALSE" } */

  return p1[0];
}
