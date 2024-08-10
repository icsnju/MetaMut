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

#define T(N, NAME, TYPE)					\
struct S##NAME##N { TYPE i[N]; };				\
								\
extern struct S##NAME##N g1s##NAME##N, g2s##NAME##N;		\
extern struct S##NAME##N g3s##NAME##N, g4s##NAME##N;		\
extern struct S##NAME##N g5s##NAME##N, g6s##NAME##N;		\
extern struct S##NAME##N g7s##NAME##N, g8s##NAME##N;		\
extern struct S##NAME##N g9s##NAME##N, g10s##NAME##N;		\
extern struct S##NAME##N g11s##NAME##N, g12s##NAME##N;		\
extern struct S##NAME##N g13s##NAME##N, g14s##NAME##N;		\
extern struct S##NAME##N g15s##NAME##N, g16s##NAME##N;		\
								\
extern void check##NAME##N (struct S##NAME##N *p, int i);	\
extern void							\
test2_##NAME##N (struct S##NAME##N s1, struct S##NAME##N s2,	\
		 struct S##NAME##N s3, struct S##NAME##N s4,	\
		 struct S##NAME##N s5, struct S##NAME##N s6,	\
		 struct S##NAME##N s7, struct S##NAME##N s8);	\
								\
void								\
init##NAME##N (struct S##NAME##N *p, int i)			\
{								\
  int j;							\
  for (j = 0; j < N; j++)					\
    p->i[j] = i + j;						\
}								\
								\
void								\
checkg##NAME##N (void)						\
{								\
  check##NAME##N ( &g1s##NAME##N,  1*16);			\
  check##NAME##N ( &g2s##NAME##N,  2*16);			\
  check##NAME##N ( &g3s##NAME##N,  3*16);			\
  check##NAME##N ( &g4s##NAME##N,  4*16);			\
  check##NAME##N ( &g5s##NAME##N,  5*16);			\
  check##NAME##N ( &g6s##NAME##N,  6*16);			\
  check##NAME##N ( &g7s##NAME##N,  7*16);			\
  check##NAME##N ( &g8s##NAME##N,  8*16);			\
  check##NAME##N ( &g9s##NAME##N,  9*16);			\
  check##NAME##N (&g10s##NAME##N, 10*16);			\
  check##NAME##N (&g11s##NAME##N, 11*16);			\
  check##NAME##N (&g12s##NAME##N, 12*16);			\
  check##NAME##N (&g13s##NAME##N, 13*16);			\
  check##NAME##N (&g14s##NAME##N, 14*16);			\
  check##NAME##N (&g15s##NAME##N, 15*16);			\
  check##NAME##N (&g16s##NAME##N, 16*16);			\
}								\
								\
void								\
test##NAME##N (struct S##NAME##N s1, struct S##NAME##N s2,	\
	       struct S##NAME##N s3, struct S##NAME##N s4,	\
	       struct S##NAME##N s5, struct S##NAME##N s6,	\
	       struct S##NAME##N s7, struct S##NAME##N s8,	\
	       struct S##NAME##N s9, struct S##NAME##N s10,	\
	       struct S##NAME##N s11, struct S##NAME##N s12,	\
	       struct S##NAME##N s13, struct S##NAME##N s14,	\
	       struct S##NAME##N s15, struct S##NAME##N s16)	\
{								\
  check##NAME##N (&s1, 1*16);					\
  check##NAME##N (&s2, 2*16);					\
  check##NAME##N (&s3, 3*16);					\
  check##NAME##N (&s4, 4*16);					\
  check##NAME##N (&s5, 5*16);					\
  check##NAME##N (&s6, 6*16);					\
  check##NAME##N (&s7, 7*16);					\
  check##NAME##N (&s8, 8*16);					\
  check##NAME##N (&s9, 9*16);					\
  check##NAME##N (&s10, 10*16);					\
  check##NAME##N (&s11, 11*16);					\
  check##NAME##N (&s12, 12*16);					\
  check##NAME##N (&s13, 13*16);					\
  check##NAME##N (&s14, 14*16);					\
  check##NAME##N (&s15, 15*16);					\
  check##NAME##N (&s16, 16*16);					\
}								\
								\
void								\
testva##NAME##N (int n, ...)					\
{								\
  int i;							\
  va_list ap;							\
  if (test_va)							\
    {								\
      va_start (ap, n);						\
      for (i = 0; i < n; i++)					\
	{							\
	  struct S##NAME##N t = va_arg (ap, struct S##NAME##N);	\
	  check##NAME##N (&t, (i+1)*16);			\
	}							\
      va_end (ap);						\
    }								\
}

#ifndef SKIP_ZERO_ARRAY
T(0, uc, unsigned char)
#endif
T(1, uc, unsigned char)
T(2, uc, unsigned char)
T(3, uc, unsigned char)
T(4, uc, unsigned char)
T(5, uc, unsigned char)
T(6, uc, unsigned char)
T(7, uc, unsigned char)
T(8, uc, unsigned char)
T(9, uc, unsigned char)
T(10, uc, unsigned char)
T(11, uc, unsigned char)
T(12, uc, unsigned char)
T(13, uc, unsigned char)
T(14, uc, unsigned char)
T(15, uc, unsigned char)
#ifndef SKIP_ZERO_ARRAY
T(0, us, unsigned short)
#endif
T(1, us, unsigned short)
T(2, us, unsigned short)
T(3, us, unsigned short)
T(4, us, unsigned short)
T(5, us, unsigned short)
T(6, us, unsigned short)
T(7, us, unsigned short)
T(8, us, unsigned short)
T(9, us, unsigned short)
T(10, us, unsigned short)
T(11, us, unsigned short)
T(12, us, unsigned short)
T(13, us, unsigned short)
T(14, us, unsigned short)
T(15, us, unsigned short)
#ifndef SKIP_ZERO_ARRAY
T(0, ui, unsigned int)
#endif
T(1, ui, unsigned int)
T(2, ui, unsigned int)
T(3, ui, unsigned int)
T(4, ui, unsigned int)
T(5, ui, unsigned int)
T(6, ui, unsigned int)
T(7, ui, unsigned int)
T(8, ui, unsigned int)
T(9, ui, unsigned int)
T(10, ui, unsigned int)
T(11, ui, unsigned int)
T(12, ui, unsigned int)
T(13, ui, unsigned int)
T(14, ui, unsigned int)
T(15, ui, unsigned int)
