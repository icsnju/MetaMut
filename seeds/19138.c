/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model --param=riscv-autovec-preference=fixed-vlmax" } */

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

#include <assert.h>

#define SZ 256

#define RUN(TYPE1,TYPE2)				\
  TYPE1 src##TYPE1##TYPE2[SZ];				\
  TYPE2 dst##TYPE1##TYPE2[SZ];				\
  for (int i = 0; i < SZ; i++)				\
  {                             			\
    src##TYPE1##TYPE2[i] = i;             		\
    dst##TYPE1##TYPE2[i] = -1;				\
  }                             			\
  vzext_##TYPE1##TYPE2 (dst##TYPE1##TYPE2,		\
			src##TYPE1##TYPE2, SZ);		\
  for (int i = 0; i < SZ; i++)				\
    assert (dst##TYPE1##TYPE2[i] == i);


#define RUN_ALL()					\
 RUN(uint8_t, uint16_t)					\
 RUN(uint8_t, uint32_t)					\
 RUN(uint8_t, uint64_t)					\
 RUN(uint16_t, uint32_t)				\
 RUN(uint16_t, uint64_t)				\
 RUN(uint32_t, uint64_t)				\

int main ()
{
  RUN_ALL()
}
