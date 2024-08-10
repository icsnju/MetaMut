/* { dg-additional-options "-fanalyzer-call-summaries" } */

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

extern int foo (int);

static int __attribute__((noinline))
do_stuff (int *p, int n)
{
  int sum = 0;
  int i;
  for (i = 0; i < n; i++)
    p[i] = i;
  for (i = 0; i < n; i++)
    sum += foo (p[i]); /* { dg-bogus "uninitialized" } */
  return sum;
}

static int __attribute__((noinline))
do_stuff_2 (int *p, int n)
{
  return 0;
}

/* Various examples of functions that use either a malloc buffer
   or a local buffer, do something, then conditionally free the
   buffer, tracking whether "free" is necessary in various
   ways.

   In each case, there ought to be only two paths through the function,
   not four.  */

/* Repeated (n > 10) predicate.  */

int test_repeated_predicate_1 (int n)
{
  int buf[10];
  int *ptr;
  int result;

  if (n > 10)
    ptr = (int *)malloc (sizeof (int) * n);
  else
    ptr = buf;

  __analyzer_dump_exploded_nodes (0); /* { dg-warning "2 processed enodes" } */

  result = do_stuff (ptr, n);

  __analyzer_dump_exploded_nodes (0); /* { dg-warning "2 processed enodes" } */

  if (n > 10)
    free (ptr); /* { dg-bogus "not on the heap" } */

  return result; /* { dg-bogus "leak" } */
}

/* A simpler version of the above.  */

int test_repeated_predicate_2 (int n)
{
  int buf[10];
  int *ptr;
  int result;

  if (n > 10)
    ptr = (int *)malloc (sizeof (int) * n);
  else
    ptr = buf;

  __analyzer_dump_exploded_nodes (0); /* { dg-warning "2 processed enodes" } */

  result = do_stuff_2 (ptr, n);

  __analyzer_dump_exploded_nodes (0); /* { dg-warning "2 processed enodes" } */

  if (n > 10)
    free (ptr); /* { dg-bogus "not on the heap" } */

  return result; /* { dg-bogus "leak" } */
}

/* A predicate that sets a flag for the 2nd test.  */

int test_explicit_flag (int n)
{
  int buf[10];
  int *ptr;
  int result;
  int need_to_free = 0;

  if (n > 10)
    {
      ptr = (int *)malloc (sizeof (int) * n);
      need_to_free = 1;
    }
  else
    ptr = buf;

  __analyzer_dump_exploded_nodes (0); /* { dg-warning "2 processed enodes" } */

  result = do_stuff (ptr, n);

  __analyzer_dump_exploded_nodes (0); /* { dg-warning "2 processed enodes" } */

  if (need_to_free)
    free (ptr); /* { dg-bogus "not on the heap" } */

  return result; /* { dg-bogus "leak" } */
}

/* Pointer comparison.  */

int test_pointer_comparison (int n)
{
  int buf[10];
  int *ptr;
  int result;

  if (n > 10)
    ptr = (int *)malloc (sizeof (int) * n);
  else
    ptr = buf;

  __analyzer_dump_exploded_nodes (0); /* { dg-warning "2 processed enodes" } */

  result = do_stuff (ptr, n);

  __analyzer_dump_exploded_nodes (0); /* { dg-warning "2 processed enodes" } */

  if (ptr != buf)
    free (ptr); /* { dg-bogus "not on the heap" } */

  return result; /* { dg-bogus "leak" } */
}

/* Set a flag based on a conditional, then use it, then reuse the
   conditional.  */

int test_initial_flag (int n)
{
  int buf[10];
  int *ptr;
  int result;
  int on_heap = 0;

  if (n > 10)
    on_heap = 1;
  else
    on_heap = 0;

  /* Due to state-merging, we lose the relationship between 'n > 10'
     and 'on_heap' here; we have to rely on feasibility-checking
     in the diagnostic_manager to reject the false warnings.  */
  __analyzer_dump_exploded_nodes (0); /* { dg-warning "1 processed enode" } */

  if (on_heap)
    ptr = (int *)malloc (sizeof (int) * n);
  else
    ptr = buf;

  __analyzer_dump_exploded_nodes (0); /* { dg-warning "2 processed enodes" } */

  result = do_stuff (ptr, n);

  __analyzer_dump_exploded_nodes (0); /* { dg-warning "2 processed enodes" } */

  if (n > 10)
    free (ptr); /* { dg-bogus "not on the heap" } */

  return result; /* { dg-bogus "leak" } */
}
