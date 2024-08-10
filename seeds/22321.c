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

typedef void (*fpi)(int);
typedef void (*fpd)(double);

extern int f1_val;
extern void checki (int, int);

void
test1a (fpi f)
{
  (*f)(1);
}

void
test1b (fpi f, int i)
{
  (*f)(i);
}

void
test1c (double x, fpd f)
{
  (*f)(x);
}

void
test2a (fpi f1, fpd f2)
{
  (*f1)(10);
  (*f2)(10.0);
}

void
test2b (fpi f1, fpd f2, int i)
{
  (*f1)(i);
  (*f2)((double)i);
}

void
test2c (fpi f1, int i, fpd f2)
{
  (*f1)(i);
  (*f2)((double)i);
}

void
test2d (int i, fpi f1, fpd f2)
{
  (*f1)(i);
  (*f2)((double)i);
}

void
test2e (fpi f1, fpd f2, int i, double x)
{
  (*f1)(i);
  (*f2)(x);
}

void
test2f (fpi f1, int i, fpd f2, double x)
{
  (*f1)(i);
  (*f2)(x);
}

void
test2g (fpi f1, int i, double x, fpd f2)
{
  (*f1)(i);
  (*f2)(x);
}

void
test2h (double x, fpd f1, fpi f2, int i)
{
  (*f1)(x);
  (*f2)(i);
}

void
test2i (double x, fpd f1, int i, fpi f2)
{
  (*f1)(x);
  (*f2)(i);
}

void
test2j (int i, double x, fpi f1, fpd f2)
{
  (*f1)(i);
  (*f2)(x);
}

void
testva (int n, ...)
{
  int i;
  va_list ap;
  va_start (ap, n);
  for (i = 0; i < n; i++)
    {
      fpi fp = va_arg (ap, fpi);
      (*fp)(i);
      checki (f1_val, i);
    }
}
