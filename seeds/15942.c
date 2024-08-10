/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param=riscv-autovec-preference=scalable -fno-vect-cost-model" } */

/* { dg-do compile } */
/* { dg-additional-options "-std=c99 -march=rv64gcv_zvfh -mabi=lp64d --param=riscv-autovec-preference=scalable -fno-vect-cost-model" } */

#include <stdint-gcc.h>

#define N 2000

#define TEST_LOOP(NAME, TYPE)					\
  void __attribute__ ((noinline, noclone))			\
  NAME (TYPE *restrict dest, TYPE *restrict src)		\
  {								\
    for (int i = 0; i < N; ++i)					\
      dest[i] += src[i * 2];					\
  }

#define TEST(NAME) \
  TEST_LOOP (NAME##_i8, int8_t) \
  TEST_LOOP (NAME##_i16, uint16_t) \
  TEST_LOOP (NAME##_f32, float) \
  TEST_LOOP (NAME##_f64, double)

TEST (test)

/* { dg-final { scan-assembler-times {vlseg2e8\.v} 1 } } */
/* { dg-final { scan-assembler-times {vlseg2e16\.v} 1 } } */
/* { dg-final { scan-assembler-times {vlseg2e32\.v} 1 } } */
/* { dg-final { scan-assembler-times {vlseg2e64\.v} 1 } } */

#undef TEST_LOOP
#define TEST_LOOP(NAME, TYPE)				\
  {							\
    TYPE out[N];					\
    TYPE in[N * 2];					\
    for (int i = 0; i < N; ++i)				\
      {							\
	out[i] = i * 7 / 2;				\
	asm volatile ("" ::: "memory");			\
      }							\
    for (int i = 0; i < N * 2; ++i)			\
      {							\
	in[i] = i * 9 / 2;				\
	asm volatile ("" ::: "memory");			\
      }							\
    NAME (out, in);					\
    for (int i = 0; i < N; ++i)				\
      {							\
	TYPE expected = i * 7 / 2 + in[i * 2];		\
	if (out[i] != expected)				\
	  __builtin_abort ();				\
	asm volatile ("" ::: "memory");			\
      }							\
  }

int __attribute__ ((optimize (1)))
main (void)
{
  TEST (test);
  return 0;
}
