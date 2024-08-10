/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model --param=riscv-autovec-preference=fixed-vlmax" } */

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

#include <assert.h>

#define SZ 255

#define RUN(TYPE)				\
  TYPE a##TYPE[SZ];				\
  for (int i = 0; i < SZ; i++)			\
  {                             		\
    a##TYPE[i] = i - 127;             		\
  }                             		\
  vnot_##TYPE (a##TYPE, a##TYPE, SZ);	        \
  for (int i = 0; i < SZ; i++)			\
    assert (a##TYPE[i] == (TYPE)~(i - 127));

#define RUN2(TYPE)				\
  TYPE a##TYPE[SZ];				\
  for (int i = 0; i < SZ; i++)			\
  {                             		\
    a##TYPE[i] = i;             		\
  }                             		\
  vnot_##TYPE (a##TYPE, a##TYPE, SZ);	        \
  for (int i = 0; i < SZ; i++)			\
    assert (a##TYPE[i] == (TYPE)~i);

#define RUN_ALL()	                        \
 RUN(int8_t)	                                \
 RUN(int16_t)	                                \
 RUN(int32_t)	                                \
 RUN(int64_t)	                                \
 RUN(uint8_t)	                                \
 RUN(uint16_t)	                                \
 RUN(uint32_t)	                                \
 RUN(uint64_t)

int main ()
{
  RUN_ALL()
}
