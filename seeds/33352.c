/* { dg-additional-options "-fanalyzer-call-summaries --param analyzer-min-snodes-for-call-summary=0" } */

/* There need to be at least two calls to a function for the
   call-summarization code to be used.
   TODO: add some kind of test that summarization *was* used.  */

#include <stdlib.h>
#include <string.h>
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

int *malloc_int (int i)
{
  int *res = (int *) malloc (sizeof (int));
  if (!res)
    return NULL;
  *res = i;
  return res;
}

void test_malloc_int (int x)
{
  int *p, *q;

  p = malloc_int (42);
  if (p)
    __analyzer_eval (*p == 42); /* { dg-warning "TRUE" } */
  free (p);

  q = malloc_int (x);
  if (q)
    __analyzer_eval (*q == x); /* { dg-warning "TRUE" } */
  free (q);
}

void test_leak (int x)
{
  int *p = malloc_int (x); /* { dg-message "when 'malloc_int' returns pointer to heap-allocated buffer" "" { target c } } */
  /* { dg-message "when 'int\\* malloc_int\\(int\\)' returns pointer to heap-allocated buffer" "" { target c++ } .-1 } */
} /* { dg-message "leak of 'p'" } */

void *wrapped_malloc (size_t sz)
{
  return malloc (sz);
}

void wrapped_free (void *p)
{
  free (p);
}

void test_wrapped_malloc_and_free (size_t sz)
{
  void *p = wrapped_malloc (100);
  void *q = wrapped_malloc (sz);
  __analyzer_dump_capacity (p); /* { dg-warning "capacity: '\\(\[^\n\r\]*\\)100'" } */
  __analyzer_dump_capacity (q); /* { dg-warning "capacity: 'INIT_VAL\\(sz_\[^\n\r\]*\\)'" } */
  wrapped_free (p);
  wrapped_free (q);
}

void test_use_after_free (void)
{
  // TODO
}

void test_use_without_check (size_t sz)
{
  char *buf = (char *) wrapped_malloc (sz); /* { dg-message "this call could return NULL" } */
  memset (buf, 'x', sz); /* { dg-warning "use of possibly-NULL 'buf' where non-null expected" } */
  wrapped_free (buf);
}

void test_out_of_bounds (size_t sz)
{
  char *buf = (char *) wrapped_malloc (sz);
  if (!buf)
    return;
  memset (buf, 'x', sz);
  buf[sz] = '\0'; /* { dg-warning "heap-based buffer overflow" } */
  wrapped_free (buf);
}
