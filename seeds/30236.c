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

#define DEFS(NAME,TYPEM)					\
typedef struct { TYPEM a; } S##NAME##1;				\
typedef struct { TYPEM a; TYPEM b; } S##NAME##2;		\
typedef struct { TYPEM a; TYPEM b; TYPEM c; } S##NAME##3;	\
typedef struct { TYPEM a; TYPEM b; TYPEM c; TYPEM d; }		\
	       S##NAME##4;					\
typedef struct { TYPEM a; TYPEM b; TYPEM c; TYPEM d; TYPEM e; }	\
	       S##NAME##5;					\
typedef struct { TYPEM a; TYPEM b; TYPEM c; TYPEM d; TYPEM e;	\
  		 TYPEM f; } S##NAME##6;				\
typedef struct { TYPEM a; TYPEM b; TYPEM c; TYPEM d; TYPEM e;	\
		 TYPEM f; TYPEM g; } S##NAME##7;		\
typedef struct { TYPEM a; TYPEM b; TYPEM c; TYPEM d; TYPEM e;	\
		 TYPEM f; TYPEM g; TYPEM h; } S##NAME##8;	\
typedef struct { TYPEM a; TYPEM b; TYPEM c; TYPEM d; TYPEM e;	\
		 TYPEM f; TYPEM g; TYPEM h; TYPEM i; }		\
	       S##NAME##9;					\
typedef struct { TYPEM a; TYPEM b; TYPEM c; TYPEM d; TYPEM e;	\
		 TYPEM f; TYPEM g; TYPEM h; TYPEM i; TYPEM j; }	\
	       S##NAME##10;					\
typedef struct { TYPEM a; TYPEM b; TYPEM c; TYPEM d; TYPEM e;	\
		 TYPEM f; TYPEM g; TYPEM h; TYPEM i; TYPEM j;	\
		 TYPEM k; } S##NAME##11;			\
typedef struct { TYPEM a; TYPEM b; TYPEM c; TYPEM d; TYPEM e;	\
		 TYPEM f; TYPEM g; TYPEM h; TYPEM i; TYPEM j;	\
		 TYPEM k; TYPEM l; } S##NAME##12;		\
typedef struct { TYPEM a; TYPEM b; TYPEM c; TYPEM d; TYPEM e;	\
		 TYPEM f; TYPEM g; TYPEM h; TYPEM i; TYPEM j;	\
		 TYPEM k; TYPEM l; TYPEM m; } S##NAME##13;	\
typedef struct { TYPEM a; TYPEM b; TYPEM c; TYPEM d; TYPEM e;	\
		 TYPEM f; TYPEM g; TYPEM h; TYPEM i; TYPEM j;	\
		 TYPEM k; TYPEM l; TYPEM m; TYPEM n; }		\
	        S##NAME##14;					\
typedef struct { TYPEM a; TYPEM b; TYPEM c; TYPEM d; TYPEM e;	\
		 TYPEM f; TYPEM g; TYPEM h; TYPEM i; TYPEM j;	\
		 TYPEM k; TYPEM l; TYPEM m; TYPEM n; TYPEM o; }	\
	       S##NAME##15;					\
typedef struct { TYPEM a; TYPEM b; TYPEM c; TYPEM d; TYPEM e;	\
		 TYPEM f; TYPEM g; TYPEM h; TYPEM i; TYPEM j;	\
		 TYPEM k; TYPEM l; TYPEM m; TYPEM n; TYPEM o;	\
		 TYPEM p; } S##NAME##16;
/* Function definitions that are used by multiple tests.  */

#define CHECKS(NAME,TYPEM)					\
void checkS##NAME##1 (S##NAME##1 x, TYPEM y)			\
{ if (x.a != y) DEBUG_CHECK }					\
void checkS##NAME##2 (S##NAME##2 x, TYPEM y)			\
{ if (x.a != y || x.b != y+1 ) DEBUG_CHECK }			\
void checkS##NAME##3 (S##NAME##3 x, TYPEM y)			\
{ if (x.a != y || x.b != y+1 || x.c != y+2 )			\
    DEBUG_CHECK }						\
