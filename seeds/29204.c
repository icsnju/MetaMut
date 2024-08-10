/* { dg-do run } */
/* { dg-options "-DEXPENSIVE" { target run_expensive_tests } } */
/* { dg-skip-if "" { ! run_expensive_tests } { "*" } { "-O2" } } */
/* { dg-skip-if "" { ! run_expensive_tests } { "-flto" } { "" } } */

#if __SIZEOF_FLOAT__ == 4
typedef float V __attribute__((vector_size(8)));
# if __SIZEOF_INT__ == 4
typedef unsigned int VI __attribute__((vector_size(8)));
# elif __SIZEOF_LONG__ == 4
typedef unsigned long VI __attribute__((vector_size(8)));
# else
#  define UNSUPPORTED
# endif
#else
# define UNSUPPORTED
#endif

#define N 2
#define TESTS \
T (0,	0, 0) \
T (1,	0, 1) \
T (2,	0, 2) \
T (3,	0, 3) \
T (4,	1, 0) \
T (5,	1, 1) \
T (6,	1, 2) \
T (7,	1, 3) \
T (8,	2, 0) \
T (9,	2, 1) \
T (10,	2, 2) \
T (11,	2, 3) \
T (12,	3, 0) \
T (13,	3, 1) \
T (14,	3, 2) \
T (15,	3, 3)
#define EXPTESTS
/* Driver fragment for __builtin_shuffle of any vector shape.  */

extern void abort (void);

#ifndef UNSUPPORTED
V a, b, c, d;

#define T(num, msk...) \
__attribute__((noinline, noclone)) void		\
test_##num (void)				\
{						\
  VI mask = { msk };				\
  int i;					\
  c = __builtin_shuffle (a, mask);		\
  d = __builtin_shuffle (a, b, mask);		\
  __asm ("" : : "r" (&c), "r" (&d) : "memory");	\
  for (i = 0; i < N; ++i)			\
    if (c[i] != a[mask[i] & (N - 1)])		\
      abort ();					\
    else if ((mask[i] & N))			\
      {						\
	if (d[i] != b[mask[i] & (N - 1)])	\
	  abort ();				\
      }						\
    else if (d[i] != a[mask[i] & (N - 1)])	\
      abort ();					\
}
TESTS
#ifdef EXPENSIVE
EXPTESTS
#endif
#endif

int
main ()
{
#ifndef UNSUPPORTED
  int i;
  for (i = 0; i < N; ++i)
    {
      a[i] = i + 2;
      b[i] = N + i + 2;
    }

#undef T
#define T(num, msk...) \
  test_##num ();
  TESTS
#ifdef EXPENSIVE
  EXPTESTS
#endif
#endif

  return 0;
}
