/* { dg-options "-Wno-psabi" { target { { i?86-*-* x86_64-*-* } && ilp32 } } } */

/*  Several of the binary compatibility tests use these macros to
    allow debugging the test or tracking down a failure by getting an
    indication of whether each individual check passed or failed.
    When DBG is defined, each check is shown by a dot (pass) or 'F'
    (fail) rather than aborting as soon as a failure is detected.  */
 
#ifdef DBG
#include <stdio.h>
#define DEBUG_INIT setbuf (stdout, NULL);
#define DEBUG_FPUTS(x) fputs (x, stdout)
#define DEBUG_DOT putc ('.', stdout)
#define DEBUG_NL putc ('\n', stdout)
#define DEBUG_FAIL putc ('F', stdout); fails++
#define DEBUG_CHECK { DEBUG_FAIL; } else { DEBUG_DOT; }
#define DEBUG_FINI if (fails) DEBUG_FPUTS ("failed\n"); \
		   else DEBUG_FPUTS ("passed\n");
#else
#define DEBUG_INIT
#define DEBUG_FPUTS(x)
#define DEBUG_DOT
#define DEBUG_NL
#define DEBUG_FAIL abort ()
#define DEBUG_CHECK abort ();
#define DEBUG_FINI
#endif

#ifdef SKIP_COMPLEX
#ifndef SKIP_COMPLEX_INT
#define SKIP_COMPLEX_INT
#endif
#endif

#ifndef SKIP_COMPLEX
#ifdef __GNUC__
#define CINT(x, y) (x + y * __extension__ 1i)
#define CDBL(x, y) (x + y * __extension__ 1.0i)
#else
#ifdef __SUNPRO_C
/* ??? Complex support without <complex.h>.  */
#else
#include <complex.h>
#endif
#ifndef SKIP_COMPLEX_INT
#define CINT(x, y) ((_Complex int) (x + y * _Complex_I))
#endif
#define CDBL(x, y) (x + y * _Complex_I)
#endif
#endif

#ifdef __cplusplus
extern "C" void abort (void);
#else
extern void abort (void);
#endif
extern int fails;
/* Define several variants of a struct for which the alignment differs
   between powerpc64-linux and powerpc64-aix.  This might be interesting
   for other targets as well.  */

#define DESC_orig "original"
struct B1_orig {
  char c;
  double d;
};

struct A2_orig {
  double d;
};

struct B2_orig {
  char c;
  struct A2_orig a2;
};

struct A3_orig {
  double d;
  int i;
};

struct B3_orig {
  char c;
  struct A3_orig a3;
};

#ifndef SKIP_ATTRIBUTE
#define DESC_p_all "packed attribute for all"
struct B1_p_all {
  char c;
  double d;
} __attribute__ ((packed));

struct A2_p_all {
  double d;
} __attribute__ ((packed));

struct B2_p_all {
  char c;
  struct A2_p_all a2;
} __attribute__ ((packed));

struct A3_p_all {
  double d;
  int i;
} __attribute__ ((packed));

struct B3_p_all {
  char c;
  struct A3_p_all a3;
} __attribute__ ((packed));

#define DESC_p_inner "packed attribute for inner"
struct B1_p_inner {
  char c;
  double d;
};

struct A2_p_inner {
  double d;
} __attribute__ ((packed));

struct B2_p_inner {
  char c;
  struct A2_p_inner a2;
};

struct A3_p_inner {
  double d;
  int i;
} __attribute__ ((packed));

struct B3_p_inner {
  char c;
  struct A3_p_inner a3;
};

#define DESC_p_outer "packed attribute for outer"
struct B1_p_outer {
  char c;
  double d;
} __attribute__ ((packed));

struct A2_p_outer {
  double d;
};

struct B2_p_outer {
  char c;
  struct A2_p_outer a2;
} __attribute__ ((packed));

struct A3_p_outer {
  double d;
  int i;
};

struct B3_p_outer {
  char c;
  struct A3_p_outer a3;
} __attribute__ ((packed));

#define DESC_a_max "maximum useful struct alignment for all"
struct B1_a_max {
  char c;
  double d;
} __attribute__ ((aligned));

struct A2_a_max {
  double d;
} __attribute__ ((aligned));

struct B2_a_max {
  char c;
  struct A2_a_max a2;
} __attribute__ ((aligned));

struct A3_a_max {
  double d;
  int i;
} __attribute__ ((aligned));

struct B3_a_max {
  char c;
  struct A3_a_max a3;
} __attribute__ ((aligned));

#define DESC_m_outer_p_inner "maximum alignment for outer, packed inner"
struct B1_m_outer_p_inner {
  char c;
  double d;
} __attribute__ ((aligned)) __attribute__ ((packed));

