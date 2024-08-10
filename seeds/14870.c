/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize" } */

#include <stdint.h>

#define DEF_LOOP(TYPE1, TYPE2, CONST, COUNT)			\
  void __attribute__ ((noipa))					\
  test_##CONST##_##TYPE1##_##TYPE2 (TYPE2 *restrict r,		\
				    TYPE1 *restrict a,		\
				    TYPE2 *restrict b)		\
  {								\
    for (int i = 0; i < COUNT; ++i)				\
      r[i] = a[i] > 20 ? b[i] & CONST : 127;			\
  }

#define TEST_ALL(T)			\
  T (int32_t, uint16_t, 0xff, 3)	\
					\
  T (int64_t, uint16_t, 0xff, 5)	\
  T (int64_t, uint32_t, 0xff, 5)	\
  T (int64_t, uint32_t, 0xffff, 5)

TEST_ALL (DEF_LOOP)

/* { dg-final { scan-assembler-times {\tuxtb\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\tuxtb\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tuxth\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s\n} 1 } } */

/* { dg-final { scan-assembler-not {\tsel\t} } } */

#define TEST_LOOP(TYPE1, TYPE2, CONST, N)			\
  {								\
    TYPE1 a[N];							\
    TYPE2 r[N], b[N];						\
    for (int i = 0; i < N; ++i)					\
      {								\
	a[i] = (i & 1 ? -20 - i : 20 + i);			\
	b[i] = -5 - i;						\
	asm volatile ("" ::: "memory");				\
      }								\
    test_##CONST##_##TYPE1##_##TYPE2 (r, a, b);			\
    for (int i = 0; i < N; ++i)					\
      if (r[i] != (TYPE2) (a[i] > 20 ? b[i] & CONST : 127))	\
	__builtin_abort ();					\
  }

int main ()
{
  TEST_ALL (TEST_LOOP)
  return 0;
}
