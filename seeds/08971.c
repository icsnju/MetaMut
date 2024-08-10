/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -moverride=sve_width=256" } */

#include <stdint.h>

#define DEF_LOOP(TYPE1, TYPE2, COUNT)				\
  void __attribute__ ((noipa))					\
  test_##TYPE1##_##TYPE2 (TYPE2 *__restrict r,			\
			  TYPE1 *__restrict a,			\
			  TYPE2 *__restrict b)			\
  {								\
    for (int i = 0; i < COUNT; ++i)				\
      r[i] = a[i] == 0 ? b[i] / 16 : b[i];			\
  }

#define TEST_ALL(T) \
  T (int16_t, int8_t, 7) \
  T (int32_t, int8_t, 3) \
  T (int32_t, int16_t, 3) \
  T (int64_t, int8_t, 5) \
  T (int64_t, int16_t, 5) \
  T (int64_t, int32_t, 5)

TEST_ALL (DEF_LOOP)

/* { dg-final { scan-assembler-times {\tasrd\tz[0-9]+\.b, p[0-7]/m, z[0-9]+\.b, #4\n} 3 } } */
/* { dg-final { scan-assembler-times {\tasrd\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h, #4\n} 2 } } */
/* { dg-final { scan-assembler-times {\tasrd\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, #4\n} 1 } } */

/* { dg-final { scan-assembler-not {\tmov\tz} } } */
/* { dg-final { scan-assembler-not {\tmovprfx\t} } } */
/* { dg-final { scan-assembler-not {\tsel\t} } } */

#define TEST_LOOP(TYPE1, TYPE2, N)				\
  {								\
    TYPE1 a[N];							\
    TYPE2 r[N], b[N];						\
    for (int i = 0; i < N; ++i)					\
      {								\
	a[i] = i % 3 < 2 ? 0 : i * 13;				\
	b[i] = (i & 1 ? i : -i) * 17;				\
	asm volatile ("" ::: "memory");				\
      }								\
    test_##TYPE1##_##TYPE2 (r, a, b);				\
    for (int i = 0; i < N; ++i)					\
      if (r[i] != (TYPE2) (a[i] == 0 ? b[i] / 16 : b[i]))	\
	__builtin_abort ();					\
  }

int main ()
{
  TEST_ALL (TEST_LOOP)
  return 0;
}
