/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model --param=riscv-autovec-preference=fixed-vlmax" } */

#include <stdint-gcc.h>

#define TEST(TYPE1, TYPE2)					\
  __attribute__((noipa))					\
  void vfncvt_##TYPE1##TYPE2 (TYPE2 *dst, TYPE1 *a, int n)	\
  {								\
    for (int i = 0; i < n; i++)					\
      dst[i] = (TYPE2)a[i];					\
  }

#define TEST_ALL()						\
 TEST(float, _Float16)						\
 TEST(double, _Float16)						\
 TEST(double, float)						\

TEST_ALL()

#include <assert.h>

#define SZ 512
#define EPS 1e-4

#define RUN(TYPE1,TYPE2)				\
  TYPE1 src##TYPE1##TYPE2[SZ];				\
  TYPE2 dst##TYPE1##TYPE2[SZ];				\
  for (int i = 0; i < SZ; i++)				\
  {                             			\
    src##TYPE1##TYPE2[i] = (i & 1) ? -i : i;  		\
    src##TYPE1##TYPE2[i] *= 3.141592;			\
    dst##TYPE1##TYPE2[i] = -1;				\
  }                             			\
  vfncvt_##TYPE1##TYPE2 (dst##TYPE1##TYPE2,		\
			src##TYPE1##TYPE2, SZ);		\
  for (int i = 0; i < SZ; i++)				\
    assert (__builtin_fabs (dst##TYPE1##TYPE2[i]	\
	    - ((i & 1) ? -i : i) * 3.141592) < EPS);	\


#define RUN_ALL()					\
 RUN(double, float)					\

int main ()
{
  RUN_ALL()
}