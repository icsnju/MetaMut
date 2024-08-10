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

#define T(TYPE)							\
TYPE g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE;		\
TYPE g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE;		\
TYPE g9s##TYPE, g10s##TYPE, g11s##TYPE, g12s##TYPE;		\
TYPE g13s##TYPE, g14s##TYPE, g15s##TYPE, g16s##TYPE;		\
								\
extern void init##TYPE (TYPE *p, int i);			\
extern void checkg##TYPE (void);				\
extern void							\
test##TYPE (TYPE s1, TYPE s2, TYPE s3, TYPE s4,			\
	    TYPE s5, TYPE s6, TYPE s7, TYPE s8,			\
	    TYPE s9, TYPE s10, TYPE s11, TYPE s12,		\
	    TYPE s13, TYPE s14, TYPE s15, TYPE s16);		\
extern void testva##TYPE (int n, ...);				\
								\
void								\
test2_##TYPE (TYPE s1, TYPE s2, TYPE s3, TYPE s4,		\
	      TYPE s5, TYPE s6, TYPE s7, TYPE s8)		\
{								\
  test##TYPE (s1, g2s##TYPE, s2, g4s##TYPE,			\
	      s3, g6s##TYPE, s4, g8s##TYPE,			\
	      s5, g10s##TYPE, s6, g12s##TYPE,			\
	      s7, g14s##TYPE, s8, g16s##TYPE);			\
}								\
								\
