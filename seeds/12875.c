/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize" } */

#include <stdint.h>

#define abd(A, B) (((A) < (B) ? (B) : (A)) - ((A) < (B) ? (A) : (B)))

#define DEF_LOOP(TYPE)					\
  void __attribute__ ((noinline, noclone))		\
  test_##TYPE (TYPE *__restrict r, TYPE *__restrict a,	\
	       TYPE *__restrict b, TYPE *__restrict c,	\
	       int n)					\
  {							\
    for (int i = 0; i < n; ++i)				\
      r[i] = a[i] < 20 ? abd (b[i], c[i]) : 0;		\
  }

#define TEST_ALL(T) \
  T (int8_t) \
  T (uint8_t) \
  T (int16_t) \
  T (uint16_t) \
  T (int32_t) \
  T (uint32_t) \
  T (int64_t) \
  T (uint64_t)

TEST_ALL (DEF_LOOP)

/* { dg-final { scan-assembler-times {\tsabd\tz[0-9]+\.b, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tsabd\tz[0-9]+\.h, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tsabd\tz[0-9]+\.s, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tsabd\tz[0-9]+\.d, p[0-7]/m,} 1 } } */

/* { dg-final { scan-assembler-times {\tuabd\tz[0-9]+\.b, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tuabd\tz[0-9]+\.h, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tuabd\tz[0-9]+\.s, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tuabd\tz[0-9]+\.d, p[0-7]/m,} 1 } } */

/* { dg-final { scan-assembler-times {\tmovprfx\tz[0-9]+\.b, p[0-7]/z, z[0-9]+\.b\n} 2 } } */
/* { dg-final { scan-assembler-times {\tmovprfx\tz[0-9]+\.h, p[0-7]/z, z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\tmovprfx\tz[0-9]+\.s, p[0-7]/z, z[0-9]+\.s\n} 2 } } */
/* { dg-final { scan-assembler-times {\tmovprfx\tz[0-9]+\.d, p[0-7]/z, z[0-9]+\.d\n} 2 } } */

/* { dg-final { scan-assembler-not {\tmov\tz[^,]*z} } } */
/* { dg-final { scan-assembler-not {\tsel\t} } } */

#define N 99

#define TEST_LOOP(TYPE)						\
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
	TYPE expected = a[i] < 20 ? abd (b[i], c[i]) : 0;	\
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
