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

#define INITS(NAME,TYPEM)					\
void initS##NAME##1 (S##NAME##1 *p, TYPEM y)			\
{ p->a = y; }							\
void initS##NAME##2 (S##NAME##2 *p, TYPEM y)			\
{ p->a = y; p->b = y+1; }					\
void initS##NAME##3 (S##NAME##3 *p, TYPEM y)			\
{ p->a = y; p->b = y+1; p->c = y+2; }				\
void initS##NAME##4 (S##NAME##4 *p, TYPEM y)			\
{ p->a = y; p->b = y+1; p->c = y+2; p->d = y+3; }		\
void initS##NAME##5 (S##NAME##5 *p, TYPEM y)			\
{ p->a = y; p->b = y+1; p->c = y+2; p->d = y+3; p->e = y+4; }	\
void initS##NAME##6 (S##NAME##6 *p, TYPEM y)			\
{ p->a = y; p->b = y+1; p->c = y+2; p->d = y+3; p->e = y+4;	\
  p->f = y+5; }							\
void initS##NAME##7 (S##NAME##7 *p, TYPEM y)			\
{ p->a = y; p->b = y+1; p->c = y+2; p->d = y+3; p->e = y+4;	\
  p->f = y+5; p->g = y+6; }					\
void initS##NAME##8 (S##NAME##8 *p, TYPEM y)			\
{ p->a = y; p->b = y+1; p->c = y+2; p->d = y+3; p->e = y+4;	\
  p->f = y+5; p->g = y+6; p->h = y+7; }				\
void initS##NAME##9 (S##NAME##9 *p, TYPEM y)			\
{ p->a = y; p->b = y+1; p->c = y+2; p->d = y+3; p->e = y+4;	\
  p->f = y+5; p->g = y+6; p->h = y+7; p->i = y+8; }		\
void initS##NAME##10 (S##NAME##10 *p, TYPEM y)			\
{ p->a = y; p->b = y+1; p->c = y+2; p->d = y+3; p->e = y+4;	\
  p->f = y+5; p->g = y+6; p->h = y+7; p->i = y+8; p->j = y+9; }	\
void initS##NAME##11 (S##NAME##11 *p, TYPEM y)			\
{ p->a = y; p->b = y+1; p->c = y+2; p->d = y+3; p->e = y+4;	\
  p->f = y+5; p->g = y+6; p->h = y+7; p->i = y+8; p->j = y+9;	\
  p->k = y+10; }						\
void initS##NAME##12 (S##NAME##12 *p, TYPEM y)			\
{ p->a = y; p->b = y+1; p->c = y+2; p->d = y+3; p->e = y+4;	\
  p->f = y+5; p->g = y+6; p->h = y+7; p->i = y+8; p->j = y+9;	\
  p->k = y+10; p->l = y+11; }					\
void initS##NAME##13 (S##NAME##13 *p, TYPEM y)			\
{ p->a = y; p->b = y+1; p->c = y+2; p->d = y+3; p->e = y+4;	\
  p->f = y+5; p->g = y+6; p->h = y+7; p->i = y+8; p->j = y+9;	\
  p->k = y+10; p->l = y+11; p->m = y+12; }			\
void initS##NAME##14 (S##NAME##14 *p, TYPEM y)			\
{ p->a = y; p->b = y+1; p->c = y+2; p->d = y+3; p->e = y+4;	\
  p->f = y+5; p->g = y+6; p->h = y+7; p->i = y+8; p->j = y+9;	\
  p->k = y+10; p->l = y+11; p->m = y+12; p->n = y+13; }		\
void initS##NAME##15 (S##NAME##15 *p, TYPEM y)			\
{ p->a = y; p->b = y+1; p->c = y+2; p->d = y+3; p->e = y+4;	\
  p->f = y+5; p->g = y+6; p->h = y+7; p->i = y+8; p->j = y+9;	\
  p->k = y+10; p->l = y+11; p->m = y+12; p->n = y+13;		\
  p->o = y+14; }						\
