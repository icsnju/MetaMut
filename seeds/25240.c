/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model --param=riscv-autovec-preference=fixed-vlmax -ffast-math" } */

#include <stdlib.h>
#include <stdint-gcc.h>

#define TEST_TYPE(TYPE) 				\
  __attribute__((noipa))				\
  void vneg_##TYPE (TYPE *dst, TYPE *a, int n)		\
  {							\
    for (int i = 0; i < n; i++)				\
      dst[i] =  -a[i];					\
  }

#define TEST_ALL()					\
 TEST_TYPE(int8_t)					\
 TEST_TYPE(int16_t)					\
 TEST_TYPE(int32_t)					\
 TEST_TYPE(int64_t)					\
 TEST_TYPE(_Float16)					\
 TEST_TYPE(float)					\
 TEST_TYPE(double)					\

TEST_ALL()

#include <assert.h>

#define SZ 255

#define RUN(TYPE)				\
  TYPE a##TYPE[SZ];				\
  for (int i = 0; i < SZ; i++)			\
  {                             		\
    a##TYPE[i] = i - 127;             		\
  }                             		\
  vneg_##TYPE (a##TYPE, a##TYPE, SZ);		\
  for (int i = 0; i < SZ; i++)			\
    assert (a##TYPE[i] == -(i - 127));

#define RUN_ALL()	                        \
 RUN(int8_t)	                                \
 RUN(int16_t)	                                \
 RUN(int32_t)	                                \
 RUN(int64_t)					\
 RUN(float)	                                \
 RUN(double)					\

int main ()
{
  RUN_ALL()
}