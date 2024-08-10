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

void test_1 (void)
{
  __analyzer_dump_path (); /* { dg-message "path" } */
}

void test_2 (int flag)
{
  if (flag)
    __analyzer_dump_path (); /* { dg-message "path" } */
}

void test_3 (int flag)
{
  if (flag)
    if (!flag)
      __analyzer_dump_path (); /* { dg-bogus "path" } */
}

int global_for_test_4;
static void __attribute__((noinline)) called_by_test_4 () {}
void test_4 (void)
{
  /* Verify that a state change that happens in a stmt that
     isn't the first within its BB can affect path feasibility.  */
  global_for_test_4 = 0;
  global_for_test_4 = 1;
  /* Thwart the optimizer.  */
  called_by_test_4 ();
  if (global_for_test_4)
    __analyzer_dump_path (); /* { dg-message "path" } */
}

/* Verify that loops don't confuse the feasibility checker.  */

void test_5 (void)
{
  for (int i = 0; i < 1024; i++)
    {
    }
  __analyzer_dump_path (); /* { dg-message "path" } */
}

/* Reproducer for an issue seen with CVE-2005-1689 (PR analyzer/96374): if we
   take the shortest path and update state and check feasibility per-edge, we
   can erroneously reject valid diagnostics.  */

int test_6 (int a, int b)
{
  int problem = 0;
  if (a)
    problem = 1;
  if (b)
    {
      if (!problem)
	problem = 2;
      __analyzer_dump_path (); /* { dg-message "path" } */
    }
  return problem;
}

/* As above, but call a static function.
   Even if the path to the call of called_by_test_6a is falsely rejected
   as infeasible, it still makes sense to complain about errors within
   the called function.  */

static void __attribute__((noinline))
called_by_test_6a (void *ptr)
{
  __builtin_free (ptr);
  __builtin_free (ptr); /* { dg-message "double-'free'" } */
}

int test_6a (int a, int b, void *ptr)
{
  int problem = 0;
  if (a)
    problem = 1;
  if (b)
    {
      if (!problem)
	problem = 2;
      called_by_test_6a (ptr);
    }
  return problem;
}

/* After state-merging, the shortest path skips the loop,
   but the shortest feasible path enters it.  */

void test_7 (int n)
{
  int entered_loop = 0;
  int i;
  for (i = 0; i < n; i++)
    entered_loop = 1;
  if (entered_loop)
    __analyzer_dump_path (); /* { dg-message "path" } */
}