void initS##NAME##16 (S##NAME##16 *p, TYPEM y)			\
{ p->a = y; p->b = y+1; p->c = y+2; p->d = y+3; p->e = y+4;	\
  p->f = y+5; p->g = y+6; p->h = y+7; p->i = y+8; p->j = y+9;	\
  p->k = y+10; p->l = y+11; p->m = y+12; p->n = y+13;		\
  p->o = y+14; p->p = y+15; }
#define TEST(TYPE,TYPE2)					\
extern TYPE g1s##TYPE, g2s##TYPE, g3s##TYPE, g4s##TYPE;		\
extern TYPE g5s##TYPE, g6s##TYPE, g7s##TYPE, g8s##TYPE;		\
extern TYPE g9s##TYPE, g10s##TYPE, g11s##TYPE, g12s##TYPE;	\
extern TYPE g13s##TYPE, g14s##TYPE, g15s##TYPE, g16s##TYPE;	\
								\
extern void check##TYPE (TYPE x, TYPE2 y);			\
								\
void								\
checkg##TYPE (void)						\
{								\
  check##TYPE (  g1s##TYPE,  (TYPE2)1);				\
  check##TYPE (  g2s##TYPE,  (TYPE2)2);				\
  check##TYPE (  g3s##TYPE,  (TYPE2)3);				\
  check##TYPE (  g4s##TYPE,  (TYPE2)4);				\
  check##TYPE (  g5s##TYPE,  (TYPE2)5);				\
  check##TYPE (  g6s##TYPE,  (TYPE2)6);				\
  check##TYPE (  g7s##TYPE,  (TYPE2)7);				\
  check##TYPE (  g8s##TYPE,  (TYPE2)8);				\
  check##TYPE (  g9s##TYPE,  (TYPE2)9);				\
  check##TYPE ( g10s##TYPE, (TYPE2)10);				\
  check##TYPE ( g11s##TYPE, (TYPE2)11);				\
  check##TYPE ( g12s##TYPE, (TYPE2)12);				\
  check##TYPE ( g13s##TYPE, (TYPE2)13);				\
  check##TYPE ( g14s##TYPE, (TYPE2)14);				\
  check##TYPE ( g15s##TYPE, (TYPE2)15);				\
  check##TYPE ( g16s##TYPE, (TYPE2)16);				\
}								\
								\
void								\
test##TYPE (TYPE s1, TYPE s2, TYPE s3, TYPE s4,			\
	    TYPE s5, TYPE s6, TYPE s7, TYPE s8,			\
	    TYPE s9, TYPE s10, TYPE s11, TYPE s12,		\
	    TYPE s13, TYPE s14, TYPE s15, TYPE s16)		\
{								\
  check##TYPE (s1, (TYPE2)1);					\
  check##TYPE (s2, (TYPE2)2);					\
  check##TYPE (s3, (TYPE2)3);					\
  check##TYPE (s4, (TYPE2)4);					\
  check##TYPE (s5, (TYPE2)5);					\
  check##TYPE (s6, (TYPE2)6);					\
  check##TYPE (s7, (TYPE2)7);					\
  check##TYPE (s8, (TYPE2)8);					\
  check##TYPE (s9, (TYPE2)9);					\
  check##TYPE (s10, (TYPE2)10);					\
  check##TYPE (s11, (TYPE2)11);					\
  check##TYPE (s12, (TYPE2)12);					\
  check##TYPE (s13, (TYPE2)13);					\
  check##TYPE (s14, (TYPE2)14);					\
  check##TYPE (s15, (TYPE2)15);					\
  check##TYPE (s16, (TYPE2)16);					\
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
	  check##TYPE (t, (TYPE2)i+1);				\
	}							\
      va_end (ap);						\
    }								\
}

#ifndef SKIP_COMPLEX
DEFS(cf,_Complex float)
INITS(cf, _Complex float)

TEST(Scf13, _Complex float)
TEST(Scf14, _Complex float)
TEST(Scf15, _Complex float)
TEST(Scf16, _Complex float)
#endif
