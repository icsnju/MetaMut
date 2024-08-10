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

#include <stdlib.h>

extern void foo(void *ptrA, void *ptrB, void *ptrC) /* { dg-message "argument 1 of 'foo' must be non-null" } */
  __attribute__((nonnull (1, 3)));

extern void bar(void *ptrA, void *ptrB, void *ptrC) /* { dg-message "argument 1 of 'bar' must be non-null" } */
  __attribute__((nonnull));

// TODO: complain about NULL and possible NULL args
// FIXME: ought to complain about NULL args

void test_1 (void *p, void *q, void *r)
{
  foo(p, q, r);
  foo(NULL, q, r); /* { dg-warning "use of NULL where non-null expected" "warning" } */
  /* { dg-message "argument 1 NULL where non-null expected" "note" { target *-*-* } .-1 } */
}

void test_1a (void *q, void *r)
{
  void *p = NULL;
  foo(p, q, r); /* { dg-warning "use of NULL 'p' where non-null expected" "warning" } */
  /* { dg-message "argument 1 \\('p'\\) NULL where non-null expected" "note" { target *-*-* } .-1 } */
}

void test_1b (void *p, void *r)
{
  foo(p, NULL, r);
}

void test_1c (void *p, void *q, void *r)
{
  foo(p, q, NULL); /* { dg-warning "use of NULL where non-null expected" } */
}

void test_2a (void *p, void *q, void *r)
{
  bar(p, q, r);
}

void test_2b (void *p, void *q, void *r)
{
  bar(p, NULL, r); /* { dg-warning "use of NULL where non-null expected" "warning" } */
  /* { dg-message "argument 2 NULL where non-null expected" "note" { target *-*-* } .-1 } */
}

void test_2c (void *p, void *q, void *r)
{
  bar(p, q, NULL); /* { dg-warning "use of NULL where non-null expected" "warning" } */
}

void test_3 (void *q, void *r)
{
  void *p = malloc(1024); /* { dg-message "\\(1\\) this call could return NULL" } */

  foo(p, q, r); /* { dg-warning "use of possibly-NULL 'p' where non-null expected" "warning" } */
  /* { dg-message "argument 1 \\('p'\\) from \\(1\\) could be NULL where non-null expected" "note" { target *-*-* } .-1 } */

  foo(p, q, r);

  free(p);
}

void test_4 (void *q, void *r)
{
  void *p = malloc(1024); /* { dg-message "\\(1\\) this call could return NULL" } */

  bar(p, q, r); /* { dg-warning "use of possibly-NULL 'p' where non-null expected" "warning" } */
  /* { dg-message "argument 1 \\('p'\\) from \\(1\\) could be NULL where non-null expected" "note" { target *-*-* } .-1 } */

  bar(p, q, r);

  free(p);
}

/* Verify that we detect passing NULL to a __attribute__((nonnull)) function
   when it's called via a function pointer.  */

typedef void (*bar_t)(void *ptrA, void *ptrB, void *ptrC);

static bar_t __attribute__((noinline))
get_bar (void)
{
  return bar;
}

void test_5 (void *q, void *r)
{
  void *p = malloc(1024); /* { dg-message "\\(1\\) this call could return NULL" } */
  bar_t cb = get_bar ();
  cb(p, q, r); /* { dg-warning "use of possibly-NULL 'p' where non-null expected" "warning" } */
  /* { dg-message "argument 1 \\('p'\\) from \\(1\\) could be NULL where non-null expected" "note" { target *-*-* } .-1 } */
  /* TODO: do we want an event showing where cb is assigned "bar"?  */

  cb(p, q, r);

  free(p);
}

__attribute__((nonnull(1, 3)))
void test_6 (void *p, void *q, void *r)
{
  __analyzer_eval (p != NULL); /* { dg-warning "TRUE" } */
  __analyzer_eval (q != NULL); /* { dg-warning "UNKNOWN" } */
  __analyzer_eval (r != NULL); /* { dg-warning "TRUE" } */
}

__attribute__((nonnull))
void test_7 (void *p, void *q, void *r)
{
  __analyzer_eval (p != NULL); /* { dg-warning "TRUE" } */
  __analyzer_eval (q != NULL); /* { dg-warning "TRUE" } */
  __analyzer_eval (r != NULL); /* { dg-warning "TRUE" } */
}