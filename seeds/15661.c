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

#define TEST(NAME)						\
extern char v1_##NAME;						\
extern double v2_##NAME;					\
extern int v3_##NAME;						\
								\
extern struct B1_##NAME b1_##NAME, ab1_##NAME[2];		\
extern struct B2_##NAME b2_##NAME, ab2_##NAME[2];		\
extern struct B3_##NAME b3_##NAME, ab3_##NAME[2];		\
								\
extern void pass1_##NAME (struct B1_##NAME);			\
extern void pass2_##NAME (struct B2_##NAME);			\
extern void pass3_##NAME (struct B3_##NAME);			\
extern struct B1_##NAME return1_##NAME (void);			\
extern struct B2_##NAME return2_##NAME (void);			\
extern struct B3_##NAME return3_##NAME (void);			\
								\
void								\
checkp1_##NAME (struct B1_##NAME *p)				\
{								\
  if (p->c != v1_##NAME)					\
    DEBUG_CHECK;						\
  if (p->d != v2_##NAME)					\
    DEBUG_CHECK;						\
}								\
								\
void								\
checkp2_##NAME (struct B2_##NAME *p)				\
{								\
  if (p->c != v1_##NAME)					\
    DEBUG_CHECK;						\
  if (p->a2.d != v2_##NAME)					\
    DEBUG_CHECK;						\
}								\
								\
void								\
checkp3_##NAME (struct B3_##NAME *p)				\
{								\
  if (p->c != v1_##NAME)					\
    DEBUG_CHECK;						\
  if (p->a3.d != v2_##NAME)					\
    DEBUG_CHECK;						\
  if (p->a3.i != v3_##NAME)					\
    DEBUG_CHECK;						\
}								\
								\
void								\
test_##NAME (void)						\
{								\
  struct B1_##NAME s1;						\
  struct B2_##NAME s2;						\
  struct B3_##NAME s3;						\
  DEBUG_FPUTS (DESC_##NAME);					\
  DEBUG_NL;							\
  DEBUG_FPUTS ("  global variable");				\
  checkp1_##NAME (&b1_##NAME);					\
  checkp2_##NAME (&b2_##NAME);					\
  checkp3_##NAME (&b3_##NAME);					\
  DEBUG_NL;							\
  DEBUG_FPUTS ("  global array");				\
  checkp1_##NAME (&ab1_##NAME[1]);				\
  checkp2_##NAME (&ab2_##NAME[1]);				\
  checkp3_##NAME (&ab3_##NAME[1]);				\
  DEBUG_NL;							\
  DEBUG_FPUTS ("  argument");					\
  pass1_##NAME (b1_##NAME);					\
  pass2_##NAME (b2_##NAME);					\
  pass3_##NAME (b3_##NAME);					\
  DEBUG_NL;							\
  DEBUG_FPUTS ("  function result");				\
  s1 = return1_##NAME ();					\
  checkp1_##NAME (&s1);						\
  s2 = return2_##NAME ();					\
  checkp2_##NAME (&s2);						\
  s3 = return3_##NAME ();					\
  checkp3_##NAME (&s3);						\
  DEBUG_NL;							\
}

TEST (orig)
#ifndef SKIP_ATTRIBUTE
TEST (p_all)
TEST (p_inner)
TEST (p_outer)
TEST (a_max)
TEST (m_outer_p_inner)
TEST (m_inner_p_outer)
#endif
