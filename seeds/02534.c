/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize --param aarch64-sve-compare-costs=0" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize --param aarch64-sve-compare-costs=0" } */

#include <stdint.h>

#define TEST(TYPE, NAME, OP)				\
  void __attribute__ ((noinline, noclone))		\
  test_##TYPE##_##NAME (TYPE *__restrict x,		\
			TYPE *__restrict y,		\
			TYPE *__restrict z,		\
			TYPE *__restrict pred, int n)	\
  {							\
    for (int i = 0; i < n; ++i)				\
      x[i] = pred[i] != 1 ? y[i] OP z[i] : y[i];	\
  }

#define TEST_INT_TYPE(TYPE) \
  TEST (TYPE, div, /)

#define TEST_FP_TYPE(TYPE) \
  TEST (TYPE, add, +) \
  TEST (TYPE, sub, -) \
  TEST (TYPE, mul, *) \
  TEST (TYPE, div, /)

#define TEST_ALL \
  TEST_INT_TYPE (int8_t) \
  TEST_INT_TYPE (uint8_t) \
  TEST_INT_TYPE (int16_t) \
  TEST_INT_TYPE (uint16_t) \
  TEST_INT_TYPE (int32_t) \
  TEST_INT_TYPE (uint32_t) \
  TEST_INT_TYPE (int64_t) \
  TEST_INT_TYPE (uint64_t) \
  TEST_FP_TYPE (float) \
  TEST_FP_TYPE (double)

TEST_ALL

/* { dg-final { scan-assembler-not {\t.div\tz[0-9]+\.b} } } */		\
/* { dg-final { scan-assembler-not {\t.div\tz[0-9]+\.h} } } */		\
/* { dg-final { scan-assembler-times {\tsdiv\tz[0-9]+\.s, p[0-7]/m,} 7 } } */
/* At present we don't vectorize the uint8_t or uint16_t loops because the
   division is done directly in the narrow type, rather than being widened
   to int first.  */
/* { dg-final { scan-assembler-times {\tudiv\tz[0-9]+\.s, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tsdiv\tz[0-9]+\.d, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tudiv\tz[0-9]+\.d, p[0-7]/m,} 1 } } */

/* { dg-final { scan-assembler-times {\tfadd\tz[0-9]+\.s, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tfadd\tz[0-9]+\.d, p[0-7]/m,} 1 } } */

/* { dg-final { scan-assembler-times {\tfsub\tz[0-9]+\.s, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tfsub\tz[0-9]+\.d, p[0-7]/m,} 1 } } */

/* { dg-final { scan-assembler-times {\tfmul\tz[0-9]+\.s, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tfmul\tz[0-9]+\.d, p[0-7]/m,} 1 } } */

/* { dg-final { scan-assembler-times {\tfdiv\tz[0-9]+\.s, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tfdiv\tz[0-9]+\.d, p[0-7]/m,} 1 } } */

/* We fail to optimize away the SEL for the int8_t and int16_t loops,
   because the 32-bit result is converted before selection.  */
/* { dg-final { scan-assembler-times {\tsel\t} 2 } } */

#define N 99

#undef TEST
#define TEST(TYPE, NAME, OP)					\
  {								\
    TYPE x[N], y[N], z[N], pred[N];				\
    for (int i = 0; i < N; ++i)					\
      {								\
	y[i] = i * i;						\
	z[i] = ((i + 2) % 3) * (i + 1);				\
	pred[i] = i % 3;					\
      }								\
    test_##TYPE##_##NAME (x, y, z, pred, N);			\
    for (int i = 0; i < N; ++i)					\
      {								\
	TYPE expected = i % 3 != 1 ? y[i] OP z[i] : y[i];	\
	if (x[i] != expected)					\
	  __builtin_abort ();					\
	asm volatile ("" ::: "memory");				\
      }								\
  }

int
main (void)
{
  TEST_ALL
  return 0;
}
