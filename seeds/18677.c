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

#ifdef SKIP_VA
const int test_va = 0;
#else
const int test_va = 1;
#endif

#ifndef SKIP_VLA_IN_STRUCT
#define T(NAME, FIELDS, TYPE, FIELDINIT, FIELDTEST)			\
void									\
testva##NAME (int n, ...)						\
{									\
  va_list ap;								\
  if (test_va)								\
    {									\
      struct S { FIELDS TYPE a[n]; } s;					\
      int fail = 0, i, j;						\
									\
      va_start (ap, n);							\
      for (j = 0; j < 2; ++j)						\
        {								\
	  s = va_arg (ap, struct S);					\
	  for (i = 0; i < n; ++i)					\
	    if (s.a[i] != 12 + n - i)					\
	      ++fail;							\
	  if (fail)							\
	    { DEBUG_FAIL; }						\
	  if (!j && va_arg (ap, int) != n)				\
	    { DEBUG_FAIL; }						\
	  FIELDTEST;							\
	}								\
      va_end (ap);							\
    }									\
}

#ifndef SKIP_VLA_IN_STRUCT

#ifndef T
#include "compat-common.h"
#include "mixed-struct-defs.h"
/* Function definitions that are used by multiple tests.  */

void initScd (Scd *p, int i)
{ p->c = (char)i; p->d = (double)i+1; }
void initScdc (Scdc *p, int i)
{ p->c = (char)i; p->d = (double)i+1; p->b = (char)i+2; }
void initSd (Sd *p, int i)
{ p->d = (double)i; }
void initSdi (Sdi *p, int i)
{ p->d = (double)i; p->i = i+1; }
void initScsds (Scsds *p, int i)
{ p->c = (char)i; p->sd.d = (double)i+1; }
void initScsdsc (Scsdsc *p, int i)
{ p->c = (char)i; p->sd.d = (double)i+1; p->b = (char)i+2; }
void initScsdis (Scsdis *p, int i)
{ p->c = (char)i; p->sdi.d = (double)i+1; p->sdi.i = i+2; }
void initScsdisc (Scsdisc *p, int i)
{ p->c = (char)i; p->sdi.d = (double)i+1; p->sdi.i = i+2; p->b = (char)i+3; }
void initSsds (Ssds *p, int i)
{ p->sd.d = (double)i; }
void initSsdsc (Ssdsc *p, int i)
{ p->sd.d = (double)i; p->c = (char)i+1; }
void initScssdss (Scssdss *p, int i)
{ p->c = (char)i; p->ssds.sd.d = (double)i+1; }
void initScssdssc (Scssdssc *p, int i)
{ p->c = (char)i; p->ssds.sd.d = (double)i+1; p->b = (char)i+2; }

void initSfi (Sfi *x, int i)
{ x->f = (float)i; x->i = i+1; }
void initSfii (Sfii *x, int i)
{ x->f = (float)i; x->i1 = i+1; x->i2 = i+2; }
void initSfifi (Sfifi *x, int i)
{ x->fi.f = (float)i; x->fi.i = i+1; }
void initSfiifii (Sfiifii *x, int i)
{ x->fii.f = (float)i; x->fii.i1 = i+1; x->fii.i2 = i+2; }

#define T(NAME, FIELDS, TYPE, FIELDINIT, FIELDTEST)			\
extern void testva##NAME (int n, ...);					\
									\
void									\
testit##NAME (int n)							\
{									\
  struct S { FIELDS TYPE a[n]; } s;					\
  int i;								\
  FIELDINIT;								\
  for (i = 0; i < n; ++i)						\
    s.a[i] = 12 + n - i;						\
  testva##NAME (n, s, n, s);						\
}

#include __FILE__

#undef T

void
struct_by_value_22_x ()
{
  int n;
DEBUG_INIT

#define T(NAME, FIELDS, TYPE, FIELDINIT, FIELDTEST) testit##NAME (n);

  for (n = 0; n < 16; ++n)
    {
#include __FILE__
      DEBUG_NL;
    }
  for (; n < 110; n += 13)
    {
#include __FILE__
      DEBUG_NL;
    }

DEBUG_FINI

if (fails != 0)
  abort ();
}

#else

#define S(NAME, FIELDS, FIELDINIT, FIELDTEST)				\
  T(c##NAME, FIELDS, char, FIELDINIT, FIELDTEST)			\
  T(s##NAME, FIELDS, short, FIELDINIT, FIELDTEST)			\
  T(u##NAME, FIELDS, unsigned, FIELDINIT, FIELDTEST)			\
  T(d##NAME, FIELDS, double, FIELDINIT, FIELDTEST)
S(E, , do {} while (0), DEBUG_DOT)
S(n, int n;, s.n = n, if (s.n != n) DEBUG_CHECK)
#define U(TYPE)								\
S(TYPE, TYPE s;, init##TYPE (&s.s, n), check##TYPE (s.s, n))
U(Scd)
U(Scdc)
U(Sd)
U(Sdi)
U(Scsds)
U(Scsdsc)
U(Scsdis)
U(Scsdisc)
U(Ssds)
U(Ssdsc)
U(Scssdss)
U(Scssdssc)
U(Sfi)
U(Sfii)
U(Sfifi)
U(Sfiifii)
#undef S
#undef U

#endif

#endif
#endif
