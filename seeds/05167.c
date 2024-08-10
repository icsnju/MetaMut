/* { dg-do compile } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model -march=rv64gcv -mabi=lp64d --param=riscv-autovec-preference=fixed-vlmax" } */

#include <stdlib.h>
#include <stdint-gcc.h>

#define TEST_TYPE(TYPE) 				\
  __attribute__((noipa))				\
  void vnot_##TYPE (TYPE *dst, TYPE *a, int n)		\
  {							\
    for (int i = 0; i < n; i++)				\
      dst[i] =  ~a[i];					\
  }

#define TEST_ALL()					\
 TEST_TYPE(int8_t)					\
 TEST_TYPE(uint8_t)					\
 TEST_TYPE(int16_t)					\
 TEST_TYPE(uint16_t)					\
 TEST_TYPE(int32_t)					\
 TEST_TYPE(uint32_t)					\
 TEST_TYPE(int64_t)					\
 TEST_TYPE(uint64_t)

TEST_ALL()

/* { dg-final { scan-assembler-times {\tvnot\.v} 8 } } */
