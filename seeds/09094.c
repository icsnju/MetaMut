/* { dg-do compile } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model -march=rv32gcv -mabi=ilp32d --param=riscv-autovec-preference=fixed-vlmax" } */

#include <stdint-gcc.h>

#define TEST(TYPE1, TYPE2)					\
  __attribute__((noipa))					\
  void vzext_##TYPE1##TYPE2 (TYPE2 *dst, TYPE1 *a, int n)	\
  {								\
    for (int i = 0; i < n; i++)					\
      dst[i] = (TYPE2)a[i];					\
  }

#define TEST_ALL()						\
 TEST(uint8_t, uint16_t)					\
 TEST(uint8_t, uint32_t)					\
 TEST(uint8_t, uint64_t)					\
 TEST(uint16_t, uint32_t)					\
 TEST(uint16_t, uint64_t)					\
 TEST(uint32_t, uint64_t)					\

TEST_ALL()

/* { dg-final { scan-assembler-times {\tvzext\.vf2} 3 } } */
/* { dg-final { scan-assembler-times {\tvzext\.vf4} 2 } } */
/* { dg-final { scan-assembler-times {\tvzext\.vf8} 1 } } */
