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

struct st
{
  void *ptr[10];
  int arr[10];
};

/* Various combinations of a pair of writes, involving
   symbolic vs concrete clusters, with symbolic vs concrete keys
   within them.  */

struct st g;

/* "ptr" write: fully concrete.  */

struct st
test_conc_conc_ptr_conc_conc_arr (void)
{
  struct st s;
  s.ptr[1] = __builtin_malloc (1024);
  __analyzer_describe (0, s.ptr[1]); /* { dg-warning "HEAP_ALLOCATED_REGION" } */
  s.arr[5] = 42;
  __analyzer_describe (0, s.ptr[1]); /* { dg-warning "HEAP_ALLOCATED_REGION" } */
  __analyzer_describe (0, s.arr[5]);  /* { dg-warning "42" } */
  return s;
}

struct st
test_conc_conc_ptr_conc_sym_arr (int j)
{
  struct st s;
  s.ptr[1] = __builtin_malloc (1024);
  __analyzer_describe (0, s.ptr[1]); /* { dg-warning "HEAP_ALLOCATED_REGION" } */
  s.arr[j] = 42;
  __analyzer_describe (0, s.ptr[1]); /* { dg-warning "UNKNOWN" } */
  __analyzer_describe (0, s.arr[j]);  /* { dg-warning "42" } */
  return s;
}

struct st
test_conc_conc_ptr_sym_conc_arr (struct st *p)
{
  struct st s;
  s.ptr[1] = __builtin_malloc (1024);
  __analyzer_describe (0, s.ptr[1]); /* { dg-warning "HEAP_ALLOCATED_REGION" } */
  p->arr[5] = 42;
  __analyzer_describe (0, s.ptr[1]); /* { dg-warning "HEAP_ALLOCATED_REGION" } */
  __analyzer_describe (0, p->arr[5]);  /* { dg-warning "42" } */
  return s;
}

struct st
test_conc_conc_ptr_sym_sym_arr (struct st *p, int j)
{
  struct st s;
  s.ptr[1] = __builtin_malloc (1024);
  __analyzer_describe (0, s.ptr[1]); /* { dg-warning "HEAP_ALLOCATED_REGION" } */
  p->arr[j] = 42;
  __analyzer_describe (0, s.ptr[1]); /* { dg-warning "HEAP_ALLOCATED_REGION" } */
  __analyzer_describe (0, p->arr[j]);  /* { dg-warning "42" } */
  return s;
}

/* "ptr" write: symbolic region, but at concrete offset.  */

void
test_sym_conc_ptr_conc_conc_arr (struct st *p)
{
  p->ptr[1] = __builtin_malloc (1024);
  __analyzer_describe (0, p->ptr[1]); /* { dg-warning "HEAP_ALLOCATED_REGION" } */
  g.arr[5] = 42;
  __analyzer_describe (0, p->ptr[1]); /* { dg-warning "UNKNOWN" } */
  __analyzer_describe (0, g.arr[5]);  /* { dg-warning "42" } */
}

void
test_sym_conc_ptr_conc_sym_arr (struct st *p, int j)
{
  p->ptr[1] = __builtin_malloc (1024);
  __analyzer_describe (0, p->ptr[1]); /* { dg-warning "HEAP_ALLOCATED_REGION" } */
  g.arr[j] = 42;
  __analyzer_describe (0, p->ptr[1]); /* { dg-warning "UNKNOWN" } */
  __analyzer_describe (0, g.arr[j]);  /* { dg-warning "42" } */
}

void
test_sym_conc_ptr_sym_conc_arr (struct st *p, struct st *q)
{
  p->ptr[1] = __builtin_malloc (1024);
  __analyzer_describe (0, p->ptr[1]); /* { dg-warning "HEAP_ALLOCATED_REGION" } */
  q->arr[5] = 42;
  __analyzer_describe (0, p->ptr[1]); /* { dg-warning "UNKNOWN" } */
  __analyzer_describe (0, q->arr[5]);  /* { dg-warning "42" } */
}

void
test_sym_conc_ptr_sym_sym_arr (struct st *p, struct st *q, int j)
{
  p->ptr[1] = __builtin_malloc (1024);
  __analyzer_describe (0, p->ptr[1]); /* { dg-warning "HEAP_ALLOCATED_REGION" } */
  q->arr[j] = 42;
  __analyzer_describe (0, p->ptr[1]); /* { dg-warning "UNKNOWN" } */
  __analyzer_describe (0, q->arr[j]);  /* { dg-warning "42" } */
}

