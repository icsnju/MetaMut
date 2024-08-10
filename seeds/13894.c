/* { dg-do run { target { riscv_vector && riscv_zvfh_hw } } } */
/* { dg-additional-options "--param=riscv-autovec-preference=scalable -ffast-math" } */

#include <assert.h>
/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=scalable -ffast-math -fno-vect-cost-model" } */

#include <stdint-gcc.h>

#define TEST_TYPE(TYPE1, TYPE2)                                                \
  __attribute__ ((noipa)) void vwsub_##TYPE1_##TYPE2 (TYPE1 *__restrict dst,   \
						      TYPE2 *__restrict a,     \
						      TYPE2 *__restrict b,     \
						      int n)                   \
  {                                                                            \
    for (int i = 0; i < n; i++)                                                \
      dst[i] = (TYPE1) a[i] - (TYPE1) b[i];                                    \
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

/* { dg-final { scan-assembler-times {\tvwsub\.vv} 3 } } */
/* { dg-final { scan-assembler-times {\tvwsubu\.vv} 3 } } */
/* { dg-final { scan-assembler-times {\tvfwsub\.vv} 2 } } */

#define SZ 512

#define RUN(TYPE1, TYPE2, LIMIT)                                               \
  TYPE2 a##TYPE2[SZ];                                                          \
  TYPE2 b##TYPE2[SZ];                                                          \
  TYPE1 dst##TYPE1[SZ];                                                        \
  for (int i = 0; i < SZ; i++)                                                 \
    {                                                                          \
      a##TYPE2[i] = LIMIT + i % 8723;                                          \
      b##TYPE2[i] = LIMIT + i & 1964;                                          \
    }                                                                          \
  vwsub_##TYPE1_##TYPE2 (dst##TYPE1, a##TYPE2, b##TYPE2, SZ);                  \
  for (int i = 0; i < SZ; i++)                                                 \
    assert (dst##TYPE1[i] == ((TYPE1) a##TYPE2[i] - (TYPE1) b##TYPE2[i]));

#define RUN_ALL() RUN (float, _Float16, -32768)

int
main ()
{
  RUN_ALL ()
}
