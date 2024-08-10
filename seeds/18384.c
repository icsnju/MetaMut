/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O2 -ftree-vectorize -ftrapping-math" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -fno-trapping-math" } */

#include <stdint.h>

#define DEF_LOOP(FLOAT_TYPE, INT_TYPE)				\
  void __attribute__ ((noipa))					\
  test_##INT_TYPE (INT_TYPE *__restrict r,			\
		   FLOAT_TYPE *__restrict a,			\
		   INT_TYPE *__restrict b,			\
		   INT_TYPE *__restrict pred, int n)		\
  {								\
    for (int i = 0; i < n; ++i)					\
      {								\
	INT_TYPE bi = b[i];					\
	r[i] = pred[i] ? (INT_TYPE) a[i] : bi;			\
      }								\
  }

#define TEST_ALL(T) \
  T (_Float16, int16_t) \
  T (_Float16, uint16_t) \
  T (float, int32_t) \
  T (float, uint32_t) \
  T (double, int64_t) \
  T (double, uint64_t)

TEST_ALL (DEF_LOOP)

/* { dg-final { scan-assembler-times {\tfcvtzs\tz[0-9]+\.h, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tfcvtzu\tz[0-9]+\.h, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tfcvtzs\tz[0-9]+\.s, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tfcvtzu\tz[0-9]+\.s, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tfcvtzs\tz[0-9]+\.d, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tfcvtzu\tz[0-9]+\.d, p[0-7]/m,} 1 } } */

/* { dg-final { scan-assembler-not {\tmov\tz} } } */
/* { dg-final { scan-assembler-not {\tmovprfx\t} } } */
/* { dg-final { scan-assembler-not {\tsel\t} } } */

#define N 99

#define TEST_LOOP(FLOAT_TYPE, INT_TYPE)				\
  {								\
    INT_TYPE r[N], b[N], pred[N];				\
    FLOAT_TYPE a[N];						\
    for (int i = 0; i < N; ++i)					\
      {								\
	a[i] = (i & 1 ? i : 3 * i) * (i % 3 == 0 ? 1 : -1);	\
	b[i] = (i % 9) * (i % 7 + 1);				\
	pred[i] = (i % 7 < 4);					\
	asm volatile ("" ::: "memory");				\
      }								\
    test_##INT_TYPE (r, a, b, pred, N);				\
    for (int i = 0; i < N; ++i)					\
      if (r[i] != (pred[i] ? (INT_TYPE) a[i] : b[i]))		\
	__builtin_abort ();					\
  }

int main ()
{
  TEST_ALL (TEST_LOOP)
  return 0;
}
