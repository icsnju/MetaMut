/* { dg-do compile } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model -march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=scalable" } */

#include <stdint-gcc.h>

#define TEST(TYPE1, TYPE2)                                        \
  __attribute__ ((noipa))					  \
  void vfncvt_##TYPE1##TYPE2 (TYPE2 *restrict dst,		  \
			      TYPE1 *restrict a, int n)		  \
  {								  \
    for (int i = 0; i < n; i++)					  \
      dst[i] = (TYPE2) (a[i] & 0x7ffffffful);			  \
  }


#define TEST_ALL()						  \
  TEST (int64_t, float)						  \
  TEST (uint64_t, float)					  \
  TEST (int64_t, _Float16)					  \
  TEST (uint64_t, _Float16)					  \
  TEST (int32_t, _Float16)					  \
  TEST (uint32_t, _Float16)					  \

TEST_ALL ()

/* { dg-final { scan-assembler-times {\tvfcvt\.f\.x\.v} 2 } } */
/* { dg-final { scan-assembler-times {\tvfncvt\.f\.x\.w} 2 } } */
/* { dg-final { scan-assembler-times {\tvfncvt\.f\.xu\.w} 2 } } */
