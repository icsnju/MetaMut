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

#ifdef SKIP_VA
const int test_va = 0;
#else
const int test_va = 1;
#endif

#define T(NAME, TYPE, INITVAL) 					\
TYPE g01##NAME, g02##NAME, g03##NAME, g04##NAME;		\
TYPE g05##NAME, g06##NAME, g07##NAME, g08##NAME;		\
TYPE g09##NAME, g10##NAME, g11##NAME, g12##NAME;		\
TYPE g13##NAME, g14##NAME, g15##NAME, g16##NAME;		\
								\
extern void init##NAME (TYPE *p, TYPE v);			\
extern void checkg##NAME (void);				\
extern TYPE test0##NAME (void);					\
extern TYPE test1##NAME (TYPE);					\
extern TYPE testva##NAME (int n, ...);				\
								\
void								\
check##NAME (TYPE x, TYPE v)					\
{								\
  if (x != v)							\
    DEBUG_CHECK							\
}								\
								\
void								\
testit##NAME (void)						\
{								\
  TYPE rslt;							\
  DEBUG_FPUTS (#NAME);						\
  DEBUG_FPUTS (" init: ");					\
  init##NAME (&g01##NAME,  1);					\
  init##NAME (&g02##NAME,  2);					\
  init##NAME (&g03##NAME,  3);					\
  init##NAME (&g04##NAME,  4);					\
  init##NAME (&g05##NAME,  5);					\
  init##NAME (&g06##NAME,  6);					\
  init##NAME (&g07##NAME,  7);					\
  init##NAME (&g08##NAME,  8);					\
  init##NAME (&g09##NAME,  9);					\
  init##NAME (&g10##NAME, 10);					\
  init##NAME (&g11##NAME, 11);					\
  init##NAME (&g12##NAME, 12);					\
  init##NAME (&g13##NAME, 13);					\
  init##NAME (&g14##NAME, 14);					\
  init##NAME (&g15##NAME, 15);					\
  init##NAME (&g16##NAME, 16);					\
  checkg##NAME ();						\
  DEBUG_NL;							\
  DEBUG_FPUTS (#NAME);						\
  DEBUG_FPUTS (" test0: ");					\
  rslt = test0##NAME ();					\
  check##NAME (rslt, g01##NAME);				\
  DEBUG_NL;							\
  DEBUG_FPUTS (#NAME);						\
  DEBUG_FPUTS (" test1: ");					\
  rslt = test1##NAME (g01##NAME);				\
  check##NAME (rslt, g01##NAME);				\
  if (test_va)							\
    {								\
      DEBUG_NL;							\
      DEBUG_FPUTS (#NAME);					\
      DEBUG_FPUTS (" testva: ");				\
      rslt = testva##NAME (1, g01##NAME);			\
      check##NAME (rslt, g01##NAME);				\
      rslt = testva##NAME (5, g01##NAME, g02##NAME, g03##NAME,	\
			   g04##NAME, g05##NAME);		\
      check##NAME (rslt, g05##NAME);				\
      rslt = testva##NAME (9, g01##NAME, g02##NAME, g03##NAME,	\
			   g04##NAME, g05##NAME, g06##NAME,	\
			   g07##NAME, g08##NAME, g09##NAME);	\
      check##NAME (rslt, g09##NAME);				\
      rslt = testva##NAME (16, g01##NAME, g02##NAME, g03##NAME,	\
			   g04##NAME, g05##NAME, g06##NAME,	\
			   g07##NAME, g08##NAME, g09##NAME,	\
			   g10##NAME, g11##NAME, g12##NAME,	\
			   g13##NAME, g14##NAME, g15##NAME,	\
			   g16##NAME);				\
      check##NAME (rslt, g16##NAME);				\
  }								\
  DEBUG_NL;							\
}

#ifndef SKIP_COMPLEX
#ifndef SKIP_COMPLEX_INT
T(cc, _Complex char, CINT (0, 1))
T(cs, _Complex short, CINT (1, 2))
#endif
T(cf, _Complex float, CDBL (1.0, 2.0))
#endif

#undef T

void
scalar_return_4_x ()
{
DEBUG_INIT

#define T(NAME) testit##NAME ();

#ifndef SKIP_COMPLEX
#ifndef SKIP_COMPLEX_INT
T(cc)
T(cs)
#endif
T(cf)
#endif

DEBUG_FINI

if (fails != 0)
  abort ();

#undef T
}
