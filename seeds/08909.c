/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O2 -ftree-vectorize -ftrapping-math" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -fno-trapping-math" } */

#include <stdint.h>

#define DEF_LOOP(FLOAT_TYPE, INT_TYPE)				\
  void __attribute__ ((noipa))					\
  test_##INT_TYPE (FLOAT_TYPE *__restrict r,			\
		   INT_TYPE *__restrict a,			\
		   INT_TYPE *__restrict pred, int n)		\
  {								\
    for (int i = 0; i < n; ++i)					\
      r[i] = pred[i] ? (FLOAT_TYPE) a[i] : 1.0;			\
  }

#define TEST_ALL(T) \
  T (_Float16, int16_t) \
  T (_Float16, uint16_t) \
  T (float, int32_t) \
  T (float, uint32_t) \
  T (double, int64_t) \
  T (double, uint64_t)

TEST_ALL (DEF_LOOP)

/* { dg-final { scan-assembler-times {\tscvtf\tz[0-9]+\.h, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tucvtf\tz[0-9]+\.h, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tscvtf\tz[0-9]+\.s, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tucvtf\tz[0-9]+\.s, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tscvtf\tz[0-9]+\.d, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tucvtf\tz[0-9]+\.d, p[0-7]/m,} 1 } } */

/* { dg-final { scan-assembler-times {\tmovprfx\tz[0-9]+, z[0-9]+\n} 6 } } */

/* { dg-final { scan-assembler-not {\tmov\tz[^\n]*z} } } */
/* { dg-final { scan-assembler-not {\tsel\t} } } */

#define N 99

#define TEST_LOOP(FLOAT_TYPE, INT_TYPE)				\
  {								\
    FLOAT_TYPE r[N];						\
    INT_TYPE a[N], pred[N];					\
    for (int i = 0; i < N; ++i)					\
      {								\
	a[i] = (i & 1 ? i : 3 * i) * (i % 3 == 0 ? 1 : -1);	\
	pred[i] = (i % 7 < 4);					\
	asm volatile ("" ::: "memory");				\
      }								\
    test_##INT_TYPE (r, a, pred, N);				\
    for (int i = 0; i < N; ++i)					\
      if (r[i] != (pred[i] ? (FLOAT_TYPE) a[i] : 1.0))		\
	__builtin_abort ();					\
  }

int main ()
{
  TEST_ALL (TEST_LOOP)
  return 0;
}
