/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize" } */

#include <stdint.h>

#define LOOP(FACTOR)						\
  __attribute__ ((noipa))					\
  void								\
  test_##FACTOR (uint64_t *restrict dst,			\
		 uint64_t *restrict src, int count)		\
  {								\
    for (int i = 0; i < count; ++i)				\
      dst[i] += (src[i] & 0xffffffff) * FACTOR;			\
  }

#define TEST_ALL(T) T (1) T (2) T (4) T (8)

TEST_ALL (LOOP)

/* { dg-final { scan-assembler-not {\tadd\tz[0-9]+\.d,} } } */
/* { dg-final { scan-assembler-not {\tlsl\tz[0-9]+\.d,} } } */
/* { dg-final { scan-assembler-not {\tand\tz[0-9]+\.d,} } } */
/* { dg-final { scan-assembler-not {\tuxtw\tz[0-9]+\.d,} } } */
/* { dg-final { scan-assembler-times {\tadr\tz[0-9]+\.d, \[z[0-9]+\.d, z[0-9]+\.d, uxtw\]} 1 } } */
/* { dg-final { scan-assembler-times {\tadr\tz[0-9]+\.d, \[z[0-9]+\.d, z[0-9]+\.d, uxtw 1\]} 1 } } */
/* { dg-final { scan-assembler-times {\tadr\tz[0-9]+\.d, \[z[0-9]+\.d, z[0-9]+\.d, uxtw 2\]} 1 } } */
/* { dg-final { scan-assembler-times {\tadr\tz[0-9]+\.d, \[z[0-9]+\.d, z[0-9]+\.d, uxtw 3\]} 1 } } */

#define N 131

#define TEST_LOOP(FACTOR)						\
  {									\
    uint64_t a[N], b[N];						\
    for (int i = 0; i < N; ++i)						\
      {									\
	a[i] = (uint64_t) i * i + i % 5;				\
	b[i] = (uint64_t) (i * 3) << ((i & 7) * 8);			\
	asm volatile ("" ::: "memory");					\
      }									\
    test_##FACTOR (a, b, N);						\
    for (int i = 0; i < N; ++i)						\
      {									\
	uint64_t expected = ((uint64_t) (i * i + i % 5)			\
			     + (((uint64_t) (i * 3) << ((i & 7) * 8))	\
				& 0xffffffff) * FACTOR);		\
	if (a[i] != expected)						\
	  __builtin_abort ();						\
      }									\
  }

int __attribute__ ((optimize (1)))
main (void)
{
  TEST_ALL (TEST_LOOP)
}
