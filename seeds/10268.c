/* { dg-do compile } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model -march=rv32gcv -mabi=ilp32d --param=riscv-autovec-preference=scalable" } */

#include <stdint-gcc.h>

#define TEST_TYPE(TYPE, TYPE2)                                                 \
  __attribute__ ((noipa)) void vavg_##TYPE (TYPE *dst, TYPE *a, TYPE *b,       \
					    int n)                             \
  {                                                                            \
    for (int i = 0; i < n; i++)                                                \
      dst[i] = ((TYPE2) a[i] + b[i]) >> 1;                                     \
  }

#define TEST_TYPE2(TYPE, TYPE2)                                                \
  __attribute__ ((noipa)) void vavg2_##TYPE (TYPE *dst, TYPE *a, TYPE *b,      \
					     int n)                            \
  {                                                                            \
    for (int i = 0; i < n; i++)                                                \
      dst[i] = ((TYPE2) a[i] + b[i] + 1) >> 1;                                 \
  }

#define TEST_ALL()                                                             \
  TEST_TYPE (int8_t, int16_t)                                                  \
  TEST_TYPE (uint8_t, uint16_t)                                                \
  TEST_TYPE (int16_t, int32_t)                                                 \
  TEST_TYPE (uint16_t, uint32_t)                                               \
  TEST_TYPE (int32_t, int64_t)                                                 \
  TEST_TYPE (uint32_t, uint64_t)                                               \
  TEST_TYPE2 (int8_t, int16_t)                                                 \
  TEST_TYPE2 (uint8_t, uint16_t)                                               \
  TEST_TYPE2 (int16_t, int32_t)                                                \
  TEST_TYPE2 (uint16_t, uint32_t)                                              \
  TEST_TYPE2 (int32_t, int64_t)                                                \
  TEST_TYPE2 (uint32_t, uint64_t)

TEST_ALL()

/* { dg-final { scan-assembler-times {\tvwadd\.vv} 6 } } */
/* { dg-final { scan-assembler-times {\tvwaddu\.vv} 6 } } */
/* { dg-final { scan-assembler-times {\tvadd\.vi} 6 } } */
/* { dg-final { scan-assembler-times {\tvnsrl.wi} 6 } } */
/* { dg-final { scan-assembler-times {\tvnsra.wi} 6 } } */
