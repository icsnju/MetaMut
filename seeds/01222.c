/* { dg-do run { target { riscv_zvfh_hw } } } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model --param=riscv-autovec-preference=fixed-vlmax -ffast-math" } */

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
   it here instead of in the template directly.  */
TEST_TYPE3(_Float16)

#include <assert.h>

#define SZ 255

#define EPS 1e-5

#define RUN(TYPE)						\
  TYPE a##TYPE[SZ];						\
  for (int i = 0; i < SZ; i++)				    	\
  {                             			    	\
    a##TYPE[i] = (TYPE)i;             			    	\
  }                             			    	\
  vsqrt_##TYPE (a##TYPE, a##TYPE, SZ);	        	    	\
  for (int i = 0; i < SZ; i++)				    	\
    assert (__builtin_fabs				    	\
	    (a##TYPE[i] - __builtin_sqrtf ((TYPE)i)) < EPS);	\

#define RUN_ALL()						\
 RUN(_Float16)	                                	    	\

int main ()
{
  RUN_ALL()
}
