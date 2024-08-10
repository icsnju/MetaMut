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

#ifdef SKIP_VA
const int test_va = 0;
#else
const int test_va = 1;
#endif

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
  check##NAME (g01##NAME,  1);					\
  check##NAME (g02##NAME,  2);					\
  check##NAME (g03##NAME,  3);					\
  check##NAME (g04##NAME,  4);					\
  check##NAME (g05##NAME,  5);					\
  check##NAME (g06##NAME,  6);					\
  check##NAME (g07##NAME,  7);					\
  check##NAME (g08##NAME,  8);					\
  check##NAME (g09##NAME,  9);					\
  check##NAME (g10##NAME, 10);					\
  check##NAME (g11##NAME, 11);					\
  check##NAME (g12##NAME, 12);					\
  check##NAME (g13##NAME, 13);					\
  check##NAME (g14##NAME, 14);					\
  check##NAME (g15##NAME, 15);					\
  check##NAME (g16##NAME, 16);					\
}								\
								\
void								\
test##NAME (TYPE x01, TYPE x02, TYPE x03, TYPE x04,		\
            TYPE x05, TYPE x06, TYPE x07, TYPE x08,		\
            TYPE x09, TYPE x10, TYPE x11, TYPE x12,		\
            TYPE x13, TYPE x14, TYPE x15, TYPE x16)		\
{								\
  check##NAME (x01,  1);					\
  check##NAME (x02,  2);					\
  check##NAME (x03,  3);					\
  check##NAME (x04,  4);					\
  check##NAME (x05,  5);					\
  check##NAME (x06,  6);					\
  check##NAME (x07,  7);					\
  check##NAME (x08,  8);					\
  check##NAME (x09,  9);					\
  check##NAME (x10, 10);					\
  check##NAME (x11, 11);					\
  check##NAME (x12, 12);					\
  check##NAME (x13, 13);					\
  check##NAME (x14, 14);					\
  check##NAME (x15, 15);					\
  check##NAME (x16, 16);					\
}								\
								\
void								\
testva##NAME (int n, ...)					\
{								\
  int i;							\
  va_list ap;							\
  if (test_va)							\
    {								\
      va_start (ap, n);						\
      for (i = 0; i < n; i++)					\
	{							\
	  TYPE t = va_arg (ap, TYPE);				\
	  check##NAME (t, i+1);					\
	}							\
      va_end (ap);						\
    }								\
}

#ifndef SKIP_COMPLEX
#ifndef SKIP_COMPLEX_INT
T(cc, _Complex char, CINT (0, 1))
T(cs, _Complex short, CINT (1, 2))
#endif
T(cf, _Complex float, CDBL (6.0, 7.0))
#endif
