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

typedef unsigned char u8;
typedef unsigned __INT16_TYPE__ u16;
typedef unsigned __INT32_TYPE__ u32;

struct st1
{
  u16 nonzero_offset;
  unsigned int f0 : 1;
  unsigned int f1 : 1;
  unsigned int f2 : 1;
  unsigned int f3 : 1;
  unsigned int f4 : 1;
  unsigned int f5 : 1;
  unsigned int f6 : 1;
  unsigned int f7 : 1;
};

void test_1 (void)
{
  struct st1 s;
  s.f0 = 0;
  __analyzer_eval (s.f0 == 0); /* { dg-warning "TRUE" } */
  s.f0 = 1;
  __analyzer_eval (s.f0 == 1); /* { dg-warning "TRUE" } */

  s.f1 = 0;
  __analyzer_eval (s.f1 == 0); /* { dg-warning "TRUE" } */
  s.f1 = 1;
  __analyzer_eval (s.f1 == 1); /* { dg-warning "TRUE" } */

  /* etc  */

  s.f6 = 0;
  __analyzer_eval (s.f6 == 0); /* { dg-warning "TRUE" } */
  s.f6 = 1;
  __analyzer_eval (s.f6 == 1); /* { dg-warning "TRUE" } */

  s.f7 = 0;
  __analyzer_eval (s.f7 == 0); /* { dg-warning "TRUE" } */
  s.f7 = 1;
  __analyzer_eval (s.f7 == 1); /* { dg-warning "TRUE" } */
};

void test_2 (_Bool v0, _Bool v1, _Bool v2, _Bool v3,
	     _Bool v4, _Bool v5, _Bool v6, _Bool v7)
{
  struct st1 s;
  s.f0 = v0;
  s.f1 = v1;
  s.f2 = v2;
  s.f3 = v3;
  s.f4 = v4;
  s.f5 = v5;
  s.f6 = v6;
  s.f7 = v7;

  __analyzer_eval (s.f0 == v0); /* { dg-warning "TRUE" } */
  __analyzer_eval (s.f1 == v1); /* { dg-warning "TRUE" } */
  __analyzer_eval (s.f2 == v2); /* { dg-warning "TRUE" } */
  __analyzer_eval (s.f3 == v3); /* { dg-warning "TRUE" } */
  __analyzer_eval (s.f4 == v4); /* { dg-warning "TRUE" } */
  __analyzer_eval (s.f5 == v5); /* { dg-warning "TRUE" } */
  __analyzer_eval (s.f6 == v6); /* { dg-warning "TRUE" } */
  __analyzer_eval (s.f7 == v7); /* { dg-warning "TRUE" } */
};

struct st3
{
  unsigned int f01 : 2;
  unsigned int f23 : 2;
  unsigned int f34 : 2;
  unsigned int f56 : 2;
};

void test_3 (void)
{
  struct st3 s;
  s.f01 = 0;
  __analyzer_eval (s.f01 == 0); /* { dg-warning "TRUE" } */
  s.f01 = 1;
  __analyzer_eval (s.f01 == 1); /* { dg-warning "TRUE" } */
  s.f01 = 2;
  __analyzer_eval (s.f01 == 2); /* { dg-warning "TRUE" } */
  s.f01 = 3;
  __analyzer_eval (s.f01 == 3); /* { dg-warning "TRUE" } */

  /* etc  */

  s.f56 = 0;
  __analyzer_eval (s.f56 == 0); /* { dg-warning "TRUE" } */
  s.f56 = 1;
  __analyzer_eval (s.f56 == 1); /* { dg-warning "TRUE" } */
  s.f56 = 2;
  __analyzer_eval (s.f56 == 2); /* { dg-warning "TRUE" } */
  s.f56 = 3;
  __analyzer_eval (s.f56 == 3); /* { dg-warning "TRUE" } */
};

/* A signed bitfield.  */

struct st4
{
  signed int f012 : 3;
  signed int f345 : 3;
};

void test_4 (void)
{
  struct st4 s;
  s.f345 = -4;
  __analyzer_eval (s.f345 == -4); /* { dg-warning "TRUE" } */
  s.f345 = -3;
  __analyzer_eval (s.f345 == -3); /* { dg-warning "TRUE" } */
  s.f345 = -2;
  __analyzer_eval (s.f345 == -2); /* { dg-warning "TRUE" } */
  s.f345 = -1;
  __analyzer_eval (s.f345 == -1); /* { dg-warning "TRUE" } */
  s.f345 = 0;
  __analyzer_eval (s.f345 == 0); /* { dg-warning "TRUE" } */
  s.f345 = 1;
  __analyzer_eval (s.f345 == 1); /* { dg-warning "TRUE" } */
  s.f345 = 2;
  __analyzer_eval (s.f345 == 2); /* { dg-warning "TRUE" } */
  s.f345 = 3;
  __analyzer_eval (s.f345 == 3); /* { dg-warning "TRUE" } */
};

/* A zero bitfield to break up padding.  */

struct st5
{
  unsigned f0 : 5;
  unsigned :0;
  unsigned f1 : 16;
};

void test_5 (void)
{
  struct st5 s;
  s.f1 = 0xcafe;
  __analyzer_eval (s.f1 == 0xcafe); /* { dg-warning "TRUE" } */
}
