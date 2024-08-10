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

#define T(TYPE)							\
TYPE g01##TYPE, g02##TYPE, g03##TYPE, g04##TYPE;		\
TYPE g05##TYPE, g06##TYPE, g07##TYPE, g08##TYPE;		\
TYPE g09##TYPE, g10##TYPE, g11##TYPE, g12##TYPE;		\
TYPE g13##TYPE, g14##TYPE, g15##TYPE, g16##TYPE;		\
								\
extern void init##TYPE (TYPE *p, int i);			\
extern void checkg##TYPE (void);				\
extern TYPE test0##TYPE (void);					\
extern TYPE test1##TYPE (TYPE);					\
extern TYPE testva##TYPE (int n, ...);				\
								\
void								\
testit##TYPE (void)						\
{								\
  TYPE rslt;							\
  DEBUG_FPUTS (#TYPE);						\
  DEBUG_FPUTS (" init: ");					\
  init##TYPE  (&g01##TYPE,  1);					\
  init##TYPE  (&g02##TYPE,  2);					\
  init##TYPE  (&g03##TYPE,  3);					\
  init##TYPE  (&g04##TYPE,  4);					\
  init##TYPE  (&g05##TYPE,  5);					\
  init##TYPE  (&g06##TYPE,  6);					\
  init##TYPE  (&g07##TYPE,  7);					\
  init##TYPE  (&g08##TYPE,  8);					\
  init##TYPE  (&g09##TYPE,  9);					\
  init##TYPE  (&g10##TYPE, 10);					\
  init##TYPE  (&g11##TYPE, 11);					\
  init##TYPE  (&g12##TYPE, 12);					\
  init##TYPE  (&g13##TYPE, 13);					\
  init##TYPE  (&g14##TYPE, 14);					\
  init##TYPE  (&g15##TYPE, 15);					\
  init##TYPE  (&g16##TYPE, 16);					\
  checkg##TYPE ();						\
  DEBUG_NL;							\
  DEBUG_FPUTS (#TYPE);						\
  DEBUG_FPUTS (" test0: ");					\
  rslt = test0##TYPE ();					\
  check##TYPE (rslt, 1);					\
  DEBUG_NL;							\
  DEBUG_FPUTS (#TYPE);						\
  DEBUG_FPUTS (" test1: ");					\
  rslt = test1##TYPE (g01##TYPE);				\
  check##TYPE (rslt, 1);					\
  if (test_va)							\
    {								\
      DEBUG_NL;							\
      DEBUG_FPUTS (#TYPE);					\
      DEBUG_FPUTS (" testva: ");				\
      rslt = testva##TYPE (1, g01##TYPE);			\
      check##TYPE (rslt, 1);					\
      rslt = testva##TYPE (5, g01##TYPE, g02##TYPE,		\
			   g03##TYPE, g04##TYPE,		\
			   g05##TYPE);				\
      check##TYPE (rslt, 5);					\
      rslt = testva##TYPE (9, g01##TYPE, g02##TYPE,		\
			   g03##TYPE, g04##TYPE,		\
			   g05##TYPE, g06##TYPE,		\
			   g07##TYPE, g08##TYPE,		\
			   g09##TYPE);				\
      check##TYPE (rslt, 9);					\
      rslt = testva##TYPE (16, g01##TYPE, g02##TYPE,		\
			   g03##TYPE, g04##TYPE,		\
			   g05##TYPE, g06##TYPE,		\
			   g07##TYPE, g08##TYPE,		\
			   g09##TYPE, g10##TYPE,		\
			   g11##TYPE, g12##TYPE,		\
			   g13##TYPE, g14##TYPE,		\
			   g15##TYPE, g16##TYPE);		\
      check##TYPE (rslt, 16);					\
    }								\
  DEBUG_NL;							\
}

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

#define CHECK_CHAR(TYPE)		\
  void check##TYPE (TYPE p, int i)	\
   { if (p.c != (char)i) DEBUG_CHECK }

CHECK_CHAR(Ucs)
CHECK_CHAR(Uci)
CHECK_CHAR(Ucl)
CHECK_CHAR(Ucll)


#define CHECK_SHORT(TYPE)		\
  void check##TYPE (TYPE p, int i)	\
   { if (p.s != (short)i) DEBUG_CHECK }

CHECK_SHORT(Usi)
CHECK_SHORT(Usl)
CHECK_SHORT(Usll)


#define CHECK_INT(TYPE)			\
  void check##TYPE (TYPE p, int i)	\
   { if (p.i != i) DEBUG_CHECK }

CHECK_INT(Uil)
CHECK_INT(Uill)


#define CHECK_LONG(TYPE)		\
  void check##TYPE (TYPE p, int i)	\
   { if (p.l != (long)i) DEBUG_CHECK }

CHECK_LONG(Ulll)

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

#undef T

void
union_return_1_x ()
{
DEBUG_INIT

#define T(TYPE) testit##TYPE ();

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

DEBUG_FINI

if (fails != 0)
  abort ();

#undef T
}
