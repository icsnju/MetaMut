#include <string.h>
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

extern void check_init_char (char v);
extern void check_init_int (int v);

void test_1 (void)
{
  union
  {
    char c[16];
    int  i[4];
  } v;
  memset (&v, 0, sizeof (v));
  v.c[5] = 42;
  check_init_int (v.c[0]);
  check_init_int (v.c[4]);
  check_init_int (v.c[6]);
  check_init_int (v.i[1]);
}

void test_2 (void)
{
  /* Intersection of byte ranges within "v".  */
  union
  {
    struct {
      int  a;
      char b;
      char c;
    } __attribute__((packed)) icc;
    struct {
      char a;
      int  b;
      char c;
    } __attribute__((packed)) cic;
    struct {
      char a;
      char b;
      int  c;
    } __attribute__((packed)) cci;
  } v;

  v.icc.a = 1066;
  v.icc.b = 42;
  v.icc.c = 17;

  __analyzer_eval (v.icc.a == 1066); /* { dg-warning "TRUE" } */
  __analyzer_eval (v.icc.b == 42); /* { dg-warning "TRUE" } */
  __analyzer_eval (v.icc.c == 17); /* { dg-warning "TRUE" } */
  check_init_int (v.icc.a);
  check_init_char (v.icc.b);
  check_init_char (v.icc.c);
  
  check_init_char (v.cic.a);
  check_init_int (v.cic.b);
  check_init_char (v.cic.c);
  
  check_init_char (v.cci.a);
  check_init_char (v.cci.b);
  check_init_int (v.cci.c);

  v.cic.a = 42;
  v.cic.b = 1066;
  v.cic.c = 17;

  __analyzer_eval (v.cic.a == 42); /* { dg-warning "TRUE" } */
  __analyzer_eval (v.cic.b == 1066); /* { dg-warning "TRUE" } */
  __analyzer_eval (v.cic.c == 17); /* { dg-warning "TRUE" } */
  check_init_int (v.icc.a);
  check_init_char (v.icc.b);
  check_init_char (v.icc.c);
  
  check_init_char (v.cic.a);
  check_init_int (v.cic.b);
  check_init_char (v.cic.c);
  
  check_init_char (v.cci.a);
  check_init_char (v.cci.b);
  check_init_int (v.cci.c);  

  v.cci.a = 42;
  v.cci.b = 17;
  v.cci.c = 1066;

  __analyzer_eval (v.cci.a == 42); /* { dg-warning "TRUE" } */
  __analyzer_eval (v.cci.b == 17); /* { dg-warning "TRUE" } */
  __analyzer_eval (v.cci.c == 1066); /* { dg-warning "TRUE" } */
  check_init_int (v.icc.a);
  check_init_char (v.icc.b);
  check_init_char (v.icc.c);
  
  check_init_char (v.cic.a);
  check_init_int (v.cic.b);
  check_init_char (v.cic.c);
  
  check_init_char (v.cci.a);
  check_init_char (v.cci.b);
  check_init_int (v.cci.c);  

}
