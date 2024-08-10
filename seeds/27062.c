/* Verify that -Wanalyzer-exposure-through-uninit-copy doesn't get confused
   if size argument to copy_to_user is an upper bound, rather than a
   constant.  */

/* { dg-do compile } */
/* { dg-options "-fanalyzer" } */
/* { dg-require-effective-target analyzer } */

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

/* Shared header for testcases for copy_from_user/copy_to_user.  */

/* Adapted from include/linux/compiler.h  */

#define __user

/* Adapted from include/asm-generic/uaccess.h  */

extern long copy_from_user(void *to, const void __user *from, long n);
extern long copy_to_user(void __user *to, const void *from, long n);

typedef unsigned __INT32_TYPE__ u32;

/* min_t adapted from include/linux/kernel.h.  */

#define min_t(type, x, y) ({			\
	type __min1 = (x);			\
	type __min2 = (y);			\
	__min1 < __min2 ? __min1: __min2; })

struct st
{
  u32 a;
  u32 b;
};

/* Verify that we cope with min_t.  */

void test_1_full_init (void __user *dst, u32 x, u32 y, unsigned long in_sz)
{
  struct st s;
  s.a = x;
  s.b = y;
  unsigned long copy_sz = min_t(unsigned long, in_sz, sizeof(s));
  copy_to_user(dst, &s, copy_sz); /* { dg-bogus "exposure" } */
}

void test_1_partial_init (void __user *dst, u32 x, u32 y, unsigned long in_sz)
{
  struct st s;
  s.a = x;
  /* s.y not initialized.  */
  unsigned long copy_sz = min_t(unsigned long, in_sz, sizeof(s));
  copy_to_user(dst, &s, copy_sz); /* { dg-warning "exposure" } */
}

/* Constant on LHS rather than RHS.  */

void test_2_full_init (void __user *dst, u32 x, u32 y, unsigned long in_sz)
{
  struct st s;
  s.a = x;
  s.b = y;
  unsigned long copy_sz = min_t(unsigned long, sizeof(s), in_sz);
  copy_to_user(dst, &s, copy_sz); /* { dg-bogus "exposure" } */
}

void test_2_partial_init (void __user *dst, u32 x, u32 y, unsigned long in_sz)
{
  struct st s;
  s.a = x;
  /* s.y not initialized.  */
  unsigned long copy_sz = min_t(unsigned long, sizeof(s), in_sz);
  copy_to_user(dst, &s, copy_sz); /* { dg-warning "exposure" } */
}

/* min_t with various casts.  */

void test_3_full_init (void __user *dst, u32 x, u32 y, int in_sz)
{
  struct st s;
  s.a = x;
  s.b = y;
  int copy_sz = min_t(unsigned int, in_sz, sizeof(s));
  copy_to_user(dst, &s, copy_sz); /* { dg-bogus "exposure" } */
}

void test_3_partial_init (void __user *dst, u32 x, u32 y, int in_sz)
{
  struct st s;
  s.a = x;
  /* s.y not initialized.  */
  int copy_sz = min_t(unsigned int, in_sz, sizeof(s));
  copy_to_user(dst, &s, copy_sz); /* { dg-warning "exposure" } */
}

/* Comparison against an upper bound.  */

void test_4_full_init (void __user *dst, u32 x, u32 y, size_t in_sz)
{
  struct st s;
  s.a = x;
  s.b = y;
  
  size_t copy_sz = in_sz;
  if (copy_sz > sizeof(s))
    copy_sz = sizeof(s);

  copy_to_user(dst, &s, copy_sz); /* { dg-bogus "exposure" } */
}

void test_4_partial_init (void __user *dst, u32 x, u32 y, size_t in_sz)
{
  struct st s;
  s.a = x;
  /* s.y not initialized.  */
  
  size_t copy_sz = in_sz;
  if (copy_sz > sizeof(s))
    copy_sz = sizeof(s);

  copy_to_user(dst, &s, copy_sz); /* { dg-warning "exposure" } */
}

/* Comparison against an upper bound with casts.  */

void test_5_full_init (void __user *dst, u32 x, u32 y, int in_sz)
{
  struct st s;
  s.a = x;
  s.b = y;
  
  int copy_sz = in_sz;
  if (copy_sz > sizeof(s))
    copy_sz = sizeof(s);
  copy_to_user(dst, &s, copy_sz); /* { dg-bogus "exposure" } */
}

/* Comparison against an upper bound with casts.  */

void test_5_partial_init (void __user *dst, u32 x, u32 y, int in_sz)
{
  struct st s;
  s.a = x;
  /* s.y not initialized.  */
  
  int copy_sz = in_sz;
  if (copy_sz > sizeof(s))
    copy_sz = sizeof(s);

  copy_to_user(dst, &s, copy_sz); /* { dg-warning "exposure" "" } */
}
