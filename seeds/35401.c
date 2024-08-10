/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param=riscv-autovec-preference=scalable -fno-vect-cost-model" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv64gcv_zvfh -mabi=lp64d --param riscv-autovec-preference=scalable -fno-vect-cost-model" } */
#include <stdint-gcc.h>

#define TEST_TYPE(TYPE1, TYPE2)                                                \
  __attribute__((noipa))                                                       \
  TYPE1 reduc_##TYPE1##_##TYPE2(TYPE2 *restrict a, int n) {                    \
    TYPE1 sum = 0;                                                             \
    for (int i = 0; i < n; i += 1)                                             \
      sum += a[i];                                                             \
    return sum;                                                                \
  }

#define TEST_ALL(TEST)                                                         \
  TEST(float, _Float16)                                                        \
  TEST(double, float)

TEST_ALL(TEST_TYPE)

/* { dg-final { scan-assembler-times {\tvfwredosum\.vs} 2 } } */

#define N 99

#define RUN(TYPE1, TYPE2)                                                      \
  {                                                                            \
    TYPE2 a[N];                                                                \
    TYPE1 r = 0;                                                               \
    for (int i = 0; i < N; i++) {                                              \
      a[i] = (i * 0.1) * (i & 1 ? 1 : -1);                                     \
      r += a[i];                                                               \
      asm volatile("" ::: "memory");                                           \
    }                                                                          \
    if (r != reduc_##TYPE1##_##TYPE2(a, N))                                    \
      __builtin_abort();                                                       \
  }

int __attribute__((optimize(1))) main() {
  TEST_ALL(RUN)
  return 0;
}