void checkS##NAME##4 (S##NAME##4 x, TYPEM y)			\
{ if (x.a != y || x.b != y+1 || x.c != y+2 || x.d != y+3)	\
    DEBUG_CHECK }						\
void checkS##NAME##5 (S##NAME##5 x, TYPEM y)			\
{ if (x.a != y || x.b != y+1 || x.c != y+2 || x.d != y+3	\
      || x.e != y+4) DEBUG_CHECK }				\
void checkS##NAME##6 (S##NAME##6 x, TYPEM y)			\
{ if (x.a != y || x.b != y+1 || x.c != y+2 || x.d != y+3	\
      || x.e != y+4 || x.f != y+5) DEBUG_CHECK }		\
void checkS##NAME##7 (S##NAME##7 x, TYPEM y)			\
{ if (x.a != y || x.b != y+1 || x.c != y+2 || x.d != y+3	\
      || x.e != y+4 || x.f != y+5 || x.g != y+6)		\
    DEBUG_CHECK }						\
void checkS##NAME##8 (S##NAME##8 x, TYPEM y)			\
{ if (x.a != y || x.b != y+1 || x.c != y+2 || x.d != y+3	\
      || x.e != y+4 || x.f != y+5 || x.g != y+6			\
      || x.h != y+7) DEBUG_CHECK }				\
void checkS##NAME##9 (S##NAME##9 x, TYPEM y)			\
{ if (x.a != y || x.b != y+1 || x.c != y+2 || x.d != y+3	\
      || x.e != y+4 || x.f != y+5 || x.g != y+6			\
      || x.h != y+7 || x.i != y+8) DEBUG_CHECK }		\
void checkS##NAME##10 (S##NAME##10 x, TYPEM y)			\
{ if (x.a != y || x.b != y+1 || x.c != y+2 || x.d != y+3	\
      || x.e != y+4 || x.f != y+5 || x.g != y+6			\
      || x.h != y+7 || x.i != y+8 || x.j != y+9)		\
   DEBUG_CHECK }						\
void checkS##NAME##11 (S##NAME##11 x, TYPEM y)			\
{ if (x.a != y || x.b != y+1 || x.c != y+2 || x.d != y+3	\
      || x.e != y+4 || x.f != y+5 || x.g != y+6			\
      || x.h != y+7 || x.i != y+8 || x.j != y+9			\
      || x.k != y+10) DEBUG_CHECK }				\
void checkS##NAME##12 (S##NAME##12 x, TYPEM y)			\
{ if (x.a != y || x.b != y+1 || x.c != y+2 || x.d != y+3	\
      || x.e != y+4 || x.f != y+5 || x.g != y+6			\
      || x.h != y+7 || x.i != y+8 || x.j != y+9			\
      || x.k != y+10 || x.l != y+11) DEBUG_CHECK }		\
void checkS##NAME##13 (S##NAME##13 x, TYPEM y)			\
{ if (x.a != y || x.b != y+1 || x.c != y+2 || x.d != y+3	\
      || x.e != y+4 || x.f != y+5 || x.g != y+6			\
      || x.h != y+7 || x.i != y+8 || x.j != y+9			\
      || x.k != y+10 || x.l != y+11 || x.m != y+12)		\
    DEBUG_CHECK }						\
void checkS##NAME##14 (S##NAME##14 x, TYPEM y)			\
{ if (x.a != y || x.b != y+1 || x.c != y+2 || x.d != y+3	\
      || x.e != y+4 || x.f != y+5 || x.g != y+6			\
      || x.h != y+7 || x.i != y+8 || x.j != y+9			\
      || x.k != y+10 || x.l != y+11 || x.m != y+12		\
      || x.n != y+13) DEBUG_CHECK }				\
void checkS##NAME##15 (S##NAME##15 x, TYPEM y)			\
{ if (x.a != y || x.b != y+1 || x.c != y+2 || x.d != y+3	\
      || x.e != y+4 || x.f != y+5 || x.g != y+6			\
      || x.h != y+7 || x.i != y+8 || x.j != y+9			\
      || x.k != y+10 || x.l != y+11 || x.m != y+12		\
      || x.n != y+13 || x.o != y+14) DEBUG_CHECK }		\