struct A2_m_outer_p_inner {
  double d;
} __attribute__ ((packed));

struct B2_m_outer_p_inner {
  char c;
  struct A2_m_outer_p_inner a2;
} __attribute__ ((aligned));

struct A3_m_outer_p_inner {
  double d;
  int i;
} __attribute__ ((packed));

struct B3_m_outer_p_inner {
  char c;
  struct A3_m_outer_p_inner a3;
} __attribute__ ((aligned));

#define DESC_m_inner_p_outer "maximum alignment for inner, packed outer"
struct B1_m_inner_p_outer {
  char c;
  double d;
} __attribute__ ((aligned)) __attribute__ ((packed));

struct A2_m_inner_p_outer {
  double d;
} __attribute__ ((aligned));

struct B2_m_inner_p_outer {
  char c;
  struct A2_m_inner_p_outer a2;
} __attribute__ ((packed));

struct A3_m_inner_p_outer {
  double d;
  int i;
} __attribute__ ((aligned));

struct B3_m_inner_p_outer {
  char c;
  struct A3_m_inner_p_outer a3;
} __attribute__ ((packed));
#endif

#define SETUP(NAME,V1,V2,V3)				\
char v1_##NAME = V1;					\
double v2_##NAME = V2;					\
int v3_##NAME = V3;					\
							\
struct B1_##NAME b1_##NAME = { V1, V2 };		\
struct B2_##NAME b2_##NAME = { V1, { V2 } };		\
struct B3_##NAME b3_##NAME = { V1, { V2, V3 } };	\
							\
struct B1_##NAME ab1_##NAME[2] =			\
  { { V1, V2 }, { V1, V2 } };				\
struct B2_##NAME ab2_##NAME[2] =			\
  { { V1, { V2 } }, { V1, { V2 } } };			\
struct B3_##NAME ab3_##NAME[2] =			\
  { { V1, { V2, V3 } }, { V1, { V2, V3 } } };		\
							\
extern void test_##NAME (void);				\
extern void checkp1_##NAME (struct B1_##NAME *);	\
extern void checkp2_##NAME (struct B2_##NAME *);	\
extern void checkp3_##NAME (struct B3_##NAME *);	\
extern void checkg1_##NAME (void);			\
extern void checkg2_##NAME (void);			\
extern void checkg3_##NAME (void);			\
							\
void							\
pass1_##NAME (struct B1_##NAME s)			\
{							\
  checkp1_##NAME (&s);					\
}							\
							\
void							\
pass2_##NAME (struct B2_##NAME s)			\
{							\
  checkp2_##NAME (&s);					\
}							\
							\
void							\
pass3_##NAME (struct B3_##NAME s)			\
{							\
  checkp3_##NAME (&s);					\
}							\
							\
struct B1_##NAME					\
return1_##NAME (void)					\
{							\
  return ab1_##NAME[0];					\
}							\
							\
struct B2_##NAME					\
return2_##NAME (void)					\
{							\
  return ab2_##NAME[0];					\
}							\
							\
struct B3_##NAME					\
return3_##NAME (void)					\
{							\
  return ab3_##NAME[0];					\
}

#define CHECK(NAME) test_##NAME()

#if __INT_MAX__ > 32767
SETUP (orig, 49, 1.0, 111111)
#else
SETUP (orig, 49, 1.0, 1111)
#endif
#ifndef SKIP_ATTRIBUTE
#if __INT_MAX__ > 32767
SETUP (p_all, 50, 2.0, 222222)
SETUP (p_inner, 51, 3.0, 333333)
SETUP (p_outer, 52, 4.0, 444444)
SETUP (a_max, 53, 5.0, 555555)
SETUP (m_outer_p_inner, 54, 6.0, 666666)
SETUP (m_inner_p_outer, 55, 7.0, 777777)
#else
SETUP (p_all, 50, 2.0, 2222)
SETUP (p_inner, 51, 3.0, 3333)
SETUP (p_outer, 52, 4.0, 4444)
SETUP (a_max, 53, 5.0, 5555)
SETUP (m_outer_p_inner, 54, 6.0, 6666)
SETUP (m_inner_p_outer, 55, 7.0, 7777)
#endif
#endif

void
struct_align_1_x (void)
{
  DEBUG_INIT

  CHECK (orig);
#ifndef SKIP_ATTRIBUTE
  CHECK (p_all);
  CHECK (p_inner);
  CHECK (p_outer);
  CHECK (a_max);
  CHECK (m_outer_p_inner);
  CHECK (m_inner_p_outer);
#endif

  DEBUG_FINI

  if (fails != 0)
    abort ();
}
