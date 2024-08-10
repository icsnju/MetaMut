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

extern void checkSc (Sc x, int i);
extern void checkSs (Ss x, int i);
extern void checkSi (Si x, int i);
extern void checkSsc (Ssc x, int i);
extern void checkScs (Scs x, int i);
extern void checkSsi (Ssi x, int i);
extern void checkSis (Sis x, int i);
extern void checkSic (Sic x, int i);
extern void checkSci (Sci x, int i);
extern void checkScsi (Scsi x, int i);
extern void checkScis (Scis x, int i);
extern void checkSsci (Ssci x, int i);
extern void checkSsic (Ssic x, int i);
extern void checkSisc (Sisc x, int i);
extern void checkSics (Sics x, int i);

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
extern TYPE g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE;		\
extern TYPE g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE;		\
extern TYPE g9s##TYPE, g10s##TYPE, g11s##TYPE, g12s##TYPE;	\
extern TYPE g13s##TYPE, g14s##TYPE, g15s##TYPE, g16s##TYPE;	\
								\
void								\
checkg##TYPE (void)						\
{								\
  check##TYPE (  g1s##TYPE,  1);				\
  check##TYPE (  g2s##TYPE,  2);				\
  check##TYPE (  g3s##TYPE,  3);				\
  check##TYPE (  g4s##TYPE,  4);				\
  check##TYPE (  g5s##TYPE,  5);				\
  check##TYPE (  g6s##TYPE,  6);				\
  check##TYPE (  g7s##TYPE,  7);				\
  check##TYPE (  g8s##TYPE,  8);				\
  check##TYPE (  g9s##TYPE,  9);				\
  check##TYPE ( g10s##TYPE, 10);				\
  check##TYPE ( g11s##TYPE, 11);				\
  check##TYPE ( g12s##TYPE, 12);				\
  check##TYPE ( g13s##TYPE, 13);				\
  check##TYPE ( g14s##TYPE, 14);				\
  check##TYPE ( g15s##TYPE, 15);				\
  check##TYPE ( g16s##TYPE, 16);				\
}								\
								\
void								\
test##TYPE (TYPE s1, TYPE s2, TYPE s3, TYPE s4,			\
	    TYPE s5, TYPE s6, TYPE s7, TYPE s8,			\
	    TYPE s9, TYPE s10, TYPE s11, TYPE s12,		\
	    TYPE s13, TYPE s14, TYPE s15, TYPE s16)		\
{								\
  check##TYPE (s1, 1);						\
  check##TYPE (s2, 2);						\
  check##TYPE (s3, 3);						\
  check##TYPE (s4, 4);						\
  check##TYPE (s5, 5);						\
  check##TYPE (s6, 6);						\
  check##TYPE (s7, 7);						\
  check##TYPE (s8, 8);						\
  check##TYPE (s9, 9);						\
  check##TYPE (s10, 10);					\
  check##TYPE (s11, 11);					\
  check##TYPE (s12, 12);					\
  check##TYPE (s13, 13);					\
  check##TYPE (s14, 14);					\
  check##TYPE (s15, 15);					\
  check##TYPE (s16, 16);					\
}								\
								\
void								\
testva##TYPE (int n, ...)					\
{								\
  int i;							\
  va_list ap;							\
  if (test_va)							\
    {								\
      va_start (ap, n);						\
      for (i = 0; i < n; i++)					\
	{							\
	  TYPE t = va_arg (ap, TYPE);				\
	  check##TYPE (t, i+1);					\
	}							\
      va_end (ap);						\
    }								\
}

T(Sc)
T(Ss)
T(Si)
T(Ssc)
T(Scs)
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