void								\
testit##TYPE (void)						\
{								\
  DEBUG_FPUTS (#TYPE);						\
  DEBUG_FPUTS (" init: ");					\
  init##TYPE  ( &g1s##TYPE,  1);				\
  init##TYPE  ( &g2s##TYPE,  2);				\
  init##TYPE  ( &g3s##TYPE,  3);				\
  init##TYPE  ( &g4s##TYPE,  4);				\
  init##TYPE  ( &g5s##TYPE,  5);				\
  init##TYPE  ( &g6s##TYPE,  6);				\
  init##TYPE  ( &g7s##TYPE,  7);				\
  init##TYPE  ( &g8s##TYPE,  8);				\
  init##TYPE  ( &g9s##TYPE,  9);				\
  init##TYPE  (&g10s##TYPE, 10);				\
  init##TYPE  (&g11s##TYPE, 11);				\
  init##TYPE  (&g12s##TYPE, 12);				\
  init##TYPE  (&g13s##TYPE, 13);				\
  init##TYPE  (&g14s##TYPE, 14);				\
  init##TYPE  (&g15s##TYPE, 15);				\
  init##TYPE  (&g16s##TYPE, 16);				\
  checkg##TYPE ();						\
  DEBUG_NL;							\
  DEBUG_FPUTS (#TYPE);						\
  DEBUG_FPUTS (" test: ");					\
  test##TYPE (g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
	      g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE,	\
	      g9s##TYPE, g10s##TYPE, g11s##TYPE, g12s##TYPE,	\
	      g13s##TYPE, g14s##TYPE, g15s##TYPE, g16s##TYPE);	\
  DEBUG_NL;							\
  DEBUG_FPUTS (#TYPE);						\
  DEBUG_FPUTS (" testva:");					\
  DEBUG_NL;							\
  testva##TYPE (1,						\
		g1s##TYPE);					\
  DEBUG_NL;							\
  testva##TYPE (2,						\
		g1s##TYPE, g2s##TYPE);				\
  DEBUG_NL;							\
  testva##TYPE (3,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE);		\
  DEBUG_NL;							\
  testva##TYPE (4,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE);	\
  DEBUG_NL;							\
  testva##TYPE (5,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
		g5s##TYPE);					\
  DEBUG_NL;							\
  testva##TYPE (6,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
		g5s##TYPE, g6s##TYPE);				\
  DEBUG_NL;							\
  testva##TYPE (7,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
		g5s##TYPE, g6s##TYPE, g7s##TYPE);		\
  DEBUG_NL;							\
  testva##TYPE (8,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
		g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE);	\
  DEBUG_NL;							\
  testva##TYPE (9,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
		g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE,	\
		g9s##TYPE);					\
  DEBUG_NL;							\
  testva##TYPE (10,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
		g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE,	\
		g9s##TYPE, g10s##TYPE);				\
  DEBUG_NL;							\
  testva##TYPE (11,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
		g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE,	\
		g9s##TYPE, g10s##TYPE, g11s##TYPE);		\
  DEBUG_NL;							\
  testva##TYPE (12,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
		g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE,	\
		g9s##TYPE, g10s##TYPE, g11s##TYPE, g12s##TYPE);	\
  DEBUG_NL;							\
  testva##TYPE (13,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
		g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE,	\
		g9s##TYPE, g10s##TYPE, g11s##TYPE, g12s##TYPE,	\
		g13s##TYPE);					\
  DEBUG_NL;							\
  testva##TYPE (14,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
		g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE,	\
		g9s##TYPE, g10s##TYPE, g11s##TYPE, g12s##TYPE,	\
		g13s##TYPE, g14s##TYPE);			\
  DEBUG_NL;							\
  testva##TYPE (15,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
		g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE,	\
		g9s##TYPE, g10s##TYPE, g11s##TYPE, g12s##TYPE,	\
		g13s##TYPE, g14s##TYPE, g15s##TYPE);		\
  DEBUG_NL;							\
  testva##TYPE (16,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
		g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE,	\
		g9s##TYPE, g10s##TYPE, g11s##TYPE, g12s##TYPE,	\
		g13s##TYPE, g14s##TYPE, g15s##TYPE, g16s##TYPE); \
  DEBUG_NL;							\
  DEBUG_FPUTS (#TYPE);						\
  DEBUG_FPUTS (" test2:");					\
  test2_##TYPE (g1s##TYPE, g3s##TYPE, g5s##TYPE, g7s##TYPE,	\
		g9s##TYPE, g11s##TYPE, g13s##TYPE, g15s##TYPE);	\
  DEBUG_NL;							\
}

/* Type definitions that are used by multiple tests.  */

typedef union { char c; short s; } Ucs;
typedef union { char c; int i; } Uci;
typedef union { char c; long l; } Ucl;
typedef union { char c; long long ll; } Ucll;

typedef union { short s; int i; } Usi;
typedef union { short s; long l; } Usl;
typedef union { short s; long long ll; } Usll;

typedef union { int i; long l; } Uil;
typedef union { int i; long long ll; } Uill;

typedef union { long l; long long ll; } Ulll;
/* Function definitions that are used by multiple tests.  */

#define CHECK_CHAR(TYPE)		\
  void check##TYPE (TYPE p, int i)	\
   { if (p.c != (char)i) DEBUG_CHECK }

CHECK_CHAR(Ucs)
CHECK_CHAR(Uci)
CHECK_CHAR(Ucl)
CHECK_CHAR(Ucll)


#define CHECK_SHORT(TYPE)		\
  void check##TYPE (TYPE p, int i)	\
   { if (p.s != (short)i) DEBUG_CHECK }

CHECK_SHORT(Usi)
CHECK_SHORT(Usl)
CHECK_SHORT(Usll)


#define CHECK_INT(TYPE)			\
  void check##TYPE (TYPE p, int i)	\
   { if (p.i != i) DEBUG_CHECK }

CHECK_INT(Uil)
CHECK_INT(Uill)


#define CHECK_LONG(TYPE)		\
  void check##TYPE (TYPE p, int i)	\
   { if (p.l != (long)i) DEBUG_CHECK }

CHECK_LONG(Ulll)

T(Ucs)
T(Uci)
T(Ucl)
T(Ucll)
T(Usi)
T(Usl)
T(Usll)
T(Uil)
T(Uill)
T(Ulll)

#undef T

void
union_by_value_1_x ()
{
DEBUG_INIT

#define T(TYPE) testit##TYPE ();

T(Ucs)
T(Uci)
T(Ucl)
T(Ucll)
T(Usi)
T(Usl)
T(Usll)
T(Uil)
T(Uill)
T(Ulll)

DEBUG_FINI

if (fails != 0)
  abort ();

#undef T
}
