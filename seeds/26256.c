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

/* Type definitions that are used by multiple tests.  */

typedef struct { float f; double d; } Sfd;
typedef struct { float f; long double l; } Sfl;
typedef struct { double d; float f; } Sdf;
typedef struct { double d; long double l; } Sdl;
typedef struct { long double l; float f; } Slf;
typedef struct { long double l; double d; } Sld;

typedef struct { float f; double d; long double l; } Sfdl;
typedef struct { float f; long double l; double d; } Sfld;
typedef struct { double d; float f; long double l; } Sdfl;
typedef struct { double d; long double l; float f; } Sdlf;
typedef struct { long double l; float f; double d; } Slfd;
typedef struct { long double l; double d; float f; } Sldf;
/* Function definitions that are used by multiple tests.  */

void checkSfd (Sfd x, double y)
{ if (x.f != y || x.d != y+1) DEBUG_CHECK }
void checkSfl (Sfl x, double y)
{ if (x.f != y || x.l != y+1) DEBUG_CHECK }
void checkSdf (Sdf x, double y)
{ if (x.d != y || x.f != y+1) DEBUG_CHECK }
void checkSdl (Sdl x, double y)
{ if (x.d != y || x.l != y+1) DEBUG_CHECK }
void checkSlf (Slf x, double y)
{ if (x.l != y || x.f != y+1) DEBUG_CHECK }
void checkSld (Sld x, double y)
{ if (x.l != y || x.d != y+1) DEBUG_CHECK }

void checkSfdl (Sfdl x, double y)
{ if (x.f != y || x.d != y+1 || x.l != y+2) DEBUG_CHECK }
void checkSfld (Sfld x, double y)
{ if (x.f != y || x.l != y+1 || x.d != y+2) DEBUG_CHECK }
void checkSdfl (Sdfl x, double y)
{ if (x.d != y || x.f != y+1 || x.l != y+2) DEBUG_CHECK }
void checkSdlf (Sdlf x, double y)
{ if (x.d != y || x.l != y+1 || x.f != y+2) DEBUG_CHECK }
void checkSlfd (Slfd x, double y)
{ if (x.l != y || x.f != y+1 || x.d != y+2) DEBUG_CHECK }
void checkSldf (Sldf x, double y)
{ if (x.l != y || x.d != y+1 || x.f != y+2) DEBUG_CHECK }

#define TEST(TYPE)						\
TYPE g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE;		\
TYPE g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE;		\
TYPE g9s##TYPE, g10s##TYPE, g11s##TYPE, g12s##TYPE;		\
TYPE g13s##TYPE, g14s##TYPE, g15s##TYPE, g16s##TYPE;		\
								\
extern void init##TYPE (TYPE *p, double x);			\
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
  init##TYPE  ( &g1s##TYPE,  (double)1);			\
  init##TYPE  ( &g2s##TYPE,  (double)2);			\
  init##TYPE  ( &g3s##TYPE,  (double)3);			\
  init##TYPE  ( &g4s##TYPE,  (double)4);			\
  init##TYPE  ( &g5s##TYPE,  (double)5);			\
  init##TYPE  ( &g6s##TYPE,  (double)6);			\
  init##TYPE  ( &g7s##TYPE,  (double)7);			\
  init##TYPE  ( &g8s##TYPE,  (double)8);			\
  init##TYPE  ( &g9s##TYPE,  (double)9);			\
  init##TYPE  (&g10s##TYPE, (double)10);			\
  init##TYPE  (&g11s##TYPE, (double)11);			\
  init##TYPE  (&g12s##TYPE, (double)12);			\
  init##TYPE  (&g13s##TYPE, (double)13);			\
  init##TYPE  (&g14s##TYPE, (double)14);			\
  init##TYPE  (&g15s##TYPE, (double)15);			\
  init##TYPE  (&g16s##TYPE, (double)16);			\
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
  DEBUG_FPUTS (" test2: ");					\
  test2_##TYPE (g1s##TYPE, g3s##TYPE, g5s##TYPE, g7s##TYPE,	\
		g9s##TYPE, g11s##TYPE, g13s##TYPE, g15s##TYPE);	\
  DEBUG_NL;							\
}

TEST(Sfd)
TEST(Sfl)
TEST(Sdf)
TEST(Sdl)
TEST(Slf)
TEST(Sld)
TEST(Sfdl)
TEST(Sfld)
TEST(Sdfl)
TEST(Sdlf)
TEST(Slfd)
TEST(Sldf)

#undef T

void
struct_by_value_10_x ()
{
DEBUG_INIT

#define T(TYPE) testit##TYPE ();

T(Sfd)
T(Sfl)
T(Sdf)
T(Sdl)
T(Slf)
T(Sld)
T(Sfdl)
T(Sfld)
T(Sdfl)
T(Sdlf)
T(Slfd)
T(Sldf)

DEBUG_FINI

if (fails != 0)
  abort ();

#undef T
}
