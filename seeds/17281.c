/* { dg-do compile } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model -march=rv64gcv_zvfh -mabi=lp64d --param=riscv-autovec-preference=scalable" } */

#include <stdint-gcc.h>

#define TEST(TYPE1, TYPE2)                                                     \
  __attribute__ ((noipa)) void vfcvt_##TYPE1##TYPE2 (TYPE2 *dst, TYPE1 *a,     \
						     int n)                    \
  {                                                                            \
    for (int i = 0; i < n; i++)                                                \
      dst[i] = (TYPE2) a[i];                                                   \
  }

#define TEST_ALL()                                                             \
  TEST (float, int32_t)                                                        \
  TEST (float, uint32_t)                                                       \
  TEST (double, int64_t)						       \
  TEST (double, uint64_t)						       \
  TEST (_Float16, int16_t)						       \
  TEST (_Float16, uint16_t)						       \

TEST_ALL ()

/* { dg-final { scan-assembler-times {\tvfcvt\.rtz\.x\.f\.v} 3 } } */
/* { dg-final { scan-assembler-times {\tvfcvt\.rtz\.xu\.f\.v} 3 } } */
