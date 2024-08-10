/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param=riscv-autovec-preference=scalable" } */

#include <assert.h>
/* { dg-do compile } */
/* { dg-additional-options "-march=rv64gcv -mabi=lp64d --param=riscv-autovec-preference=scalable -fno-vect-cost-model" } */

#include <stdint-gcc.h>

#define TEST_TYPE(TYPE1, TYPE2)                                                \
  __attribute__ ((noipa)) void vnshift_##TYPE1##_##TYPE2 (                     \
    TYPE2 *__restrict dst, TYPE1 *__restrict a, TYPE2 b, int n)                \
  {                                                                            \
    for (int i = 0; i < n; i++)                                                \
      dst[i] = a[i] >> b;                                                      \
  }

#define TEST_ALL()                                                             \
  TEST_TYPE (int16_t, int8_t)                                                  \
  TEST_TYPE (int16_t, uint8_t)                                                 \
  TEST_TYPE (uint16_t, int8_t)                                                 \
  TEST_TYPE (uint16_t, uint8_t)                                                \
  TEST_TYPE (int32_t, int16_t)                                                 \
  TEST_TYPE (int32_t, uint16_t)                                                \
  TEST_TYPE (uint32_t, int16_t)                                                \
  TEST_TYPE (uint32_t, uint16_t)                                               \
  TEST_TYPE (int64_t, int32_t)                                                 \
  TEST_TYPE (int64_t, uint32_t)                                                \
  TEST_TYPE (uint64_t, int32_t)                                                \
  TEST_TYPE (uint64_t, uint32_t)

TEST_ALL ()

/* { dg-final { scan-assembler-times {\tvnsra\.wx} 8 } } */
/* { dg-final { scan-assembler-times {\tvnsrl\.wx} 4 } } */

#define RUN(TYPE1, TYPE2, SZ)                                                  \
  TYPE1 a##TYPE1##_##TYPE2##_##SZ[SZ];                                         \
  TYPE2 dst##TYPE1##_##TYPE2##_##SZ[SZ];                                       \
  for (int i = 0; i < SZ; i++)                                                 \
    {                                                                          \
      a##TYPE1##_##TYPE2##_##SZ[i] = i % 8723;                                 \
    }                                                                          \
  vnshift_##TYPE1##_##TYPE2 (dst##TYPE1##_##TYPE2##_##SZ,                      \
			     a##TYPE1##_##TYPE2##_##SZ, 9, SZ);                \
  for (int i = 0; i < SZ; i++)                                                 \
    {                                                                          \
      assert (dst##TYPE1##_##TYPE2##_##SZ[i]                                   \
	      == (TYPE2) (a##TYPE1##_##TYPE2##_##SZ[i] >> 9));                 \
    }

#define RUN_ALL(SZ)                                                            \
  RUN (int16_t, int8_t, SZ)                                                    \
  RUN (int16_t, uint8_t, SZ)                                                   \
  RUN (uint16_t, int8_t, SZ)                                                   \
  RUN (uint16_t, uint8_t, SZ)                                                  \
  RUN (int32_t, int16_t, SZ)                                                   \
  RUN (int32_t, uint16_t, SZ)                                                  \
  RUN (uint32_t, int16_t, SZ)                                                  \
  RUN (uint32_t, uint16_t, SZ)                                                 \
  RUN (int64_t, int32_t, SZ)                                                   \
  RUN (int64_t, uint32_t, SZ)                                                  \
  RUN (uint64_t, int32_t, SZ)                                                  \
  RUN (uint64_t, uint32_t, SZ)

int
main ()
{
  RUN_ALL (15)
  RUN_ALL (16)
  RUN_ALL (17)
  RUN_ALL (127)
  RUN_ALL (128)
  RUN_ALL (129)
  RUN_ALL (512)
}
