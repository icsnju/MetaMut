/* { dg-do run { target { riscv_vector && riscv_zvfh_hw } } } */
/* { dg-additional-options "-std=c99 -march=rv64gcv_zvfh -mabi=lp64d -fno-vect-cost-model --param=riscv-autovec-preference=scalable" } */

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

#define RUN(TYPE1, TYPE2, NUM)                                                 \
  TYPE1 src##TYPE1##TYPE2##NUM[NUM];                                           \
  TYPE2 dst##TYPE1##TYPE2##NUM[NUM];                                           \
  for (int i = 0; i < NUM; i++)                                                \
    {                                                                          \
      src##TYPE1##TYPE2##NUM[i] = i * -3.1315926 - 8.947289;                   \
    }                                                                          \
  vfcvt_##TYPE1##TYPE2 (dst##TYPE1##TYPE2##NUM, src##TYPE1##TYPE2##NUM, NUM);  \
  for (int i = 0; i < NUM; i++)                                                \
    if (dst##TYPE1##TYPE2##NUM[i] != (TYPE2) src##TYPE1##TYPE2##NUM[i])        \
      __builtin_abort ();

#define RUN2(TYPE1, TYPE2, NUM)                                                \
  TYPE1 src##TYPE1##TYPE2##NUM[NUM];                                           \
  TYPE2 dst##TYPE1##TYPE2##NUM[NUM];                                           \
  for (int i = 0; i < NUM; i++)                                                \
    {                                                                          \
      src##TYPE1##TYPE2##NUM[i] = i * 3.1315926 + 8.947289;                    \
    }                                                                          \
  vfcvt_##TYPE1##TYPE2 (dst##TYPE1##TYPE2##NUM, src##TYPE1##TYPE2##NUM, NUM);  \
  for (int i = 0; i < NUM; i++)                                                \
    if (dst##TYPE1##TYPE2##NUM[i] != (TYPE2) src##TYPE1##TYPE2##NUM[i])        \
      __builtin_abort ();

int
main ()
{
  RUN (_Float16, int8_t, 3)
  RUN (_Float16, int8_t, 4)
  RUN (_Float16, int8_t, 13)
  RUN (_Float16, int8_t, 40)

  RUN2 (_Float16, uint8_t, 1)
  RUN2 (_Float16, uint8_t, 8)
  RUN2 (_Float16, uint8_t, 21)
  RUN2 (_Float16, uint8_t, 33)
}
