/* { dg-do compile } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model -march=rv64gcv_zvfh -mabi=lp64d -fno-trapping-math --param=riscv-autovec-preference=scalable" } */

#include <stdint-gcc.h>

#define TEST(TYPE1, TYPE2)                                                     \
  __attribute__ ((noipa)) void vfwcvt_##TYPE1##TYPE2 (TYPE2 *dst, TYPE1 *a,    \
						     int n)                    \
  {                                                                            \
    for (int i = 0; i < n; i++)                                                \
      dst[i] = (TYPE2) a[i];                                                   \
  }

#define TEST_ALL()                                                             \
  TEST (_Float16, int64_t)                                                     \
  TEST (_Float16, uint64_t)                                                    \
  TEST (_Float16, int32_t)                                                     \
  TEST (_Float16, uint32_t)                                                    \
  TEST (float, int64_t)							       \
  TEST (float, uint64_t)						       \

TEST_ALL ()

/* { dg-final { scan-assembler-times {\tvfwcvt\.rtz\.x\.f\.v} 4 } } */
/* { dg-final { scan-assembler-times {\tvfwcvt\.rtz\.xu\.f\.v} 2 } } */
