/* { dg-do compile } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model -march=rv32gcv -mabi=ilp32d --param=riscv-autovec-preference=fixed-vlmax" } */

#include <stdint-gcc.h>

#define TEST1_TYPE(TYPE)					\
  __attribute__((noipa))					\
  void vshl_##TYPE (TYPE *dst, TYPE *a, TYPE *b, int n)		\
  {								\
    for (int i = 0; i < n; i++)					\
      dst[i] = a[i] << b[i];					\
  }

#define TEST2_TYPE(TYPE)					\
  __attribute__((noipa))					\
  void vshiftr_##TYPE (TYPE *dst, TYPE *a, TYPE *b, int n)	\
  {								\
    for (int i = 0; i < n; i++)					\
      dst[i] = a[i] >> b[i];					\
  }

#define TEST_ALL()	\
 TEST1_TYPE(int8_t)	\
 TEST1_TYPE(uint8_t)	\
 TEST1_TYPE(int16_t)	\
 TEST1_TYPE(uint16_t)	\
 TEST1_TYPE(int32_t)	\
 TEST1_TYPE(uint32_t)	\
 TEST1_TYPE(int64_t)	\
 TEST1_TYPE(uint64_t)   \
 TEST2_TYPE(int8_t)	\
 TEST2_TYPE(uint8_t)	\
 TEST2_TYPE(int16_t)	\
 TEST2_TYPE(uint16_t)	\
 TEST2_TYPE(int32_t)	\
 TEST2_TYPE(uint32_t)	\
 TEST2_TYPE(int64_t)	\
 TEST2_TYPE(uint64_t)

TEST_ALL()

/* { dg-final { scan-assembler-times {\tvsll\.vv} 8 } } */
/* { dg-final { scan-assembler-times {\tvsrl\.vv} 4 } } */
/* { dg-final { scan-assembler-times {\tvsra\.vv} 4 } } */
