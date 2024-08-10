#include <stdarg.h>

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

#define T(NAME, TYPE, INITVAL)					\
extern TYPE g01##NAME, g02##NAME, g03##NAME, g04##NAME;		\
extern TYPE g05##NAME, g06##NAME, g07##NAME, g08##NAME;		\
extern TYPE g09##NAME, g10##NAME, g11##NAME, g12##NAME;		\
extern TYPE g13##NAME, g14##NAME, g15##NAME, g16##NAME;		\
								\
extern void check##NAME (TYPE x, TYPE v);			\
								\
void								\
init##NAME (TYPE *p, TYPE v)					\
{								\
  *p = v + INITVAL;						\
}								\
								\
void								\
checkg##NAME (void)						\
{								\
  check##NAME (g01##NAME,  1+INITVAL);				\
  check##NAME (g02##NAME,  2+INITVAL);				\
  check##NAME (g03##NAME,  3+INITVAL);				\
  check##NAME (g04##NAME,  4+INITVAL);				\
  check##NAME (g05##NAME,  5+INITVAL);				\
  check##NAME (g06##NAME,  6+INITVAL);				\
  check##NAME (g07##NAME,  7+INITVAL);				\
  check##NAME (g08##NAME,  8+INITVAL);				\
  check##NAME (g09##NAME,  9+INITVAL);				\
  check##NAME (g10##NAME, 10+INITVAL);				\
  check##NAME (g11##NAME, 11+INITVAL);				\
  check##NAME (g12##NAME, 12+INITVAL);				\
  check##NAME (g13##NAME, 13+INITVAL);				\
  check##NAME (g14##NAME, 14+INITVAL);				\
  check##NAME (g15##NAME, 15+INITVAL);				\
  check##NAME (g16##NAME, 16+INITVAL);				\
}								\
								\
TYPE								\
test0##NAME (void)						\
{								\
  return g01##NAME;						\
}								\
								\
TYPE								\
test1##NAME (TYPE x01)						\
{								\
  return x01;							\
}								\
								\
TYPE								\
testva##NAME (int n, ...)					\
{								\
  int i;							\
  TYPE rslt;							\
  va_list ap;							\
  va_start (ap, n);						\
  for (i = 0; i < n; i++)					\
    rslt = va_arg (ap, TYPE);					\
  va_end (ap);							\
  return rslt;							\
}

#ifndef SKIP_COMPLEX
#ifndef SKIP_COMPLEX_INT
T(cc, _Complex char, CINT (0, 1))
T(cs, _Complex short, CINT (1, 2))
#endif
T(cf, _Complex float, CDBL (1.0, 2.0))
#endif
