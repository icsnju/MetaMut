/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param riscv-autovec-preference=scalable -fno-vect-cost-model" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv64gcv -mabi=lp64d --param riscv-autovec-preference=scalable -fno-vect-cost-model -fdump-tree-optimized-details" } */

#include <stdint.h>

#define VEC_PERM(TYPE)                                                         \
  TYPE __attribute__ ((noinline, noclone))                                     \
  vec_slp_##TYPE (TYPE *restrict a, TYPE b, TYPE c, int n)                     \
  {                                                                            \
    for (int i = 0; i < n; ++i)                                                \
      {                                                                        \
	a[i * 4] += b;                                                         \
	a[i * 4 + 1] += c;                                                     \
	a[i * 4 + 2] += b;                                                     \
	a[i * 4 + 3] += c;                                                     \
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

/* { dg-final { scan-assembler-times {viota.m} 2 } } */

#define N (103 * 4)

#define HARNESS(TYPE)						\
  {								\
    TYPE a[N], b[2] = { 3, 11 };				\
    for (unsigned int i = 0; i < N; ++i)			\
      {								\
	a[i] = i * 2 + i % 5;					\
	asm volatile ("" ::: "memory");				\
      }								\
    vec_slp_##TYPE (a, b[0], b[1], N / 4);			\
    for (unsigned int i = 0; i < N; ++i)			\
      {								\
	TYPE orig = i * 2 + i % 5;				\
	TYPE expected = orig + b[i % 2];			\
	if (a[i] != expected)					\
	  __builtin_abort ();					\
      }								\
  }

int __attribute__ ((optimize (1)))
main (void)
{
  TEST_ALL (HARNESS)
}
