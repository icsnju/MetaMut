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

typedef __SIZE_TYPE__ size_t;

extern void* (*my_alloc)(size_t) __attribute__ ((alloc_size (1)));
extern void* (*my_alloc_2)(size_t, size_t) __attribute__ ((alloc_size (1, 2)));

int test_one_arg_concrete_int_ptr (void)
{
  int *x = (int *) my_alloc (1); /* { dg-warning "allocated buffer size is not a multiple of the pointee's size" } */
  __analyzer_dump_capacity (x); /* { dg-warning "capacity: '\\(\[^\n\r\]*\\)1'" } */
  x[0] = 0; /* { dg-warning "buffer overflow" } */
  return 0;
}

void test_one_arg_concrete (void)
{
  char *p = (char *) my_alloc (10);
  __analyzer_dump_capacity (p); /* { dg-warning "capacity: '\\(\[^\n\r\]*\\)10'" } */
  p[0] = 'a';
  p[9] = 'b';
  p[10] = 'c'; /* { dg-warning "buffer overflow" } */
  p[11] = 'c'; /* { dg-warning "buffer overflow" } */
}

void test_one_arg_symbolic (size_t sz)
{
  char *p = (char *) my_alloc (sz);
  __analyzer_dump_capacity (p); /* { dg-warning "capacity: 'INIT_VAL\\(sz" } */
}

void test_two_args_concrete (void)
{
  char *p = (char *) my_alloc_2 (2, 5);
  __analyzer_dump_capacity (p); /* { dg-warning "capacity: '\\(\[^\n\r\]*\\)10'" } */
  p[0] = 'a';
  p[9] = 'b';
  p[10] = 'c'; /* { dg-warning "buffer overflow" } */
  p[11] = 'c'; /* { dg-warning "buffer overflow" } */
}

void test_two_args_symbolic_first (size_t sz)
{
  char *p = (char *) my_alloc_2 (sz, 5);
  __analyzer_dump_capacity (p); /* { dg-warning "capacity: '\\(INIT_VAL\\(sz\[^\n\r\]*\\*\\(size_t\\)5\\)'" } */
}

void test_two_args_symbolic_second (size_t sz)
{
  char *p = (char *) my_alloc_2 (5, sz);
  __analyzer_dump_capacity (p); /* { dg-warning "capacity: '\\(INIT_VAL\\(sz\[^\n\r\]*\\*\\(size_t\\)5\\)'" } */
}

void test_two_args_symbolic_both (size_t a, size_t b)
{
  char *p = (char *) my_alloc_2 (a, b);
  __analyzer_dump_capacity (p); /* { dg-warning "capacity: '\\(INIT_VAL\\(a\[^\n\r\]*\\*INIT_VAL\\(b" } */
}

typedef void* (*my_alloc_t)(size_t) __attribute__ ((alloc_size (1)));
typedef void* (*my_alloc_2_t)(size_t, size_t) __attribute__ ((alloc_size (1, 2)));

void test_one_arg_concrete_fnptr (my_alloc_t fnptr)
{
  char *p = (char *) fnptr (10);
  __analyzer_dump_capacity (p); /* { dg-warning "capacity: '\\(\[^\n\r\]*\\)10'" } */
  p[0] = 'a';
  p[9] = 'b';
  p[10] = 'c'; /* { dg-warning "buffer overflow" } */
  p[11] = 'c'; /* { dg-warning "buffer overflow" } */
}

void test_two_args_concrete_fnptr (my_alloc_2_t fnptr)
{
  char *p = (char *) fnptr (2, 5);
  __analyzer_dump_capacity (p); /* { dg-warning "capacity: '\\(\[^\n\r\]*\\)10'" } */
  p[0] = 'a';
  p[9] = 'b';
  p[10] = 'c'; /* { dg-warning "buffer overflow" } */
  p[11] = 'c'; /* { dg-warning "buffer overflow" } */
}
