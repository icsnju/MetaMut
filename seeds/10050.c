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

typedef union { char c; short s; } Ucs;
typedef union { char c; int i; } Uci;
typedef union { char c; long l; } Ucl;
typedef union { char c; long long ll; } Ucll;

typedef union { short s; int i; } Usi;
typedef union { short s; long l; } Usl;
typedef union { short s; long long ll; } Usll;

typedef union { int i; long l; } Uil;
typedef union { int i; long long ll; } Uill;

typedef union { long l; long long ll; } Ulll;
/* Function definitions that are used by multiple tests.  */

#define INIT_CHAR(TYPE)			\
  void init##TYPE (TYPE *p, int i)	\
   { p->c = (char)i; }

INIT_CHAR(Ucs)
INIT_CHAR(Uci)
INIT_CHAR(Ucl)
INIT_CHAR(Ucll)


#define INIT_SHORT(TYPE)		\
  void init##TYPE (TYPE *p, int i)	\
   { p->s = (short)i; }

INIT_SHORT(Usi)
INIT_SHORT(Usl)
INIT_SHORT(Usll)


#define INIT_INT(TYPE)			\
  void init##TYPE (TYPE *p, int i)	\
   { p->i = i; }

INIT_INT(Uil)
INIT_INT(Uill)


#define INIT_LONG(TYPE)			\
  void init##TYPE (TYPE *p, int i)	\
   { p->l = (long)i; }

INIT_LONG(Ulll)

#define T(TYPE)							\
extern void check##TYPE (TYPE x, int i);			\
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

T(Ucs)
T(Uci)
T(Ucl)
T(Ucll)
T(Usi)
T(Usl)
T(Usll)
T(Uil)
T(Uill)
T(Ulll)
