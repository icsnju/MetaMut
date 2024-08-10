/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model --param=riscv-autovec-preference=fixed-vlmax" } */

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

#include <assert.h>

#define SZ 256

#define RUN(TYPE1,TYPE2)				\
  TYPE1 src##TYPE1##TYPE2[SZ];				\
  TYPE2 dst##TYPE1##TYPE2[SZ];				\
  for (int i = 0; i < SZ; i++)				\
  {                             			\
    src##TYPE1##TYPE2[i] = i - 128;            		\
    dst##TYPE1##TYPE2[i] = 0;				\
  }                             			\
  vsext_##TYPE1##TYPE2 (dst##TYPE1##TYPE2,		\
			src##TYPE1##TYPE2, SZ);		\
  for (int i = 0; i < SZ; i++)				\
    assert (dst##TYPE1##TYPE2[i] == i - 128);


#define RUN_ALL()					\
 RUN(int8_t, int16_t)					\
 RUN(int8_t, int32_t)					\
 RUN(int8_t, int64_t)					\
 RUN(int16_t, int32_t)					\
 RUN(int16_t, int64_t)					\
 RUN(int32_t, int64_t)					\

int main ()
{
  RUN_ALL()
}