/* "ptr" write: concrete region, but at symbolic offset.  */

struct st
test_conc_sym_ptr_conc_conc_arr (int i)
{
  struct st s;
  s.ptr[i] = __builtin_malloc (1024);
  __analyzer_describe (0, s.ptr[i]); /* { dg-warning "HEAP_ALLOCATED_REGION" } */
  s.arr[5] = 42;
  __analyzer_describe (0, s.ptr[i]); /* { dg-warning "UNKNOWN" } */
  __analyzer_describe (0, s.arr[5]);  /* { dg-warning "42" } */
  return s;
}

struct st
test_conc_sym_ptr_conc_sym_arr (int i, int j)
{
  struct st s;
  s.ptr[i] = __builtin_malloc (1024);
  __analyzer_describe (0, s.ptr[i]); /* { dg-warning "HEAP_ALLOCATED_REGION" } */
  s.arr[j] = 42;
  __analyzer_describe (0, s.ptr[i]); /* { dg-warning "UNKNOWN" } */
  __analyzer_describe (0, s.arr[j]);  /* { dg-warning "42" } */
  return s;
}

struct st
test_conc_sym_ptr_sym_conc_arr (int i, struct st *p)
{
  struct st s;
  s.ptr[i] = __builtin_malloc (1024);
  __analyzer_describe (0, s.ptr[i]); /* { dg-warning "HEAP_ALLOCATED_REGION" } */
  p->arr[5] = 42;
  __analyzer_describe (0, s.ptr[i]); /* { dg-warning "HEAP_ALLOCATED_REGION" } */
  __analyzer_describe (0, p->arr[5]);  /* { dg-warning "42" } */
  return s;
} /* { dg-bogus "leak" "PR analyzer/105190" { xfail *-*-* } } */

struct st
test_conc_sym_ptr_sym_sym_arr (int i, struct st *p, int j)
{
  struct st s;
  s.ptr[i] = __builtin_malloc (1024);
  __analyzer_describe (0, s.ptr[i]); /* { dg-warning "HEAP_ALLOCATED_REGION" } */
  p->arr[j] = 42;
  __analyzer_describe (0, s.ptr[i]); /* { dg-warning "HEAP_ALLOCATED_REGION" } */
  __analyzer_describe (0, p->arr[j]);  /* { dg-warning "42" } */
  return s;
} /* { dg-bogus "leak" "PR analyzer/105190" { xfail *-*-* } } */

/* "ptr" write: symbolic region, with symbolic offset.  */

void
test_sym_sym_ptr_conc_conc_arr (struct st *p, int i)
{
  p->ptr[i] = __builtin_malloc (1024);
  __analyzer_describe (0, p->ptr[i]); /* { dg-warning "HEAP_ALLOCATED_REGION" } */
  g.arr[5] = 42;
  __analyzer_describe (0, p->ptr[i]); /* { dg-warning "UNKNOWN" } */
  __analyzer_describe (0, g.arr[5]);  /* { dg-warning "42" } */
}

void
test_sym_sym_ptr_conc_sym_arr (struct st *p, int i, int j)
{
  p->ptr[i] = __builtin_malloc (1024);
  __analyzer_describe (0, p->ptr[i]); /* { dg-warning "HEAP_ALLOCATED_REGION" } */
  g.arr[j] = 42;
  __analyzer_describe (0, p->ptr[i]); /* { dg-warning "UNKNOWN" } */
  __analyzer_describe (0, g.arr[j]);  /* { dg-warning "42" } */
}

void
test_sym_sym_ptr_sym_conc_arr (struct st *p, int i, struct st *q)
{
  p->ptr[i] = __builtin_malloc (1024);
  __analyzer_describe (0, p->ptr[i]); /* { dg-warning "HEAP_ALLOCATED_REGION" } */
  q->arr[5] = 42;
  __analyzer_describe (0, p->ptr[i]); /* { dg-warning "UNKNOWN" } */
  __analyzer_describe (0, q->arr[5]);  /* { dg-warning "42" } */
}

void
test_sym_sym_ptr_sym_sym_arr (struct st *p, int i, struct st *q, int j)
{
  p->ptr[i] = __builtin_malloc (1024);
  __analyzer_describe (0, p->ptr[i]); /* { dg-warning "HEAP_ALLOCATED_REGION" } */
  q->arr[j] = 42;
  __analyzer_describe (0, p->ptr[i]); /* { dg-warning "UNKNOWN" } */
  __analyzer_describe (0, q->arr[j]);  /* { dg-warning "42" } */
}
