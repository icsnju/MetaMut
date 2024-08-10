/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=scalable -ffast-math" } */

#include <stdint-gcc.h>

#define TEST_TYPE(TYPE1, TYPE2)                                                \
  __attribute__ ((noipa)) void vwadd_##TYPE1_##TYPE2 (                         \
    TYPE1 *__restrict dst, TYPE1 *__restrict dst2, TYPE1 *__restrict dst3,     \
    TYPE1 *__restrict dst4, TYPE2 *__restrict a, TYPE2 *__restrict b,          \
    TYPE2 *__restrict a2, TYPE2 *__restrict b2, int *__restrict pred, int n)   \
  {                                                                            \
    for (int i = 0; i < n; i++)                                                \
      if (pred[i])                                                             \
	{                                                                      \
	  dst[i] += (TYPE1) a[i] * (TYPE1) b[i];                               \
	  dst2[i] += (TYPE1) a2[i] * (TYPE1) b[i];                             \
	  dst3[i] += (TYPE1) a2[i] * (TYPE1) a[i];                             \
	  dst4[i] += (TYPE1) a[i] * (TYPE1) b2[i];                             \
	}                                                                      \
  }

#define TEST_ALL()                                                             \
  TEST_TYPE (int16_t, int8_t)                                                  \
  TEST_TYPE (uint16_t, uint8_t)                                                \
  TEST_TYPE (int32_t, int16_t)                                                 \
  TEST_TYPE (uint32_t, uint16_t)                                               \
  TEST_TYPE (int64_t, int32_t)                                                 \
  TEST_TYPE (uint64_t, uint32_t)                                               \
  TEST_TYPE (float, _Float16)                                                  \
  TEST_TYPE (double, float)

TEST_ALL ()

/* { dg-final { scan-assembler-times {\tvwmacc\.vv} 12 } } */
/* { dg-final { scan-assembler-times {\tvwmaccu\.vv} 12 } } */
/* { dg-final { scan-assembler-times {\tvfwmacc\.vv} 8 } } */
/* { dg-final { scan-assembler-not {\tvmerge\.vvm\t} } } */
