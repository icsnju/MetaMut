/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param riscv-autovec-preference=scalable -fno-vect-cost-model" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv -mabi=ilp32d --param riscv-autovec-preference=scalable -fno-vect-cost-model" } */

#include <stdint-gcc.h>

#define VEC_PERM(TYPE)                                                         \
  TYPE __attribute__ ((noinline, noclone))                                     \
  vec_slp_##TYPE (TYPE *restrict a, int n)                                     \
  {                                                                            \
    for (int i = 0; i < n; ++i)                                                \
      {                                                                        \
	a[i * 8] += 99;                                                        \
	a[i * 8 + 1] += 11;                                                    \
	a[i * 8 + 2] += 17;                                                    \
	a[i * 8 + 3] += 80;                                                    \
	a[i * 8 + 4] += 63;                                                    \
	a[i * 8 + 5] += 37;                                                    \
	a[i * 8 + 6] += 24;                                                    \
	a[i * 8 + 7] += 81;                                                    \
      }                                                                        \
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

TEST_ALL (VEC_PERM)
/* This testcase is from aarch64 and floating-point operations are removed.
   TODO: We will add floating-point operations back and make them as common test in the future. */

#define N (59 * 8)

#define HARNESS(TYPE)						\
  {								\
    TYPE a[N], b[8] = { 99, 11, 17, 80, 63, 37, 24, 81 };	\
    for (unsigned int i = 0; i < N; ++i)			\
      {								\
	a[i] = i * 2 + i % 5;					\
	asm volatile ("" ::: "memory");				\
      }								\
    vec_slp_##TYPE (a, N / 8);					\
    for (unsigned int i = 0; i < N; ++i)			\
      {								\
	TYPE orig = i * 2 + i % 5;				\
	TYPE expected = orig + b[i % 8];			\
	if (a[i] != expected)					\
	  __builtin_abort ();					\
      }								\
  }

int __attribute__ ((optimize (1)))
main (void)
{
  TEST_ALL (HARNESS)
}
/* This testcase is from aarch64 and floating-point operations are removed.
   TODO: We will add floating-point operations back and make them as common test in the future. */
