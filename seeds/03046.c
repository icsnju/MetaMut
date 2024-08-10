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

struct coord
{
  int x;
  int y;
};

/* Copying from one on-stack array to another.  */

void test_1 (void)
{
  struct coord arr_a[16];
  struct coord arr_b[16];
  arr_a[3].x = 5;
  arr_a[3].y = 6;

  arr_b[7] = arr_a[3];

  __analyzer_eval (arr_b[7].x == 5); /* { dg-warning "TRUE" } */
  __analyzer_eval (arr_b[7].y == 6); /* { dg-warning "TRUE" } */
}

/* Copying from an on-stack array to a global array.  */

struct coord glob_arr2[16];

void test_2 (void)
{
  struct coord arr[16];
  arr[3].x = 5;
  arr[3].y = 6;

  glob_arr2[7] = arr[3];

  __analyzer_eval (glob_arr2[7].x == 5); /* { dg-warning "TRUE" } */
  __analyzer_eval (glob_arr2[7].y == 6); /* { dg-warning "TRUE" } */
}

/* Copying from a partially initialized on-stack array to a global array.  */

struct coord glob_arr3[16];

void test_3 (void)
{
  struct coord arr[16];
  arr[3].y = 6;

  glob_arr3[7] = arr[3]; // or should the uninit warning be here?

  __analyzer_eval (glob_arr3[7].x); /* { dg-warning "uninitialized" "uninit" { xfail *-*-* } } */
  /* { dg-bogus "UNKNOWN" "unknown" { xfail *-*-* } .-1 } */
  __analyzer_eval (glob_arr3[7].y == 6); /* { dg-warning "TRUE" } */
}

/* Symbolic bindings: copying from one array to another.  */

void test_4 (int i)
{
  struct coord arr_a[16];
  struct coord arr_b[16];
  arr_a[i].x = 5;
  arr_a[i].y = 6;
  __analyzer_eval (arr_a[i].x == 5); /* { dg-warning "TRUE" "TRUE" { xfail *-*-* } } */
  /* { dg-bogus "UNKNOWN" "UNKNOWN" { xfail *-*-* } .-1 } */
  __analyzer_eval (arr_a[i].y == 6); /* { dg-warning "TRUE" } */

  arr_b[i] = arr_a[i];

  __analyzer_eval (arr_b[i].x == 5); /* { dg-warning "TRUE" "TRUE" { xfail *-*-* } } */
  /* { dg-bogus "UNKNOWN" "UNKNOWN" { xfail *-*-* } .-1 } */
  __analyzer_eval (arr_b[i].y == 6); /* { dg-warning "TRUE" "TRUE" { xfail *-*-* } } */
  /* { dg-bogus "UNKNOWN" "UNKNOWN" { xfail *-*-* } .-1 } */
}

/* Symbolic bindings: copying within an array: symbolic src and dest  */

void test_5a (int i, int j)
{
  struct coord arr[16];
  arr[i].x = 5;
  arr[i].y = 6;

  arr[j] = arr[i];

  __analyzer_eval (arr[j].x == 5); /* { dg-warning "TRUE" "TRUE" { xfail *-*-* } } */
  /* { dg-bogus "UNKNOWN" "UNKNOWN" { xfail *-*-* } .-1 } */
  __analyzer_eval (arr[j].y == 6); /* { dg-warning "TRUE" "TRUE" { xfail *-*-* } } */
  /* { dg-bogus "UNKNOWN" "UNKNOWN" { xfail *-*-* } .-1 } */
}

/* Symbolic bindings: copying within an array: symbolic src, concrete dest.  */

void test_5b (int i)
{
  struct coord arr[16];
  arr[i].x = 5;
  arr[i].y = 6;

  arr[3] = arr[i];

  __analyzer_eval (arr[3].x == 5); /* { dg-warning "TRUE" "TRUE" { xfail *-*-* } } */
  /* { dg-bogus "UNKNOWN" "UNKNOWN" { xfail *-*-* } .-1 } */
  __analyzer_eval (arr[3].y == 6); /* { dg-warning "TRUE" "TRUE" { xfail *-*-* } } */
  /* { dg-bogus "UNKNOWN" "UNKNOWN" { xfail *-*-* } .-1 } */
}

/* Symbolic bindings: copying within an array: concrete src, symbolic dest.  */

void test_5c (int i)
{
  struct coord arr[16];
  arr[3].x = 5;
  arr[3].y = 6;

  arr[i] = arr[3];

  __analyzer_eval (arr[i].x == 5); /* { dg-warning "TRUE" "TRUE" { xfail *-*-* } } */
  /* { dg-bogus "UNKNOWN" "UNKNOWN" { xfail *-*-* } .-1 } */
  __analyzer_eval (arr[i].y == 6); /* { dg-warning "TRUE" "TRUE" { xfail *-*-* } } */
  /* { dg-bogus "UNKNOWN" "UNKNOWN" { xfail *-*-* } .-1 } */
}

/* No info on the subregion being copied, and hence
   binding_cluster2::maybe_get_compound_binding should return NULL.  */

struct coord glob_arr6[16];

void test_6 (void)
{
  struct coord arr[16];
  arr[7] = glob_arr6[3];

  __analyzer_eval (arr[7].x == 5); /* { dg-warning "UNKNOWN" } */
  __analyzer_eval (arr[7].y == 6); /* { dg-warning "UNKNOWN" } */
}
