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

struct iter
{
  int start;
  int end;
  int step;
  int val;
};

struct iter *  __attribute__((noinline))
iter_new (int start, int end, int step)
{
  struct iter *it = (struct iter *)malloc (sizeof (struct iter));
  if (!it)
    abort ();
  it->start = start;
  it->end = end;
  it->step = step;
  it->val = start;
  return it;
}

int __attribute__((noinline))
iter_done_p (struct iter *it)
{
  return it->val >= it->end;
}

void __attribute__((noinline))
iter_next (struct iter *it)
{
  it->val += it->step;
}

/* Example of an iterator object, to see how well we cope with a well-disguised
   iteration from 0 to n with a step of 1.  */

void test(int n)
{
  __analyzer_dump_exploded_nodes (0); /* { dg-warning "1 processed enode" } */

  struct iter *it = iter_new (0, n, 1);
  while (!iter_done_p (it))
    {
      __analyzer_eval (it->val < n); /* { dg-warning "TRUE" "true" } */
      /* { dg-bogus "UNKNOWN" "unknown" { xfail *-*-* } .-1 } */
      /* TODO(xfail^^^): ideally we ought to figure out i > 0 after 1st iteration.  */

      __analyzer_eval (it->val == 0); /* { dg-warning "TRUE" "true on 1st iter" } */
      /* { dg-warning "UNKNOWN" "unknown" { target *-*-* } .-1 } */
      /* TODO: should we ought to figure out i > 0 after 1st iteration?  */

      __analyzer_eval (it->val >= 0); /* { dg-warning "TRUE" } */

      __analyzer_dump_exploded_nodes (0); /* { dg-warning "2 processed enodes" } */

      iter_next (it);
    }

  __analyzer_eval (it->val >= n); /* { dg-warning "TRUE" "true" { xfail *-*-* } } */
  /* { dg-bogus "UNKNOWN" "unknown" { xfail *-*-* } .-1 } */

  __analyzer_eval (it->val == n); /* { dg-warning "TRUE" "desired" { xfail *-*-* } } */
  /* { dg-warning "UNKNOWN" "status quo" { target *-*-* } .-1 } */
  /* TODO(xfail^^^): it only figures out i >= 256, rather than i == 256.  */

  free (it);

  __analyzer_dump_exploded_nodes (0); /* { dg-warning "1 processed enode" } */
}
