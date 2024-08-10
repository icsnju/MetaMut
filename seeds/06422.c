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
