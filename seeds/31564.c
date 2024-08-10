/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param riscv-autovec-preference=scalable" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d -fno-vect-cost-model --param riscv-autovec-preference=scalable -fdump-tree-optimized-details" } */

#include <stdint-gcc.h>

#define EXTRACT_LAST(TYPE)                                                     \
  TYPE __attribute__ ((noinline, noclone))                                     \
  test_##TYPE (TYPE *x, int n, TYPE value)                                     \
  {                                                                            \
    TYPE last;                                                                 \
    for (int j = 0; j < n; ++j)                                                \
      {                                                                        \
	last = x[j];                                                           \
	x[j] = last * value;                                                   \
      }                                                                        \
    return last;                                                               \
  }

#define TEST_ALL(T)                                                            \
  T (int8_t)                                                                   \
  T (int16_t)                                                                  \
  T (int32_t)                                                                  \
  T (int64_t)                                                                  \
  T (uint8_t)                                                                  \
  T (uint16_t)                                                                 \
  T (uint32_t)                                                                 \
  T (uint64_t)                                                                 \
  T (_Float16)                                                                 \
  T (float)                                                                    \
  T (double)

TEST_ALL (EXTRACT_LAST)

/* { dg-final { scan-tree-dump-times "\.VEC_EXTRACT" 11 "optimized" } } */

#define N 107
#define OP 70

#define TEST_LOOP(TYPE)				\
  {						\
    TYPE a[N];					\
    for (int i = 0; i < N; ++i)			\
      {						\
	a[i] = i * 2 + (i % 3);			\
	asm volatile ("" ::: "memory");		\
      }						\
    TYPE expected = a[N - 1];			\
    TYPE res = test_##TYPE (a, N, OP);		\
    if (res != expected)			\
      __builtin_abort ();			\
    for (int i = 0; i < N; ++i)			\
      {						\
	TYPE old = i * 2 + (i % 3);		\
	if (a[i] != (TYPE) (old * (TYPE) OP))	\
	  __builtin_abort ();			\
	asm volatile ("" ::: "memory");		\
      }						\
  }

int __attribute__ ((optimize (1)))
main (void)
{
  TEST_ALL (TEST_LOOP);
  return 0;
}
