/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param riscv-autovec-preference=fixed-vlmax" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfhmin -mabi=ilp32d --param riscv-autovec-preference=fixed-vlmax -fdump-tree-vect-details" } */

#include <assert.h>
#include <stdint-gcc.h>

#define N 777

#define test_1(TYPE)                                                           \
  TYPE a_##TYPE[N] = {0};                                                      \
  TYPE b_##TYPE[N] = {0};                                                      \
  void __attribute__ ((noinline, noclone))                                     \
  test_1_##TYPE (int *__restrict cond)                                         \
  {                                                                            \
    unsigned int i = 0;                                                        \
    for (i = 0; i < 8; i++)                                                    \
      if (cond[i])                                                             \
	b_##TYPE[i] = a_##TYPE[i];                                             \
  }

#define run_1(TYPE)                                                            \
  int cond_##TYPE[N] = {0};                                                    \
  for (unsigned int i = 0; i < N; i++)                                         \
    a_##TYPE[i] = i * 2 * 33 + 1 + 109;                                        \
  for (unsigned int i = 0; i < N; i++)                                         \
    cond_##TYPE[i] = i & 1;                                                    \
  test_1_##TYPE (cond_##TYPE);                                                 \
  for (unsigned int i = 0; i < N; i++)                                         \
    {                                                                          \
      if (cond_##TYPE[i] && i < 8)                                             \
	assert (b_##TYPE[i] == a_##TYPE[i]);                                   \
      else                                                                     \
	assert (b_##TYPE[i] == 0);                                             \
    }

#define TEST_ALL(T)                                                            \
  T (int8_t)                                                                   \
  T (uint8_t)                                                                  \
  T (int16_t)                                                                  \
  T (uint16_t)                                                                 \
  T (int32_t)                                                                  \
  T (uint32_t)                                                                 \
  T (int64_t)                                                                  \
  T (uint64_t)                                                                 \
  T (_Float16)                                                                 \
  T (float)                                                                    \
  T (double)

TEST_ALL (test_1)

/* { dg-final { scan-tree-dump-times "vectorized 1 loops in function" 11 "vect" } } */

int main (void)
{
  TEST_ALL (run_1)
  return 0;
}
