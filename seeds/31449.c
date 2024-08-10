#include <stdlib.h>
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

extern void might_realloc (void *);
extern void cant_realloc (const void *);

void *
test_realloc_1 (void *p, size_t new_sz)
{
  void *q = realloc (p, new_sz);
  __analyzer_dump_capacity (q); /* { dg-warning "capacity: 'UNKNOWN\\(sizetype\\)'" "failure" } */
  /* { dg-warning "capacity: 'INIT_VAL\\(new_sz\[^\n\r\]*\\)'" "success" { target *-*-* } .-1 } */
  return q;
}

void *
test_realloc_2 (size_t sz_a, size_t sz_b)
{
  void *p = malloc (sz_a);
  __analyzer_dump_capacity (p); /* { dg-warning "capacity: 'INIT_VAL\\(sz_a_\[^\n\r\]*\\)'" } */
  void *q = realloc (p, sz_b);
  __analyzer_dump_capacity (q); /* { dg-warning "capacity: 'UNKNOWN\\(sizetype\\)'" "failure" } */
  /* { dg-warning "capacity: 'INIT_VAL\\(sz_b\[^\n\r\]*\\)'" "success" { target *-*-* } .-1 } */
  return q; /* { dg-warning "leak of 'p'" } */
}

void *
test_might_realloc (void)
{
  void *p = malloc (1024);

  __analyzer_dump_capacity (p); /* { dg-warning "capacity: '\\(size_t\\)1024'" } */

  might_realloc (p);

  __analyzer_dump_capacity (p); /* { dg-warning "capacity: 'UNKNOWN\\(sizetype\\)'" } */

  return p;
}

void *
test_cant_realloc (void)
{
  void *p = malloc (1024);

  __analyzer_dump_capacity (p); /* { dg-warning "capacity: '\\(size_t\\)1024'" } */

  cant_realloc (p);

  __analyzer_dump_capacity (p); /* { dg-warning "capacity: '\\(size_t\\)1024'" } */

  return p;
}


