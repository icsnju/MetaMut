/* { dg-do run { target { riscv_vector && riscv_zvfh_hw } } } */
/* { dg-additional-options "-std=c99 -march=rv64gcv_zvfh -mabi=lp64d -fno-vect-cost-model --param=riscv-autovec-preference=scalable" } */

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
      src##TYPE1##TYPE2##NUM[i] = i * -3 - 8;                                  \
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
      src##TYPE1##TYPE2##NUM[i] = i * 3 + 8;                                   \
    }                                                                          \
  vfwcvt_##TYPE1##TYPE2 (dst##TYPE1##TYPE2##NUM, src##TYPE1##TYPE2##NUM, NUM); \
  for (int i = 0; i < NUM; i++)                                                \
    if (dst##TYPE1##TYPE2##NUM[i] != (TYPE2) src##TYPE1##TYPE2##NUM[i])        \
      __builtin_abort ();

int
main ()
{
  RUN (int8_t, _Float16, 3)
  RUN (int8_t, _Float16, 4)
  RUN (int8_t, _Float16, 7)
  RUN (int8_t, _Float16, 12)
  RUN (int8_t, _Float16, 20)
  RUN (int8_t, _Float16, 27)

  RUN (uint8_t, _Float16, 4)
  RUN (uint8_t, _Float16, 8)
  RUN (uint8_t, _Float16, 11)
  RUN (uint8_t, _Float16, 29)
  RUN (uint8_t, _Float16, 49)
}