void checkS##NAME##16 (S##NAME##16 x, TYPEM y)			\
{ if (x.a != y || x.b != y+1 || x.c != y+2 || x.d != y+3	\
      || x.e != y+4 || x.f != y+5 || x.g != y+6			\
      || x.h != y+7 || x.i != y+8 || x.j != y+9			\
      || x.k != y+10 || x.l != y+11 || x.m != y+12		\
      || x.n != y+13 || x.o != y+14 || x.p != y+15)		\
    DEBUG_CHECK }
#define TEST(TYPE,MTYPE)					\
TYPE g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE;		\
TYPE g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE;		\
TYPE g9s##TYPE, g10s##TYPE, g11s##TYPE, g12s##TYPE;		\
TYPE g13s##TYPE, g14s##TYPE, g15s##TYPE, g16s##TYPE;		\
								\
extern void init##TYPE (TYPE *p, MTYPE x);			\
extern void checkg##TYPE (void);				\
extern void							\
test##TYPE (TYPE s1, TYPE s2, TYPE s3, TYPE s4,			\
	    TYPE s5, TYPE s6, TYPE s7, TYPE s8,			\
	    TYPE s9, TYPE s10, TYPE s11, TYPE s12,		\
	    TYPE s13, TYPE s14, TYPE s15, TYPE s16);		\
extern void testva##TYPE (int n, ...);				\
								\
void								\
test2_##TYPE (TYPE s1, TYPE s2, TYPE s3, TYPE s4,		\
	      TYPE s5, TYPE s6, TYPE s7, TYPE s8)		\
{								\
  test##TYPE (s1, g2s##TYPE, s2, g4s##TYPE,			\
	      s3, g6s##TYPE, s4, g8s##TYPE,			\
	      s5, g10s##TYPE, s6, g12s##TYPE,			\
	      s7, g14s##TYPE, s8, g16s##TYPE);			\
}								\
								\
