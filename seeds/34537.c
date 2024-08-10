/* { dg-do compile } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model -march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=fixed-vlmax" } */

#include <stdint-gcc.h>

#define TEST(TYPE1, TYPE2)					\
  __attribute__((noipa))					\
  void vfncvt_##TYPE1##TYPE2 (TYPE2 *dst, TYPE1 *a, int n)	\
  {								\
    for (int i = 0; i < n; i++)					\
      dst[i] = (TYPE2)a[i];					\
  }

#define TEST_ALL()						\
 TEST(float, _Float16)						\
 TEST(double, _Float16)						\
 TEST(double, float)						\

TEST_ALL()

/* { dg-final { scan-assembler-times {\tvfncvt\.f\.f\.w} 3 } } */
/* { dg-final { scan-assembler-times {\tvfncvt\.rod\.f\.f\.w} 1 } } */
