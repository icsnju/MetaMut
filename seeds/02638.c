/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param=riscv-autovec-preference=scalable" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv -mabi=ilp32d --param=riscv-autovec-preference=scalable -fno-vect-cost-model" } */

#include <stdint-gcc.h>

#define DEF_LOOP(TYPE)                                                         \
  void __attribute__ ((noipa)) mod_##TYPE (TYPE *dst, TYPE *src, int count)    \
  {                                                                            \
    for (int i = 0; i < count; ++i)                                            \
      dst[i] = src[i] % 19;                                                    \
  }

#define TEST_ALL(T)                                                            \
  T (int8_t)                                                                   \
  T (uint8_t)                                                                  \
  T (int16_t)                                                                  \
  T (uint16_t)                                                                 \
  T (int32_t)                                                                  \
  T (uint32_t)                                                                 \
  T (int64_t)                                                                  \
  T (uint64_t)

TEST_ALL (DEF_LOOP)

/* { dg-final { scan-assembler-times {\tvmulh\.vv} 4 } } */
/* { dg-final { scan-assembler-times {\tvmulhu\.vv} 4 } } */

#define N 79

#define TEST_LOOP(TYPE)				\
  {						\
    TYPE dst[N], src[N];			\
    for (int i = 0; i < N; ++i)			\
      {						\
	src[i] = i * 7 + i % 3;			\
	if (i % 11 > 7)				\
	  src[i] = -src[i];			\
	asm volatile ("" ::: "memory");		\
      }						\
    mod_##TYPE (dst, src, N);			\
    for (int i = 0; i < N; ++i)			\
      if (dst[i] != src[i] % 19)		\
	__builtin_abort ();			\
  }

int
main (void)
{
  TEST_ALL (TEST_LOOP);
  return 0;
}
