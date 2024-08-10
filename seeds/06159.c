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
TYPE g01##TYPE, g02##TYPE, g03##TYPE, g04##TYPE;		\
TYPE g05##TYPE, g06##TYPE, g07##TYPE, g08##TYPE;		\
TYPE g09##TYPE, g10##TYPE, g11##TYPE, g12##TYPE;		\
TYPE g13##TYPE, g14##TYPE, g15##TYPE, g16##TYPE;		\
								\
extern void init##TYPE (TYPE *p, double y);			\
extern void checkg##TYPE (void);				\
extern TYPE test0##TYPE (void);					\
extern TYPE test1##TYPE (TYPE);					\
extern TYPE testva##TYPE (int n, ...);				\
								\
void								\
testit##TYPE (void)						\
{								\
  TYPE rslt;							\
  DEBUG_FPUTS (#TYPE);						\
  DEBUG_FPUTS (" init: ");					\
  init##TYPE  (&g01##TYPE,  1.0);				\
  init##TYPE  (&g02##TYPE,  2.0);				\
  init##TYPE  (&g03##TYPE,  3.0);				\
  init##TYPE  (&g04##TYPE,  4.0);				\
  init##TYPE  (&g05##TYPE,  5.0);				\
  init##TYPE  (&g06##TYPE,  6.0);				\
  init##TYPE  (&g07##TYPE,  7.0);				\
  init##TYPE  (&g08##TYPE,  8.0);				\
  init##TYPE  (&g09##TYPE,  9.0);				\
  init##TYPE  (&g10##TYPE, 10.0);				\
  init##TYPE  (&g11##TYPE, 11.0);				\
  init##TYPE  (&g12##TYPE, 12.0);				\
  init##TYPE  (&g13##TYPE, 13.0);				\
  init##TYPE  (&g14##TYPE, 14.0);				\
  init##TYPE  (&g15##TYPE, 15.0);				\
  init##TYPE  (&g16##TYPE, 16.0);				\
  checkg##TYPE ();						\
  DEBUG_NL;							\
  DEBUG_FPUTS (#TYPE);						\
  DEBUG_FPUTS (" test0: ");					\
  rslt = test0##TYPE ();					\
  check##TYPE (rslt, 1.0);					\
  DEBUG_NL;							\
  DEBUG_FPUTS (#TYPE);						\
  DEBUG_FPUTS (" test1: ");					\
  rslt = test1##TYPE (g01##TYPE);				\
  check##TYPE (rslt, 1.0);					\
  DEBUG_NL;							\
  DEBUG_FPUTS (#TYPE);						\
  DEBUG_FPUTS (" testva:");					\
  rslt = testva##TYPE (1, g01##TYPE);				\
  check##TYPE (rslt, 1.0);					\
  rslt = testva##TYPE (5, g01##TYPE, g02##TYPE,			\
			  g03##TYPE, g04##TYPE,			\
			  g05##TYPE);				\
  check##TYPE (rslt, 5.0);					\
  rslt = testva##TYPE (9, g01##TYPE, g02##TYPE,			\
			  g03##TYPE, g04##TYPE,			\
			  g05##TYPE, g06##TYPE,			\
			  g07##TYPE, g08##TYPE,			\
			  g09##TYPE);				\
  check##TYPE (rslt, 9.0);					\
  rslt = testva##TYPE (16, g01##TYPE, g02##TYPE,		\
			  g03##TYPE, g04##TYPE,			\
			  g05##TYPE, g06##TYPE,			\
			  g07##TYPE, g08##TYPE,			\
			  g09##TYPE, g10##TYPE,			\
			  g11##TYPE, g12##TYPE,			\
			  g13##TYPE, g14##TYPE,			\
			  g15##TYPE, g16##TYPE);		\
  check##TYPE (rslt, 16.0);					\
  DEBUG_NL;							\
}

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

#undef T

void
struct_return_10_x ()
{
DEBUG_INIT

#define T(TYPE) testit##TYPE ();

T(Sfd);
T(Sfl);
T(Sdf);
T(Sdl);
T(Slf);
T(Sld);
T(Sfdl);
T(Sfld);
T(Sdfl);
T(Sdlf);
T(Slfd);
T(Sldf);

DEBUG_FINI

if (fails != 0)
  return;

#undef T
}
