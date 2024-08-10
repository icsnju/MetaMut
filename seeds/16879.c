/* { dg-do compile } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model -march=rv32gcv_zvfh -mabi=ilp32d -fno-trapping-math --param=riscv-autovec-preference=scalable" } */

#include <stdint-gcc.h>

#define TEST(TYPE1, TYPE2)                                                     \
  __attribute__ ((noipa)) void vfncvt_##TYPE1##TYPE2 (TYPE2 *dst, TYPE1 *a,    \
						     int n)                    \
  {                                                                            \
    for (int i = 0; i < n; i++)                                                \
      dst[i] = (TYPE2) a[i];                                                   \
  }

#define TEST_ALL()                                                             \
  TEST (double, int32_t)                                                       \
  TEST (double, uint32_t)                                                      \
  TEST (double, int16_t)                                                       \
  TEST (double, uint16_t)                                                      \
  TEST (double, int8_t)                                                        \
  TEST (double, uint8_t)                                                       \
  TEST (float, int16_t)							       \
  TEST (float, uint16_t)						       \
  TEST (float, int8_t)							       \
  TEST (float, uint8_t)							       \
  TEST (_Float16, int8_t)						       \
  TEST (_Float16, uint8_t)						       \

TEST_ALL ()

/* The vectorizer only performs int -> float conversions with
   intermediate types with -fno-trapping-math.  */
/* { dg-final { scan-assembler-times {\tvfncvt\.rtz\.x\.f\.w} 9 } } */
/* { dg-final { scan-assembler-times {\tvfncvt\.rtz\.xu\.f\.w} 3 } } */
