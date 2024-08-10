/* { dg-do compile } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model -march=rv32gcv -mabi=ilp32d --param=riscv-autovec-preference=fixed-vlmax" } */

#include <stdint-gcc.h>

#define TEST(TYPE1, TYPE2)					\
  __attribute__((noipa))					\
  void vsext_##TYPE1##TYPE2 (TYPE2 *dst, TYPE1 *a, int n)	\
  {								\
    for (int i = 0; i < n; i++)					\
      dst[i] = (TYPE2)a[i];					\
  }

#define TEST_ALL()						\
 TEST(int8_t, int16_t)						\
 TEST(int8_t, int32_t)						\
 TEST(int8_t, int64_t)						\
 TEST(int16_t, int32_t)						\
 TEST(int16_t, int64_t)						\
 TEST(int32_t, int64_t)						\

TEST_ALL()

/* { dg-final { scan-assembler-times {\tvsext\.vf2} 3 } } */
/* { dg-final { scan-assembler-times {\tvsext\.vf4} 2 } } */
/* { dg-final { scan-assembler-times {\tvsext\.vf8} 1 } } */
