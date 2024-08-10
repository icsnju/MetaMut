/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param riscv-autovec-preference=scalable" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d -fno-vect-cost-model --param riscv-autovec-preference=scalable -fdump-tree-optimized-details" } */

#include <stdint-gcc.h>

#define EXTRACT_LAST(TYPE)                                                     \
  _Bool __attribute__ ((noipa))                                                \
  test_##TYPE (TYPE *restrict x, TYPE *restrict y, int n)		       \
  {                                                                            \
    _Bool last;                                                                \
    for (int j = 0; j < n; ++j)                                                \
      {                                                                        \
	last = !x[j];							       \
	y[j] = last;                                                           \
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
  T (uint64_t)

TEST_ALL (EXTRACT_LAST)

/* { dg-final { scan-tree-dump-times "\.VEC_EXTRACT" 8 "optimized" } } */

#define TEST_LOOP(TYPE, N)                                                     \
  {                                                                            \
    TYPE a##N[N];                                                              \
    TYPE b##N[N];                                                              \
    for (int i = 0; i < N; ++i)                                                \
      {                                                                        \
	a##N[i] = i & 1;                                                       \
	b##N[i] = 0;                                                           \
	asm volatile ("" ::: "memory");                                        \
      }                                                                        \
    TYPE expected##N = !(a##N[N - 1]);                                         \
    TYPE res##N = test_##TYPE (a##N, b##N, N);                                 \
    if (res##N != expected##N)                                                 \
      __builtin_abort ();                                                      \
    for (int i = 0; i < N; ++i)                                                \
      {                                                                        \
	if (b##N[i] != !a##N[i])                                               \
	  __builtin_abort ();                                                  \
	asm volatile ("" ::: "memory");                                        \
      }                                                                        \
  }

#define TEST_ALL_N(T, N)                                                       \
  T (int8_t, N)                                                                \
  T (int16_t, N)                                                               \
  T (int32_t, N)                                                               \
  T (int64_t, N)                                                               \
  T (uint8_t, N)                                                               \
  T (uint16_t, N)                                                              \
  T (uint32_t, N)                                                              \
  T (uint64_t, N)

int __attribute__ ((optimize (1))) main (void)
{
  TEST_ALL_N (TEST_LOOP, 2);
  TEST_ALL_N (TEST_LOOP, 3);
  TEST_ALL_N (TEST_LOOP, 4);
  TEST_ALL_N (TEST_LOOP, 5);
  TEST_ALL_N (TEST_LOOP, 6);
  TEST_ALL_N (TEST_LOOP, 7);
  TEST_ALL_N (TEST_LOOP, 8);
  TEST_ALL_N (TEST_LOOP, 17);
  TEST_ALL_N (TEST_LOOP, 64);
  TEST_ALL_N (TEST_LOOP, 107);
  TEST_ALL_N (TEST_LOOP, 255);
  TEST_ALL_N (TEST_LOOP, 256);
  TEST_ALL_N (TEST_LOOP, 4389);
  return 0;
}
