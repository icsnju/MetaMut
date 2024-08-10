/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve" } */

#include <stdint.h>

#define DEF_LOOP(TYPE)							\
  void __attribute__ ((noinline, noclone))				\
  test_##TYPE (TYPE *a, TYPE a1, TYPE a2, TYPE a3, TYPE a4, int n)	\
  {									\
    for (int i = 0; i < n; i += 2)					\
      {									\
	a[i] = a[i] >= 1 && a[i] != 3 ? a1 : a2;			\
	a[i + 1] = a[i + 1] >= 1 && a[i + 1] != 3 ? a3 : a4;		\
      }									\
  }

#define FOR_EACH_TYPE(T) \
  T (int8_t) \
  T (uint8_t) \
  T (int16_t) \
  T (uint16_t) \
  T (int32_t) \
  T (uint32_t) \
  T (int64_t) \
  T (uint64_t) \
  T (_Float16) \
  T (float) \
  T (double)

FOR_EACH_TYPE (DEF_LOOP)

/* { dg-final { scan-assembler-times {\tld1b\t} 2 } } */
/* { dg-final { scan-assembler-times {\tld1h\t} 3 } } */
/* { dg-final { scan-assembler-times {\tld1w\t} 3 } } */
/* { dg-final { scan-assembler-times {\tld1d\t} 3 } } */
/* { dg-final { scan-assembler-times {\tsel\tz[0-9]} 11 } } */

#define N 133

#define TEST_LOOP(TYPE)							\
  {									\
    TYPE a[N];								\
    for (int i = 0; i < N; ++i)						\
      a[i] = i % 7;							\
    test_##TYPE (a, 10, 11, 12, 13, N);					\
    for (int i = 0; i < N; ++i)						\
      if (a[i] != 10 + (i & 1) * 2 + (i % 7 == 0 || i % 7 == 3))	\
	__builtin_abort ();						\
  }

int
main (void)
{
  FOR_EACH_TYPE (TEST_LOOP);
  return 0;
}
