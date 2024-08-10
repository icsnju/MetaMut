/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O3" } */

/* { dg-do assemble { target aarch64_asm_sve_ok } } */
/* { dg-options "-O3 --save-temps" } */

#include <stdint.h>

#define DO_VNLOGICAL(TYPE)				\
void __attribute__ ((noinline, noclone))		\
vnlogical_not_##TYPE (TYPE *dst, int count)		\
{							\
  for (int i = 0; i < count; i++)			\
    dst[i] = ~dst[i];					\
}							\
							\
void __attribute__ ((noinline, noclone))		\
vnlogical_bic_##TYPE (TYPE *dst, TYPE *src, int count)	\
{							\
  for (int i = 0; i < count; i++)			\
    dst[i] = dst[i] & ~src[i];				\
}

#define TEST_ALL(T)				\
  T (int8_t)					\
  T (int16_t)					\
  T (int32_t)					\
  T (int64_t)

TEST_ALL (DO_VNLOGICAL)

/* { dg-final { scan-assembler-times {\tnot\tz[0-9]+\.b, p[0-7]/m, z[0-9]+\.b\n} 1 } } */
/* { dg-final { scan-assembler-times {\tnot\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\tnot\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tnot\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d\n} 1 } } */
/* { dg-final { scan-assembler-times {\tbic\tz[0-9]+\.d, z[0-9]+\.d, z[0-9]+\.d\n} 4 } } */

#define N 128

#define TEST_VNLOGICAL(TYPE)				\
  {							\
    TYPE dst[N], src[N];				\
    for (int i = 0; i < N; ++i)				\
      {							\
	dst[i] = i ^ 42;				\
	asm volatile ("" ::: "memory");			\
      }							\
    vnlogical_not_##TYPE (dst, N);			\
    for (int i = 0; i < N; ++i)				\
      if (dst[i] != (TYPE) ~(i ^ 42))			\
	__builtin_abort ();				\
    for (int i = 0; i < N; ++i)				\
      {							\
	dst[i] = i ^ 42;				\
	src[i] = i % 5;					\
	asm volatile ("" ::: "memory");			\
      }							\
    vnlogical_bic_##TYPE (dst, src, N);			\
    for (int i = 0; i < N; ++i)				\
      if (dst[i] != (TYPE) ((i ^ 42) & ~(i % 5)))	\
	__builtin_abort ();				\
  }

int __attribute__ ((optimize (1)))
main (void)
{
  TEST_ALL (TEST_VNLOGICAL)
  return 0;
}
