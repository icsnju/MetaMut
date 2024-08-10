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

typedef struct { char c; double d; } Scd;
typedef struct { char c; double d; char b; } Scdc;
typedef struct { double d; } Sd;
typedef struct { double d; int i; } Sdi;
typedef struct { char c; Sd sd; } Scsds;
typedef struct { char c; Sd sd; char b; } Scsdsc;
typedef struct { char c; Sdi sdi; } Scsdis;
typedef struct { char c; Sdi sdi; char b; } Scsdisc;
typedef struct { Sd sd; } Ssds;
typedef struct { Sd sd; char c; } Ssdsc;
typedef struct { char c; Ssds ssds; } Scssdss;
typedef struct { char c; Ssds ssds; char b; } Scssdssc;

typedef struct { float f; int i; } Sfi;
typedef struct { float f; int i1; int i2; } Sfii;
typedef struct { float f; int i; Sfi fi; } Sfifi;
typedef struct { float f; int i1; int i2; Sfii fii; } Sfiifii;
/* Function definitions that are used by multiple tests.  */

void checkScd (Scd x, int i)
{ if (x.c != (char)i || x.d != (double)i+1) DEBUG_CHECK }
void checkScdc (Scdc x, int i)
{ if (x.c != (char)i || x.d != (double)i+1 || x.b != (char)i+2) DEBUG_CHECK }
void checkSd (Sd x, int i)
{ if (x.d != (double)i) DEBUG_CHECK }
void checkSdi (Sdi x, int i)
{ if (x.d != (double)i || x.i != i+1) DEBUG_CHECK }
void checkScsds (Scsds x, int i)
{ if (x.c != (char)i || x.sd.d != (double)i+1) DEBUG_CHECK }
void checkScsdsc (Scsdsc x, int i)
{ if (x.c != (char)i || x.sd.d != (double)i+1 || x.b != (char)i+2) DEBUG_CHECK }
void checkScsdis (Scsdis x, int i)
{ if (x.c != (char)i || x.sdi.d != (double)i+1 || x.sdi.i != i+2) DEBUG_CHECK }
void checkScsdisc (Scsdisc x, int i)
{ if (x.c != (char)i || x.sdi.d != (double)i+1 || x.sdi.i != i+2
    || x.b != (char)i+3) DEBUG_CHECK }
void checkSsds (Ssds x, int i)
{ if (x.sd.d != (double)i) DEBUG_CHECK }
void checkSsdsc (Ssdsc x, int i)
{ if (x.sd.d != (double)i || x.c != (char)i+1) DEBUG_CHECK }
void checkScssdss (Scssdss x, int i)
{ if (x.c != (char)i || x.ssds.sd.d != (double)i+1) DEBUG_CHECK }
void checkScssdssc (Scssdssc x, int i)
{ if (x.c != (char)i || x.ssds.sd.d != (double)i+1
    || x.b != (char)i+2) DEBUG_CHECK }

void checkSfi (Sfi x, int i)
{ if (x.f != (float)i || x.i != i+1) DEBUG_CHECK }
void checkSfii (Sfii x, int i)
{ if (x.f != (float)i || x.i1 != i+1 || x.i2 != i+2) DEBUG_CHECK }
void checkSfifi (Sfifi x, int i)
{ if (x.fi.f != (float)i || x.fi.i != i+1) DEBUG_CHECK }
void checkSfiifii (Sfiifii x, int i)
{ if (x.fii.f != (float)i || x.fii.i1 != i+1 || x.fii.i2 != i+2) DEBUG_CHECK }

T(Sfi)
T(Sfii)
T(Sfifi)
T(Sfiifii)

#undef T

void
struct_by_value_21_x ()
{
DEBUG_INIT

#define T(TYPE) testit##TYPE ();

T(Sfi)
T(Sfii)
T(Sfifi)
T(Sfiifii)

DEBUG_FINI

if (fails != 0)
  abort ();

#undef T
}
