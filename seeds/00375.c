// TODO: remove need for this option:
/* { dg-additional-options "-fanalyzer-checker=taint" } */

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
#include <stdio.h>

struct st1
{
  int a;
  int b;
};


int test_1 (FILE *f)
{
  struct st1 s;
  fread (&s, sizeof (s), 1, f);
  return s.a / s.b;  /* { dg-warning "use of attacker-controlled value 's\\.b' as divisor without checking for zero" } */
}

int test_2 (FILE *f)
{
  struct st1 s;
  fread (&s, sizeof (s), 1, f);
  return s.a % s.b;  /* { dg-warning "use of attacker-controlled value 's\\.b' as divisor without checking for zero" } */
}

/* We shouldn't complain if the divisor has been checked for zero.  */

int test_checked_ne_zero (FILE *f)
{
  struct st1 s;
  fread (&s, sizeof (s), 1, f);
  if (s.b)
    return s.a / s.b; /* { dg-bogus "divisor" } */
  else
    return 0;
}

int test_checked_gt_zero (FILE *f)
{
  struct st1 s;
  fread (&s, sizeof (s), 1, f);
  if (s.b > 0)
    return s.a / s.b; /* { dg-bogus "divisor" } */
  else
    return 0;
}

int test_checked_lt_zero (FILE *f)
{
  struct st1 s;
  fread (&s, sizeof (s), 1, f);
  if (s.b < 0)
    return s.a / s.b; /* { dg-bogus "divisor" } */
  else
    return 0;
}

/* We should complain if the check on the divisor still allows it to be
   zero.  */

int test_checked_ge_zero (FILE *f)
{
  struct st1 s;
  fread (&s, sizeof (s), 1, f);
  if (s.b >= 0)
    return s.a / s.b;  /* { dg-warning "use of attacker-controlled value 's\\.b' as divisor without checking for zero" } */
  else
    return 0;
}

int test_checked_le_zero (FILE *f)
{
  struct st1 s;
  fread (&s, sizeof (s), 1, f);
  if (s.b <= 0)
    return s.a / s.b;  /* { dg-warning "use of attacker-controlled value 's\\.b' as divisor without checking for zero" } */
  else
    return 0;
}

int test_checked_eq_zero (FILE *f)
{
  struct st1 s;
  fread (&s, sizeof (s), 1, f);
  /* Wrong sense of test.  */
  if (s.b != 0)
    return 0;
  else
    return s.a / s.b;  /* { dg-warning "use of attacker-controlled value 's\\.b' as divisor without checking for zero" } */
}
