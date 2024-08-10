/* { dg-do compile } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model -march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=fixed-vlmax -ffast-math" } */

#include <stdlib.h>
#include <stdint-gcc.h>

#define TEST_TYPE(TYPE) 				\
  __attribute__((noipa))				\
  void vabs_##TYPE (TYPE *dst, TYPE *a, int n)		\
  {							\
    for (int i = 0; i < n; i++)				\
      dst[i] = __builtin_abs (a[i]);				\
  }

#define TEST_TYPE2(TYPE) 				\
  __attribute__((noipa))				\
  void vabs_##TYPE (TYPE *dst, TYPE *a, int n)		\
  {							\
    for (int i = 0; i < n; i++)				\
      dst[i] = __builtin_llabs (a[i]);				\
  }

#define TEST_TYPE3(TYPE) 				\
  __attribute__((noipa))				\
  void vabs_##TYPE (TYPE *dst, TYPE *a, int n)		\
  {							\
    for (int i = 0; i < n; i++)				\
      dst[i] = __builtin_fabs (a[i]);				\
  }

#define TEST_ALL()	\
 TEST_TYPE(int8_t)	\
 TEST_TYPE(int16_t)	\
 TEST_TYPE(int32_t)	\
 TEST_TYPE2(int64_t)	\
 TEST_TYPE3(_Float16)	\
 TEST_TYPE3(float)	\
 TEST_TYPE3(double)	\

TEST_ALL()

/* { dg-final { scan-assembler-times {\tvseti?vli\s+[a-z0-9,]+,ta,mu} 4 } } */
/* { dg-final { scan-assembler-times {\tvmslt\.vi} 4 } } */
/* { dg-final { scan-assembler-times {\tvneg.v\sv[0-9]+,v[0-9]+,v0\.t} 4 } } */
/* { dg-final { scan-assembler-times {\tvfabs.v} 3 } } */
