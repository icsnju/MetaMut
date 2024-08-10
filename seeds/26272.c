/* { dg-additional-options "-fanalyzer-transitivity" } */
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

void test (int i, int j)
{
  if (i > 4)
    {
      __analyzer_eval (i > 4); /* { dg-warning "TRUE" } */
      __analyzer_eval (i <= 4); /* { dg-warning "FALSE" } */
      __analyzer_eval (i > 3); /* { dg-warning "TRUE" } */

      __analyzer_eval (i > 5); /* { dg-warning "UNKNOWN" } */
      __analyzer_eval (i != 3); /* { dg-warning "TRUE" } */

      __analyzer_eval (i == 3); /* { dg-warning "FALSE" } */

      __analyzer_eval (i != 4); /* { dg-warning "TRUE" } */
      __analyzer_eval (i == 4); /* { dg-warning "FALSE" } */
      __analyzer_eval (i == 5); /* { dg-warning "UNKNOWN" } */
      __analyzer_eval (i != 5); /* { dg-warning "UNKNOWN" } */
      __analyzer_eval (i < 5); /* { dg-warning "FALSE" } */
      __analyzer_eval (i <= 5); /* { dg-warning "UNKNOWN" } */

      /* Tests of transitivity.  */
      if (j < i)
	{
	  __analyzer_eval (j < i); /* { dg-warning "TRUE" } */
	  __analyzer_eval (j <= 4); /* { dg-warning "UNKNOWN" } */
	}
      else
	{
	  __analyzer_eval (j >= i); /* { dg-warning "TRUE" } */
	  __analyzer_eval (j > 4); /* { dg-warning "TRUE" } */
	}
    }
  else
    {
      __analyzer_eval (i > 4); /* { dg-warning "FALSE" } */
      __analyzer_eval (i <= 4); /* { dg-warning "TRUE" } */
      __analyzer_eval (i > 3); /* { dg-warning "UNKNOWN" } */

      __analyzer_eval (i > 5); /* { dg-warning "FALSE" } */
      __analyzer_eval (i != 3); /* { dg-warning "UNKNOWN" } */

      __analyzer_eval (i == 3); /* { dg-warning "UNKNOWN" } */

      __analyzer_eval (i != 4); /* { dg-warning "UNKNOWN" } */
      __analyzer_eval (i == 4); /* { dg-warning "UNKNOWN" } */
      __analyzer_eval (i == 5); /* { dg-warning "FALSE" } */
      __analyzer_eval (i != 5); /* { dg-warning "TRUE" } */
      __analyzer_eval (i < 5); /* { dg-warning "TRUE" } */
      __analyzer_eval (i <= 5); /* { dg-warning "TRUE" } */
    }
}

void test_2 (int i, int j, int k)
{
  if (i >= j)
    {
      __analyzer_eval (i == k); /* { dg-warning "UNKNOWN" } */
      if (j >= k)
	{
	  __analyzer_eval (i >= k); /* { dg-warning "TRUE" } */
	  __analyzer_eval (i == k); /* { dg-warning "UNKNOWN" } */
	  if (k >= i)
	    __analyzer_eval (i == k); /* { dg-warning "TRUE" } */
	}
    }
}

void test_3 (int flag, unsigned int i)
{
  if (!flag) {
    return;
  }

  __analyzer_eval (flag); /* { dg-warning "TRUE" } */
      
  if (i>0) {
    __analyzer_eval (i > 0); /* { dg-warning "TRUE" } */
    __analyzer_eval (flag); /* { dg-warning "TRUE" } */
  } else {
    __analyzer_eval (i <= 0); /* { dg-warning "TRUE" } */
    __analyzer_eval (flag); /* { dg-warning "TRUE" } */
  }

  __analyzer_eval (flag); /* { dg-warning "TRUE" } */
}

void test_range_int_gt_lt (int i)
{
  if (i > 3)
    if (i < 5)
      __analyzer_eval (i == 4); /* { dg-warning "TRUE" } */
}

void test_range_float_gt_lt (float f)
{
  if (f > 3)
    if (f < 5)
      __analyzer_eval (f == 4); /* { dg-warning "UNKNOWN" } */
}

void test_range_int_ge_lt (int i)
{
  if (i >= 4)
    if (i < 5)
      __analyzer_eval (i == 4); /* { dg-warning "TRUE" } */
}

void test_range_float_ge_lt (float f)
{
  if (f >= 4)
    if (f < 5)
      __analyzer_eval (f == 4); /* { dg-warning "UNKNOWN" } */
}

void test_range_int_gt_le (int i)
{
  if (i > 3)
    if (i <= 4)
      __analyzer_eval (i == 4); /* { dg-warning "TRUE" } */
}

void test_range_float_gt_le (float f)
{
  if (f > 3)
    if (f <= 4)
      __analyzer_eval (f == 4); /* { dg-warning "UNKNOWN" } */
}

void test_range_int_ge_le (int i)
{
  if (i >= 4)
    if (i <= 4)
      __analyzer_eval (i == 4); /* { dg-warning "TRUE" } */
}

void test_range_float_ge_le (float f)
{
  if (f >= 4)
    if (f <= 4)
      __analyzer_eval (f == 4); /* { dg-warning "TRUE" "PR 93356" { xfail *-*-* } } */
  /* { dg-warning "UNKNOWN" "disabled float comparisons" { target *-*-* } .-1 } */
}

void test_float_selfcmp (float f)
{
  __analyzer_eval (f == f); /* { dg-warning "UNKNOWN" } */
  __analyzer_eval (f != f); /* { dg-warning "UNKNOWN" } */
}
