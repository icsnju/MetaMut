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

typedef struct { char c; } Sc;
typedef struct { short s; } Ss;
typedef struct { int i; } Si;
typedef struct { short s; char c; } Ssc;
typedef struct { char c; short s; } Scs;
typedef struct { int i; char c; } Sic;
typedef struct { char c; int i; } Sci;
typedef struct { short s; int i; } Ssi;
typedef struct { int i; short s; } Sis;
typedef struct { char c; short s; int i; } Scsi;
typedef struct { char c; int i; short s; } Scis;
typedef struct { short s; char c; int i; } Ssci;
typedef struct { short s; int i; char c; } Ssic;
typedef struct { int i; short s; char c; } Sisc;
typedef struct { int i; char c; short s; } Sics;
/* Function definitions that are used by multiple tests.  */

void initSc (Sc *p, int i) { p->c = (char)i; }
void initSs (Ss *p, int i) { p->s = i; }
void initSi (Si *p, int i) { p->i = i; }
void initSsc (Ssc *p, int i) { p->s = i; p->c = (char)i+1; }
void initScs (Scs *p, int i) { p->c = (char)i; p->s = i+1; }
void initSsi (Ssi *p, int i) { p->s = i; p->i = i+1; }
void initSis (Sis *p, int i) { p->i = i; p->s = i+1; }
void initSic (Sic *p, int i) { p->i = i; p->c = (char)i+1; }
void initSci (Sci *p, int i) { p->c = (char)i; p->i = i+1; }
void initScsi (Scsi *p, int i) { p->c = (char)i; p->s = i+1; p->i = i+2; }
void initScis (Scis *p, int i) { p->c = (char)i; p->i = i+1; p->s = i+2; }
void initSsci (Ssci *p, int i) { p->s = i; p->c = (char)i+1; p->i = i+2; }
void initSsic (Ssic *p, int i) { p->s = i; p->i = i+1; p->c = (char)i+2; }
void initSisc (Sisc *p, int i) { p->i = i; p->s = i+1; p->c = (char)i+2; }
void initSics (Sics *p, int i) { p->i = i; p->c = (char)i+1; p->s = i+2; }

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

T(Sc)
T(Ss)
T(Si)
T(Scs)
T(Ssc)
T(Sic)
T(Sci)
T(Ssi)
T(Sis)
T(Scsi)
T(Scis)
T(Ssci)
T(Ssic)
T(Sisc)
T(Sics)
