/* { dg-do run { target { riscv_vector && riscv_zvfh_hw } } } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model --param=riscv-autovec-preference=fixed-vlmax -ffast-math" } */

#include <stdint-gcc.h>

#define TEST_TYPE(TYPE) 				\
  __attribute__((noipa))				\
  void vadd_##TYPE (TYPE *dst, TYPE *a, TYPE *b, int n)	\
  {							\
    for (int i = 0; i < n; i++)				\
      dst[i] = a[i] + b[i];				\
  }

#define TEST2_TYPE(TYPE) 				\
  __attribute__((noipa))				\
  void vadds_##TYPE (TYPE *dst, TYPE *a, TYPE b, int n)	\
  {							\
    for (int i = 0; i < n; i++)				\
      dst[i] = a[i] + b;				\
  }

#define TEST3_TYPE(TYPE) 				\
  __attribute__((noipa))				\
  void vaddi_##TYPE (TYPE *dst, TYPE *a, int n)	        \
  {							\
    for (int i = 0; i < n; i++)				\
      dst[i] = a[i] + 15;				\
  }

#define TEST3M_TYPE(TYPE) 				\
  __attribute__((noipa))				\
  void vaddim_##TYPE (TYPE *dst, TYPE *a, int n)	\
  {							\
    for (int i = 0; i < n; i++)				\
      dst[i] = a[i] - 16;				\
  }

#define TEST_ALL()	\
 TEST_TYPE(int8_t)	\
 TEST_TYPE(uint8_t)	\
 TEST_TYPE(int16_t)	\
 TEST_TYPE(uint16_t)	\
 TEST_TYPE(int32_t)	\
 TEST_TYPE(uint32_t)	\
 TEST_TYPE(int64_t)	\
 TEST_TYPE(uint64_t)    \
 TEST_TYPE(_Float16)	\
 TEST_TYPE(float)	\
 TEST_TYPE(double)	\
 TEST2_TYPE(int8_t)	\
 TEST2_TYPE(uint8_t)	\
 TEST2_TYPE(int16_t)	\
 TEST2_TYPE(uint16_t)	\
 TEST2_TYPE(int32_t)	\
 TEST2_TYPE(uint32_t)	\
 TEST2_TYPE(int64_t)	\
 TEST2_TYPE(uint64_t)   \
 TEST2_TYPE(_Float16)	\
 TEST2_TYPE(float)	\
 TEST2_TYPE(double)	\
 TEST3M_TYPE(int8_t)	\
 TEST3_TYPE(uint8_t)	\
 TEST3M_TYPE(int16_t)	\
 TEST3_TYPE(uint16_t)	\
 TEST3M_TYPE(int32_t)	\
 TEST3_TYPE(uint32_t)	\
 TEST3M_TYPE(int64_t)	\
 TEST3_TYPE(uint64_t)   \
 TEST3M_TYPE(_Float16)	\
 TEST3_TYPE(float)	\
 TEST3M_TYPE(double)	\

TEST_ALL()

#include <assert.h>

#define SZ 512

#define RUN(TYPE,VAL)				\
  TYPE a##TYPE[SZ];				\
  TYPE b##TYPE[SZ];	  			\
  for (int i = 0; i < SZ; i++)			\
  {                             		\
    a##TYPE[i] = 0;             		\
    b##TYPE[i] = VAL;           		\
  }                             		\
  vadd_##TYPE (a##TYPE, a##TYPE, b##TYPE, SZ);	\
  for (int i = 0; i < SZ; i++)			\
    assert (a##TYPE[i] == VAL);

#define RUN2(TYPE,VAL)				\
  TYPE as##TYPE[SZ];				\
  for (int i = 0; i < SZ; i++)			\
    as##TYPE[i] = 0;            		\
  vadds_##TYPE (as##TYPE, as##TYPE, VAL, SZ);	\
  for (int i = 0; i < SZ; i++)			\
    assert (as##TYPE[i] == VAL);

#define RUN3(TYPE,VAL)				\
  TYPE ai##TYPE[SZ];	  	        	\
  for (int i = 0; i < SZ; i++)			\
    ai##TYPE[i] = VAL;				\
  vaddi_##TYPE (ai##TYPE, ai##TYPE, SZ);	\
  for (int i = 0; i < SZ; i++)			\
    assert (ai##TYPE[i] == VAL + 15);

#define RUN3M(TYPE,VAL)				\
  TYPE aim##TYPE[SZ];				\
  for (int i = 0; i < SZ; i++)			\
    aim##TYPE[i] = VAL;				\
  vaddim_##TYPE (aim##TYPE, aim##TYPE, SZ);	\
  for (int i = 0; i < SZ; i++)			\
    assert (aim##TYPE[i] == VAL - 16);

#define RUN_ALL()	\
 RUN(_Float16, 4)	\
 RUN2(_Float16, 10)	\
 RUN3M(_Float16, 17)	\

int main ()
{
  RUN_ALL()
}
