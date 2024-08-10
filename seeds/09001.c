/* { dg-additional-options "-Wno-free-nonheap-object" } */

typedef __SIZE_TYPE__ size_t;

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

extern void *malloc (size_t __size)
  __attribute__ ((__nothrow__ , __leaf__))
  __attribute__ ((__malloc__))
  __attribute__ ((__alloc_size__ (1)));
extern void *realloc (void *__ptr, size_t __size)
  __attribute__ ((__nothrow__ , __leaf__))
  __attribute__ ((__warn_unused_result__))
  __attribute__ ((__alloc_size__ (2)));
extern void free (void *__ptr)
  __attribute__ ((__nothrow__ , __leaf__));

void *test_1 (void *ptr)
{
  return realloc (ptr, 1024);
}

void test_2 (void *ptr)
{
  void *p = malloc (1024); /* { dg-message "allocated here" } */
  p = realloc (p, 4096); /* { dg-message "when 'realloc' fails" } */
  free (p);
} /* { dg-warning "leak of 'p'" } */ // ideally this would be on the realloc stmt

void test_3 (void *ptr)
{
  void *p = malloc (1024);
  void *q = realloc (p, 4096);
  if (q)
    free (q);
  else
    free (p);
}

void *test_4 (void)
{
  return realloc (NULL, 1024);
}

int *test_5 (int *p)
{
  *p = 42;
  int *q = (int *) realloc (p, sizeof(int) * 4); /* { dg-message "when 'realloc' fails" } */
  *q = 43; /* { dg-warning "dereference of NULL 'q'" } */
  return q;
}

void test_6 (size_t sz)
{
  void *p = realloc (NULL, sz);
} /* { dg-warning "leak of 'p'" } */

/* The analyzer should complain about realloc of non-heap.  */

void *test_7 (size_t sz)
{
  char buf[100]; /* { dg-message "region created on stack here" } */
  void *p = realloc (&buf, sz); /* { dg-warning "'realloc' of '& ?buf' which points to memory on the stack" } */
  return p;  
}

/* Mismatched allocator.  */

struct foo
{
  int m_int;
};

extern void foo_release (struct foo *);
extern struct foo *foo_acquire (void)
  __attribute__ ((malloc (foo_release)));

void test_8 (void)
{
  struct foo *p = foo_acquire ();
  void *q = realloc (p, 1024); /* { dg-warning "'p' should have been deallocated with 'foo_release' but was deallocated with 'realloc'" } */
}

/* We should complain about realloc on a freed pointer.  */

void test_9 (void *p)
{
  free (p);
  void *q = realloc (p, 1024); /* { dg-warning "double-'free' of 'p'" } */
}

void test_10 (char *s, int n)
{
  __builtin_realloc(s, n); /* { dg-warning "ignoring return value of '__builtin_realloc' declared with attribute 'warn_unused_result'" "" { target c } } */
  /* { dg-warning "ignoring return value of 'void\\* __builtin_realloc\\(void\\*, (long )?unsigned int\\)' declared with attribute 'warn_unused_result'" "" { target c++ } .-1 } */
} /* { dg-warning "leak" } */
