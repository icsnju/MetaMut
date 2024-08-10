/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model --param=riscv-autovec-preference=scalable" } */

#include <stdint-gcc.h>

#define TEST(TYPE1, TYPE2)                                        \
  __attribute__ ((noipa))					  \
  void vfwcvt_##TYPE1##TYPE2 (TYPE2 *restrict dst,		  \
			      TYPE1 *restrict a, int n)		  \
  {								  \
    for (int i = 0; i < n; i++)					  \
      dst[i] = (TYPE2) a[i];                                      \
  }

#define TEST_ALL()						  \
  TEST (int8_t, _Float16)					  \
  TEST (uint8_t, _Float16)					  \
  TEST (int8_t, float)						  \
  TEST (uint8_t, float)						  \
  TEST (int8_t, double)						  \
  TEST (uint8_t, double)					  \
  TEST (int16_t, float)						  \
  TEST (uint16_t, float)					  \
  TEST (int16_t, double)					  \
  TEST (uint16_t, double)					  \
  TEST (int32_t, double)					  \
  TEST (uint32_t, double)					  \

TEST_ALL ()

#define RUN(TYPE1, TYPE2, NUM)                                                 \
  TYPE1 src##TYPE1##TYPE2##NUM[NUM];                                           \
  TYPE2 dst##TYPE1##TYPE2##NUM[NUM];                                           \
  for (int i = 0; i < NUM; i++)                                                \
    {                                                                          \
      src##TYPE1##TYPE2##NUM[i] = i * -3 - 88932;                              \
    }                                                                          \
  vfwcvt_##TYPE1##TYPE2 (dst##TYPE1##TYPE2##NUM, src##TYPE1##TYPE2##NUM, NUM); \
  for (int i = 0; i < NUM; i++)                                                \
    if (dst##TYPE1##TYPE2##NUM[i] != (TYPE2) src##TYPE1##TYPE2##NUM[i])        \
      __builtin_abort ();

#define RUN2(TYPE1, TYPE2, NUM)                                                \
  TYPE1 src##TYPE1##TYPE2##NUM[NUM];                                           \
  TYPE2 dst##TYPE1##TYPE2##NUM[NUM];                                           \
  for (int i = 0; i < NUM; i++)                                                \
    {                                                                          \
      src##TYPE1##TYPE2##NUM[i] = i * 3 + 88932;                               \
    }                                                                          \
  vfwcvt_##TYPE1##TYPE2 (dst##TYPE1##TYPE2##NUM, src##TYPE1##TYPE2##NUM, NUM); \
  for (int i = 0; i < NUM; i++)                                                \
    if (dst##TYPE1##TYPE2##NUM[i] != (TYPE2) src##TYPE1##TYPE2##NUM[i])        \
      __builtin_abort ();

int
main ()
{
  RUN (int8_t, float, 3)
  RUN (int8_t, float, 4)
  RUN (int8_t, float, 7)
  RUN (int8_t, float, 99)
  RUN (int8_t, float, 119)
  RUN (int8_t, float, 128)
  RUN (int8_t, float, 256)
  RUN (int8_t, float, 279)
  RUN (int8_t, float, 555)
  RUN (int8_t, float, 1024)
  RUN (int8_t, float, 1389)
  RUN (int8_t, float, 2048)
  RUN (int8_t, float, 3989)
  RUN (int8_t, float, 4096)
  RUN (int8_t, float, 5975)

  RUN2 (uint8_t, float, 3)
  RUN2 (uint8_t, float, 4)
  RUN2 (uint8_t, float, 7)
  RUN2 (uint8_t, float, 99)
  RUN2 (uint8_t, float, 119)
  RUN2 (uint8_t, float, 128)
  RUN2 (uint8_t, float, 256)
  RUN2 (uint8_t, float, 279)
  RUN2 (uint8_t, float, 555)
  RUN2 (uint8_t, float, 1024)
  RUN2 (uint8_t, float, 1389)
  RUN2 (uint8_t, float, 2048)
  RUN2 (uint8_t, float, 3989)
  RUN2 (uint8_t, float, 4096)
  RUN2 (uint8_t, float, 5975)

  RUN (int8_t, double, 3)
  RUN (int8_t, double, 4)
  RUN (int8_t, double, 7)
  RUN (int8_t, double, 99)
  RUN (int8_t, double, 119)
  RUN (int8_t, double, 128)
  RUN (int8_t, double, 256)
  RUN (int8_t, double, 279)
  RUN (int8_t, double, 555)
  RUN (int8_t, double, 1024)
  RUN (int8_t, double, 1389)
  RUN (int8_t, double, 2048)
  RUN (int8_t, double, 3989)
  RUN (int8_t, double, 4096)
  RUN (int8_t, double, 5975)

  RUN2 (uint8_t, double, 3)
  RUN2 (uint8_t, double, 4)
  RUN2 (uint8_t, double, 7)
  RUN2 (uint8_t, double, 99)
  RUN2 (uint8_t, double, 119)
  RUN2 (uint8_t, double, 128)
  RUN2 (uint8_t, double, 256)
  RUN2 (uint8_t, double, 279)
  RUN2 (uint8_t, double, 555)
  RUN2 (uint8_t, double, 1024)
  RUN2 (uint8_t, double, 1389)
  RUN2 (uint8_t, double, 2048)
  RUN2 (uint8_t, double, 3989)
  RUN2 (uint8_t, double, 4096)
  RUN2 (uint8_t, double, 5975)

  RUN (int16_t, float, 3)
  RUN (int16_t, float, 4)
  RUN (int16_t, float, 7)
  RUN (int16_t, float, 99)
  RUN (int16_t, float, 119)
  RUN (int16_t, float, 128)
  RUN (int16_t, float, 256)
  RUN (int16_t, float, 279)
  RUN (int16_t, float, 555)
  RUN (int16_t, float, 1024)
  RUN (int16_t, float, 1389)
  RUN (int16_t, float, 2048)
  RUN (int16_t, float, 3989)
  RUN (int16_t, float, 4096)
  RUN (int16_t, float, 5975)

  RUN2 (uint16_t, float, 3)
  RUN2 (uint16_t, float, 4)
  RUN2 (uint16_t, float, 7)
  RUN2 (uint16_t, float, 99)
  RUN2 (uint16_t, float, 119)
  RUN2 (uint16_t, float, 128)
  RUN2 (uint16_t, float, 256)
  RUN2 (uint16_t, float, 279)
  RUN2 (uint16_t, float, 555)
  RUN2 (uint16_t, float, 1024)
  RUN2 (uint16_t, float, 1389)
  RUN2 (uint16_t, float, 2048)
  RUN2 (uint16_t, float, 3989)
  RUN2 (uint16_t, float, 4096)
  RUN2 (uint16_t, float, 5975)

  RUN (int16_t, double, 3)
  RUN (int16_t, double, 4)
  RUN (int16_t, double, 7)
  RUN (int16_t, double, 99)
  RUN (int16_t, double, 119)
  RUN (int16_t, double, 128)
  RUN (int16_t, double, 256)
  RUN (int16_t, double, 279)
  RUN (int16_t, double, 555)
  RUN (int16_t, double, 1024)
  RUN (int16_t, double, 1389)
  RUN (int16_t, double, 2048)
  RUN (int16_t, double, 3989)
  RUN (int16_t, double, 4096)
  RUN (int16_t, double, 5975)

  RUN2 (uint16_t, double, 3)
  RUN2 (uint16_t, double, 4)
  RUN2 (uint16_t, double, 7)
  RUN2 (uint16_t, double, 99)
  RUN2 (uint16_t, double, 119)
  RUN2 (uint16_t, double, 128)
  RUN2 (uint16_t, double, 256)
  RUN2 (uint16_t, double, 279)
  RUN2 (uint16_t, double, 555)
  RUN2 (uint16_t, double, 1024)
  RUN2 (uint16_t, double, 1389)
  RUN2 (uint16_t, double, 2048)
  RUN2 (uint16_t, double, 3989)
  RUN2 (uint16_t, double, 4096)
  RUN2 (uint16_t, double, 5975)

  RUN (int32_t, double, 3)
  RUN (int32_t, double, 4)
  RUN (int32_t, double, 7)
  RUN (int32_t, double, 99)
  RUN (int32_t, double, 119)
  RUN (int32_t, double, 128)
  RUN (int32_t, double, 256)
  RUN (int32_t, double, 279)
  RUN (int32_t, double, 555)
  RUN (int32_t, double, 1024)
  RUN (int32_t, double, 1389)
  RUN (int32_t, double, 2048)
  RUN (int32_t, double, 3989)
  RUN (int32_t, double, 4096)
  RUN (int32_t, double, 5975)

  RUN2 (uint32_t, double, 3)
  RUN2 (uint32_t, double, 4)
  RUN2 (uint32_t, double, 7)
  RUN2 (uint32_t, double, 99)
  RUN2 (uint32_t, double, 119)
  RUN2 (uint32_t, double, 128)
  RUN2 (uint32_t, double, 256)
  RUN2 (uint32_t, double, 279)
  RUN2 (uint32_t, double, 555)
  RUN2 (uint32_t, double, 1024)
  RUN2 (uint32_t, double, 1389)
  RUN2 (uint32_t, double, 2048)
  RUN2 (uint32_t, double, 3989)
  RUN2 (uint32_t, double, 4096)
  RUN2 (uint32_t, double, 5975)
}
