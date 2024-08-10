/* { dg-do compile } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model -march=rv64gcv_zvfh -mabi=lp64d --param=riscv-autovec-preference=fixed-vlmax -ffast-math" } */

#include <stdint-gcc.h>

#define TEST_TYPE(TYPE) 				\
  __attribute__((noipa))				\
  void vsqrt_##TYPE (TYPE *dst, TYPE *a, int n)		\
  {							\
    for (int i = 0; i < n; i++)				\
      dst[i] = __builtin_sqrtf (a[i]);			\
  }

#define TEST_TYPE2(TYPE) 				\
  __attribute__((noipa))				\
  void vsqrt_##TYPE (TYPE *dst, TYPE *a, int n)		\
  {							\
    for (int i = 0; i < n; i++)				\
      dst[i] = __builtin_sqrt (a[i]);			\
  }

#define TEST_TYPE3(TYPE) 				\
  __attribute__((noipa))				\
  void vsqrt_##TYPE (TYPE *dst, TYPE *a, int n)		\
  {							\
    for (int i = 0; i < n; i++)				\
      dst[i] = __builtin_sqrtf16 (a[i]);		\
  }

#define TEST_ALL()					\
 TEST_TYPE(float)					\
 TEST_TYPE2(double)					\

TEST_ALL()

/* We cannot link this without the Zvfh extension so define
   it here instead of in the template directly (which is also
   included by the run test that might need to be linked without
   the extension).  */
TEST_TYPE3(_Float16)

/* { dg-final { scan-assembler-times {\tvfsqrt\.v} 3 } } */
