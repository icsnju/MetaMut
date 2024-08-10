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

#define T(NAME, TYPE, INITVAL) 					\
TYPE g01##NAME, g02##NAME, g03##NAME, g04##NAME;		\
TYPE g05##NAME, g06##NAME, g07##NAME, g08##NAME;		\
TYPE g09##NAME, g10##NAME, g11##NAME, g12##NAME;		\
TYPE g13##NAME, g14##NAME, g15##NAME, g16##NAME;		\
								\
extern void init##NAME (TYPE *p, TYPE v);			\
extern void checkg##NAME (void);				\
extern void							\
test##NAME (TYPE x01, TYPE x02, TYPE x03, TYPE x04,		\
            TYPE x05, TYPE x06, TYPE x07, TYPE x08,		\
            TYPE x09, TYPE x10, TYPE x11, TYPE x12,		\
            TYPE x13, TYPE x14, TYPE x15, TYPE x16);		\
								\
void								\
check##NAME (TYPE x, TYPE v)					\
{								\
  if (x != v + INITVAL)						\
    DEBUG_CHECK							\
}								\
								\
void								\
test2_##NAME (TYPE x01, TYPE x02, TYPE x03, TYPE x04,		\
	      TYPE x05, TYPE x06, TYPE x07, TYPE x08)		\
{								\
  test##NAME (x01, g02##NAME, x02, g04##NAME,			\
	      x03, g06##NAME, x04, g08##NAME,			\
	      x05, g10##NAME, x06, g12##NAME,			\
	      x07, g14##NAME, x08, g16##NAME);			\
}								\
								\
void								\
testit##NAME (void)						\
{								\
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
  DEBUG_FPUTS (" test: ");					\
  test##NAME (g01##NAME, g02##NAME, g03##NAME, g04##NAME,	\
	      g05##NAME, g06##NAME, g07##NAME, g08##NAME,	\
	      g09##NAME, g10##NAME, g11##NAME, g12##NAME,	\
	      g13##NAME, g14##NAME, g15##NAME, g16##NAME);	\
  DEBUG_NL;							\
  DEBUG_FPUTS (#NAME);						\
  DEBUG_FPUTS (" test2: ");					\
  test2_##NAME (g01##NAME, g03##NAME, g05##NAME, g07##NAME,	\
		g09##NAME, g11##NAME, g13##NAME, g15##NAME);	\
  DEBUG_NL;							\
}

T(c, char, 21)
T(uc, unsigned char, 22)
T(sc, signed char, (-33))
T(us, unsigned short, 41)
T(ss, short, (-44))
T(f, float, 90.0)

#undef T

void
scalar_by_value_2_x ()
{
DEBUG_INIT

#define T(NAME) testit##NAME ();

T(c)
T(uc)
T(sc)
T(us)
T(ss)
T(f)

DEBUG_FINI

if (fails != 0)
  abort ();

#undef T
}