void								\
testit##TYPE (void)						\
{								\
  DEBUG_FPUTS (#TYPE);						\
  DEBUG_FPUTS (" init: ");					\
  init##TYPE  ( &g1s##TYPE,  (MTYPE)1);				\
  init##TYPE  ( &g2s##TYPE,  (MTYPE)2);				\
  init##TYPE  ( &g3s##TYPE,  (MTYPE)3);				\
  init##TYPE  ( &g4s##TYPE,  (MTYPE)4);				\
  init##TYPE  ( &g5s##TYPE,  (MTYPE)5);				\
  init##TYPE  ( &g6s##TYPE,  (MTYPE)6);				\
  init##TYPE  ( &g7s##TYPE,  (MTYPE)7);				\
  init##TYPE  ( &g8s##TYPE,  (MTYPE)8);				\
  init##TYPE  ( &g9s##TYPE,  (MTYPE)9);				\
  init##TYPE  (&g10s##TYPE, (MTYPE)10);				\
  init##TYPE  (&g11s##TYPE, (MTYPE)11);				\
  init##TYPE  (&g12s##TYPE, (MTYPE)12);				\
  init##TYPE  (&g13s##TYPE, (MTYPE)13);				\
  init##TYPE  (&g14s##TYPE, (MTYPE)14);				\
  init##TYPE  (&g15s##TYPE, (MTYPE)15);				\
  init##TYPE  (&g16s##TYPE, (MTYPE)16);				\
  checkg##TYPE ();						\
  DEBUG_NL;							\
  DEBUG_FPUTS (#TYPE);						\
  DEBUG_FPUTS (" test: ");					\
  test##TYPE (g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
	      g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE,	\
	      g9s##TYPE, g10s##TYPE, g11s##TYPE, g12s##TYPE,	\
	      g13s##TYPE, g14s##TYPE, g15s##TYPE, g16s##TYPE);	\
  DEBUG_NL;							\
  DEBUG_FPUTS (#TYPE);						\
  DEBUG_FPUTS (" testva:");					\
  DEBUG_NL;							\
  testva##TYPE (1,						\
		g1s##TYPE);					\
  DEBUG_NL;							\
  testva##TYPE (2,						\
		g1s##TYPE, g2s##TYPE);				\
  DEBUG_NL;							\
  testva##TYPE (3,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE);		\
  DEBUG_NL;							\
  testva##TYPE (4,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE);	\
  DEBUG_NL;							\
  testva##TYPE (5,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
		g5s##TYPE);					\
  DEBUG_NL;							\
  testva##TYPE (6,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
		g5s##TYPE, g6s##TYPE);				\
  DEBUG_NL;							\
  testva##TYPE (7,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
		g5s##TYPE, g6s##TYPE, g7s##TYPE);		\
  DEBUG_NL;							\
  testva##TYPE (8,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
		g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE);	\
  DEBUG_NL;							\
  testva##TYPE (9,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
		g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE,	\
		g9s##TYPE);					\
  DEBUG_NL;							\
  testva##TYPE (10,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
		g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE,	\
		g9s##TYPE, g10s##TYPE);				\
  DEBUG_NL;							\
  testva##TYPE (11,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
		g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE,	\
		g9s##TYPE, g10s##TYPE, g11s##TYPE);		\
  DEBUG_NL;							\
  testva##TYPE (12,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
		g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE,	\
		g9s##TYPE, g10s##TYPE, g11s##TYPE, g12s##TYPE);	\
  DEBUG_NL;							\
  testva##TYPE (13,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
		g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE,	\
		g9s##TYPE, g10s##TYPE, g11s##TYPE, g12s##TYPE,	\
		g13s##TYPE);					\
  DEBUG_NL;							\
  testva##TYPE (14,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
		g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE,	\
		g9s##TYPE, g10s##TYPE, g11s##TYPE, g12s##TYPE,	\
		g13s##TYPE, g14s##TYPE);			\
  DEBUG_NL;							\
  testva##TYPE (15,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
		g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE,	\
		g9s##TYPE, g10s##TYPE, g11s##TYPE, g12s##TYPE,	\
		g13s##TYPE, g14s##TYPE, g15s##TYPE);		\
  DEBUG_NL;							\
  testva##TYPE (16,						\
		g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE,	\
		g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE,	\
		g9s##TYPE, g10s##TYPE, g11s##TYPE, g12s##TYPE,	\
		g13s##TYPE, g14s##TYPE, g15s##TYPE, g16s##TYPE); \
  DEBUG_NL;							\
  DEBUG_FPUTS (#TYPE);						\
  DEBUG_FPUTS (" test2: ");					\
  test2_##TYPE (g1s##TYPE, g3s##TYPE, g5s##TYPE, g7s##TYPE,	\
		g9s##TYPE, g11s##TYPE, g13s##TYPE, g15s##TYPE);	\
  DEBUG_NL;							\
}

#ifndef SKIP_COMPLEX_INT
DEFS(cc, _Complex char)
CHECKS(cc, _Complex char)

TEST(Scc1, _Complex char)
TEST(Scc2, _Complex char)
TEST(Scc3, _Complex char)
TEST(Scc4, _Complex char)
TEST(Scc5, _Complex char)
TEST(Scc6, _Complex char)
TEST(Scc7, _Complex char)
TEST(Scc8, _Complex char)
TEST(Scc9, _Complex char)
TEST(Scc10, _Complex char)
TEST(Scc11, _Complex char)
TEST(Scc12, _Complex char)
TEST(Scc13, _Complex char)
TEST(Scc14, _Complex char)
TEST(Scc15, _Complex char)
TEST(Scc16, _Complex char)
#endif

#undef T

void
struct_by_value_11_x ()
{
#ifndef SKIP_COMPLEX_INT
DEBUG_INIT

#define T(TYPE, MTYPE) testit##TYPE ();

T(Scc1, _Complex char)
T(Scc2, _Complex char)
T(Scc3, _Complex char)
T(Scc4, _Complex char)
T(Scc5, _Complex char)
T(Scc6, _Complex char)
T(Scc7, _Complex char)
T(Scc8, _Complex char)
T(Scc9, _Complex char)
T(Scc10, _Complex char)
T(Scc11, _Complex char)
T(Scc12, _Complex char)
T(Scc13, _Complex char)
T(Scc14, _Complex char)
T(Scc15, _Complex char)
T(Scc16, _Complex char)

DEBUG_FINI

if (fails != 0)
  abort ();

#undef T
#endif
}
