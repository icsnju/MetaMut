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

struct foo
{
  int i;
  int j;
};

struct coord
{
  int x;
  int y;
  int z;
};

struct foo g;

void test_1 (void)
{
  g.i = 42;
  if (g.j)
    __analyzer_eval (g.j); /* { dg-warning "TRUE" } */
  else
    __analyzer_eval (g.j); /* { dg-warning "FALSE" } */
  __analyzer_dump_exploded_nodes (0); /* { dg-warning "1 processed enode" } */
}

void test_2 (struct foo f)
{
  f.i = 42;
  if (f.j)
    __analyzer_eval (f.j); /* { dg-warning "TRUE" } */
  else
    __analyzer_eval (f.j); /* { dg-warning "FALSE" } */
  __analyzer_dump_exploded_nodes (0); /* { dg-warning "1 processed enode" } */
}

void test_3 (struct foo *p)
{
  struct foo f = *p;
  f.i = 42;
  if (f.j)
    __analyzer_eval (f.j); /* { dg-warning "TRUE" } */
  else
    __analyzer_eval (f.j); /* { dg-warning "FALSE" } */
  __analyzer_dump_exploded_nodes (0); /* { dg-warning "1 processed enode" } */
}

void test_4 (struct coord *p)
{
  struct coord f = *p;
  f.x = 42;
  __analyzer_eval (f.y == p->y); /* { dg-warning "TRUE" } */
  __analyzer_eval (f.z == p->z); /* { dg-warning "TRUE" } */
}

struct s5
{
  char arr[8];
};

void test_5 (struct s5 *p)
{
  struct s5 f = *p;
  f.arr[3] = 42;
  __analyzer_eval (f.arr[0] == p->arr[0]); /* { dg-warning "TRUE" } */
  __analyzer_eval (f.arr[1] == p->arr[1]); /* { dg-warning "TRUE" } */
  __analyzer_eval (f.arr[2] == p->arr[2]); /* { dg-warning "TRUE" } */
  __analyzer_eval (f.arr[3] == 42); /* { dg-warning "TRUE" } */
  __analyzer_eval (f.arr[4] == p->arr[4]); /* { dg-warning "TRUE" } */
  __analyzer_eval (f.arr[5] == p->arr[5]); /* { dg-warning "TRUE" } */
  __analyzer_eval (f.arr[6] == p->arr[6]); /* { dg-warning "TRUE" } */
  __analyzer_eval (f.arr[7] == p->arr[7]); /* { dg-warning "TRUE" } */
}

struct s6
{
  int before; /* Give "arr" a nonzero offset.  */
  struct foo arr[4];
  int after;
};

void test_6 (struct s6 *p, struct foo *q)
{
  struct s6 f = *p;
  f.arr[1] = *q;
  __analyzer_eval (f.before == p->before); /* { dg-warning "TRUE" } */
  __analyzer_eval (f.arr[0].i == p->arr[0].i); /* { dg-warning "TRUE" } */
  __analyzer_eval (f.arr[0].j == p->arr[0].j); /* { dg-warning "TRUE" } */
  __analyzer_eval (f.arr[1].i == q->i); /* { dg-warning "TRUE" } */
  __analyzer_eval (f.arr[1].j == q->j); /* { dg-warning "TRUE" } */
  __analyzer_eval (f.arr[2].i == p->arr[2].i); /* { dg-warning "TRUE" } */
  __analyzer_eval (f.arr[2].j == p->arr[2].j); /* { dg-warning "TRUE" } */
  __analyzer_eval (f.arr[3].i == p->arr[3].i); /* { dg-warning "TRUE" } */
  __analyzer_eval (f.arr[3].j == p->arr[3].j); /* { dg-warning "TRUE" } */
  __analyzer_eval (f.after == p->after); /* { dg-warning "TRUE" } */
}
