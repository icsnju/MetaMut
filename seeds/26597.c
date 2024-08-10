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

#define T(TYPE)							\
extern TYPE g01##TYPE, g02##TYPE, g03##TYPE, g04##TYPE;		\
extern TYPE g05##TYPE, g06##TYPE, g07##TYPE, g08##TYPE;		\
extern TYPE g09##TYPE, g10##TYPE, g11##TYPE, g12##TYPE;		\
extern TYPE g13##TYPE, g14##TYPE, g15##TYPE, g16##TYPE;		\
								\
extern void check##TYPE (TYPE x, int i);			\
								\
void								\
checkg##TYPE (void)						\
{								\
  check##TYPE (g01##TYPE,  1);					\
  check##TYPE (g02##TYPE,  2);					\
  check##TYPE (g03##TYPE,  3);					\
  check##TYPE (g04##TYPE,  4);					\
  check##TYPE (g05##TYPE,  5);					\
  check##TYPE (g06##TYPE,  6);					\
  check##TYPE (g07##TYPE,  7);					\
  check##TYPE (g08##TYPE,  8);					\
  check##TYPE (g09##TYPE,  9);					\
  check##TYPE (g10##TYPE, 10);					\
  check##TYPE (g11##TYPE, 11);					\
  check##TYPE (g12##TYPE, 12);					\
  check##TYPE (g13##TYPE, 13);					\
  check##TYPE (g14##TYPE, 14);					\
  check##TYPE (g15##TYPE, 15);					\
  check##TYPE (g16##TYPE, 16);					\
}								\
								\
TYPE								\
test0##TYPE (void)						\
{								\
  return g01##TYPE;						\
}								\
								\
TYPE								\
test1##TYPE (TYPE x01)						\
{								\
  return x01;							\
}								\
								\
TYPE								\
testva##TYPE (int n, ...)					\
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

T(Sfi)
T(Sfii)
T(Sfifi)
T(Sfiifii)
