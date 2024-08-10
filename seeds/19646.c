/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -fno-trapping-math" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -fno-trapping-math" } */

#include <stdint.h>

#define DEF_LOOP(TYPE, ABS)				\
  void __attribute__ ((noinline, noclone))		\
  test_##TYPE (TYPE *__restrict r, TYPE *__restrict a,	\
	       TYPE *__restrict b, TYPE *__restrict c,	\
	       int n)					\
  {							\
    for (int i = 0; i < n; ++i)				\
      r[i] = a[i] < 20 ? ABS (b[i] - c[i]) : b[i];	\
  }

#define TEST_ALL(T) \
  T (_Float16, __builtin_fabsf16) \
  T (float, __builtin_fabsf) \
  T (double, __builtin_fabs)

TEST_ALL (DEF_LOOP)

/* { dg-final { scan-assembler-times {\tfabd\tz[0-9]+\.h, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tfabd\tz[0-9]+\.s, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tfabd\tz[0-9]+\.d, p[0-7]/m,} 1 } } */

/* { dg-final { scan-assembler-not {\tmov\tz[^,]*z} } } */
/* { dg-final { scan-assembler-not {\tmovprfx\t} } } */
/* { dg-final { scan-assembler-not {\tsel\t} } } */

#define N 99

#define TEST_LOOP(TYPE, ABS)					\
  {								\
    TYPE r[N], a[N], b[N], c[N];				\
    for (int i = 0; i < N; ++i)					\
      {								\
	a[i] = (i & 1 ? i : 3 * i);				\
	b[i] = (i >> 4) << (i & 15);				\
	c[i] = ((i + 2) % 3) * (i + 1);				\
	asm volatile ("" ::: "memory");				\
      }								\
    test_##TYPE (r, a, b, c, N);				\
    for (int i = 0; i < N; ++i)					\
      {								\
	TYPE expected = a[i] < 20 ? ABS (b[i] - c[i]) : b[i];	\
	if (r[i] != expected)					\
	  __builtin_abort ();					\
	asm volatile ("" ::: "memory");				\
      }								\
  }

int
main (void)
{
  TEST_ALL (TEST_LOOP)
  return 0;
}
