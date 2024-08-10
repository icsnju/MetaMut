/* Tests of brace-enclosed initializers
   Some of these use the CONSTRUCTOR tree code, but it appears
   only for a full zero-init; it appears that by the time the analyzer
   runs that this initialization has been converted into field-wise
   gimple assign stmts, with just "zero-init everything" CONSTRUCTORs
   and "clobber" CONSTRUCTORs.  */

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

struct tri
{
  struct coord v[3];
};

union iap
{
  int i;
  void *p;
};

void test_1 (void)
{
  struct coord c = {3, 4};
  __analyzer_eval (c.x == 3); /* { dg-warning "TRUE" } */
  __analyzer_eval (c.y == 4); /* { dg-warning "TRUE" } */  
}

void test_2 (void)
{
  struct coord c = {3};
  __analyzer_eval (c.x == 3); /* { dg-warning "TRUE" } */
  __analyzer_eval (c.y == 0); /* { dg-warning "TRUE" } */  
}

void test_3 (void)
{
  struct coord c = {};
  __analyzer_eval (c.x == 0); /* { dg-warning "TRUE" } */
  __analyzer_eval (c.y == 0); /* { dg-warning "TRUE" } */  
}

void test_4 (void)
{
  int c[2] = {3, 4};
  __analyzer_eval (c[0] == 3); /* { dg-warning "TRUE" } */
  __analyzer_eval (c[1] == 4); /* { dg-warning "TRUE" } */  
}

void test_5 (void)
{
  int c[2] = {3};
  __analyzer_eval (c[0] == 3); /* { dg-warning "TRUE" } */
  __analyzer_eval (c[1] == 0); /* { dg-warning "TRUE" } */  
}

void test_6 (void)
{
  int c[2] = {};
  __analyzer_eval (c[0] == 0); /* { dg-warning "TRUE" } */
  __analyzer_eval (c[1] == 0); /* { dg-warning "TRUE" } */  
}

void test_7 (void)
{
  struct coord c[2] = {{3, 4}, {5, 6}};
  __analyzer_eval (c[0].x == 3); /* { dg-warning "TRUE" } */
  __analyzer_eval (c[0].y == 4); /* { dg-warning "TRUE" } */  
  __analyzer_eval (c[1].x == 5); /* { dg-warning "TRUE" } */
  __analyzer_eval (c[1].y == 6); /* { dg-warning "TRUE" } */  
}

void test_8 (void)
{
  struct coord c[2] = {{3}, {5}};
  __analyzer_eval (c[0].x == 3); /* { dg-warning "TRUE" } */
  __analyzer_eval (c[0].y == 0); /* { dg-warning "TRUE" } */  
  __analyzer_eval (c[1].x == 5); /* { dg-warning "TRUE" } */
  __analyzer_eval (c[1].y == 0); /* { dg-warning "TRUE" } */  
}

void test_9 (void)
{
  struct coord c[2] = {{}, {}};
  __analyzer_eval (c[0].x == 0); /* { dg-warning "TRUE" } */
  __analyzer_eval (c[0].y == 0); /* { dg-warning "TRUE" } */  
  __analyzer_eval (c[1].x == 0); /* { dg-warning "TRUE" } */
  __analyzer_eval (c[1].y == 0); /* { dg-warning "TRUE" } */  
}

void test_10 (void)
{
  struct coord c[2] = {{.x = 3, .y = 4}, {5, 6}};
  __analyzer_eval (c[0].x == 3); /* { dg-warning "TRUE" } */
  __analyzer_eval (c[0].y == 4); /* { dg-warning "TRUE" } */  
  __analyzer_eval (c[1].x == 5); /* { dg-warning "TRUE" } */
  __analyzer_eval (c[1].y == 6); /* { dg-warning "TRUE" } */  
}

void test_11 (void)
{
  struct coord c[2] = {{.y = 4}, {5, 6}};
  __analyzer_eval (c[0].x == 0); /* { dg-warning "TRUE" } */
  __analyzer_eval (c[0].y == 4); /* { dg-warning "TRUE" } */  
  __analyzer_eval (c[1].x == 5); /* { dg-warning "TRUE" } */
  __analyzer_eval (c[1].y == 6); /* { dg-warning "TRUE" } */  
}

void test_12 (void)
{
  struct tri t = {};
  __analyzer_eval (t.v[0].x == 0); /* { dg-warning "TRUE" } */
  __analyzer_eval (t.v[2].y == 0); /* { dg-warning "TRUE" } */  
}

void test_13 (void)
{
  struct tri t = {3, 4, 5, 6, 7, 8};
  __analyzer_eval (t.v[0].x == 3); /* { dg-warning "TRUE" } */
  __analyzer_eval (t.v[0].y == 4); /* { dg-warning "TRUE" } */
  __analyzer_eval (t.v[1].x == 5); /* { dg-warning "TRUE" } */
  __analyzer_eval (t.v[1].y == 6); /* { dg-warning "TRUE" } */
  __analyzer_eval (t.v[2].x == 7); /* { dg-warning "TRUE" } */
  __analyzer_eval (t.v[2].y == 8); /* { dg-warning "TRUE" } */
}

void test_14 (void)
{
  union iap u = {};
  __analyzer_eval (u.i == 0); /* { dg-warning "TRUE" } */
}
