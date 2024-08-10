/* { dg-options "-w -Wno-psabi -mno-mmx -msse2" } */

/* { dg-options "-w" } */
/* { dg-options "-w -mno-mmx -Wno-psabi" { target { i?86-*-* x86_64-*-* } } } */


#ifndef SKIP_ATTRIBUTE

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
/* This includes all of the vector modes that are recognized by
   c_common_type_for_mode.  */

typedef int __attribute__((mode(QI))) qi;
typedef int __attribute__((mode(HI))) hi;
typedef int __attribute__((mode(SI))) si;
typedef int __attribute__((mode(DI))) di;
typedef float __attribute__((mode(SF))) sf;
#if ((__SIZEOF_DOUBLE__ >= 8) || (__SIZEOF_LONG_DOUBLE__ >= 8))
typedef float __attribute__((mode(DF))) df;
#else
typedef float __attribute__((mode(SF))) df;
#endif

#if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 5)

typedef qi __attribute__((vector_size (8))) v8qi;
typedef qi __attribute__((vector_size (16))) v16qi;

typedef hi __attribute__((vector_size (4))) v2hi;
typedef hi __attribute__((vector_size (8))) v4hi;
typedef hi __attribute__((vector_size (16))) v8hi;

typedef si __attribute__((vector_size (8))) v2si;
typedef si __attribute__((vector_size (16))) v4si;

typedef di __attribute__((vector_size (8))) v1di;
typedef di __attribute__((vector_size (16))) v2di;

typedef sf __attribute__((vector_size (8))) v2sf;
typedef sf __attribute__((vector_size (16))) v4sf;
typedef sf __attribute__((vector_size (64))) v16sf;

typedef df __attribute__((vector_size (16))) v2df;

#else

typedef int __attribute__((mode(V8QI))) v8qi;
typedef int __attribute__((mode(V16QI))) v16qi;

typedef int __attribute__((mode(V2HI))) v2hi;
typedef int __attribute__((mode(V4HI))) v4hi;
typedef int __attribute__((mode(V8HI))) v8hi;

typedef int __attribute__((mode(V2SI))) v2si;
typedef int __attribute__((mode(V4SI))) v4si;

typedef int __attribute__((mode(V1DI))) v1di;
typedef int __attribute__((mode(V2DI))) v2di;

typedef float __attribute__((mode(V2SF))) v2sf;
typedef float __attribute__((mode(V4SF))) v4sf;
typedef float __attribute__((mode(V16SF))) v16sf;

typedef float __attribute__((mode(V2DF))) v2df;

#endif

typedef qi __attribute__((vector_size (32))) v32qi;
typedef hi __attribute__((vector_size (32))) v16hi;
typedef si __attribute__((vector_size (32))) v8si;
typedef di __attribute__((vector_size (32))) v4di;
typedef sf __attribute__((vector_size (32))) v8sf;
typedef df __attribute__((vector_size (32))) v4df;

typedef union U32QI { v32qi v; qi a[32]; } u32qi;
typedef union U16HI { v16hi v; hi a[16]; } u16hi;
typedef union U8SI { v8si v; si a[8]; } u8si;
typedef union U4DI { v4di v; di a[4]; } u4di;
typedef union U8SF { v8sf v; sf a[8]; } u8sf;
typedef union U4DF { v4df v; df a[4]; } u4df;

typedef union U8QI { v8qi v; qi a[8]; } u8qi;
typedef union U16QI { v16qi v; qi a[16]; } u16qi;

typedef union U2HI { v2hi v; hi a[2]; } u2hi;
typedef union U4HI { v4hi v; hi a[4]; } u4hi;
typedef union U8HI { v8hi v; hi a[8]; } u8hi;

typedef union U2SI { v2si v; si a[2]; } u2si;
typedef union U4SI { v4si v; si a[4]; } u4si;

typedef union U1DI { v1di v; di a[1]; } u1di;
typedef union U2DI { v2di v; di a[2]; } u2di;

typedef union U2SF { v2sf v; sf a[2]; } u2sf;
typedef union U4SF { v4sf v; sf a[4]; } u4sf;
typedef union U16SF { v16sf v; sf a[16]; } u16sf;

typedef union U2DF { v2df v; df a[2]; } u2df;
#define SETUP(NUM,TMODE)				\
v##NUM##TMODE g_v##NUM##TMODE;				\
TMODE g_##TMODE;					\
							\
extern void test_v##NUM##TMODE (void);			\
extern void checkp_##NUM##TMODE (TMODE *);		\
							\
void							\
pass_v##NUM##TMODE (v##NUM##TMODE v)			\
{							\
  u##NUM##TMODE u;					\
  int j;						\
  TMODE a[NUM];						\
  							\
  u.v = v;						\
  for (j = 0; j < NUM; j++)				\
    a[j] = u.a[j];					\
  checkp_##NUM##TMODE (a);				\
}							\
							\
v##NUM##TMODE						\
return_v##NUM##TMODE (void)				\
{							\
  return g_v##NUM##TMODE;				\
}

#define CHECK(NUM,TMODE)				\
  test_v##NUM##TMODE()

SETUP (2, sf);
SETUP (4, sf);
SETUP (16, sf);
SETUP (8, sf);
SETUP (2, df);
SETUP (4, df);

#endif

void
vector_2_x (void)
{
#ifndef SKIP_ATTRIBUTE
  DEBUG_INIT

  CHECK (2, sf);
  CHECK (4, sf);
  CHECK (8, sf);
  CHECK (16, sf);
  CHECK (4, df);
  CHECK (2, df);

  DEBUG_FINI

  if (fails != 0)
    abort ();
#endif
}
