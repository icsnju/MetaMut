/* { dg-do run { target { riscv_vector } } } */
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
 RUN(int8_t, -1)	\
 RUN(uint8_t, 2)	\
 RUN(int16_t, -1)	\
 RUN(uint16_t, 2)	\
 RUN(int32_t, -3)	\
 RUN(uint32_t, 4)	\
 RUN(int64_t, -5)	\
 RUN(uint64_t, 6)	\
 RUN(float, -5)		\
 RUN(double, 6)		\
 RUN2(int8_t, -7)	\
 RUN2(uint8_t, 8)	\
 RUN2(int16_t, -7)	\
 RUN2(uint16_t, 8)	\
 RUN2(int32_t, -9)	\
 RUN2(uint32_t, 10)	\
 RUN2(int64_t, -11)	\
 RUN2(uint64_t, 12)	\
 RUN2(float, -11)	\
 RUN2(double, 12)	\
 RUN3M(int8_t, 13)	\
 RUN3(uint8_t, 14)	\
 RUN3M(int16_t, 13)	\
 RUN3(uint16_t, 14)	\
 RUN3M(int32_t, 15)	\
 RUN3(uint32_t, 16)	\
 RUN3M(int64_t, 17)	\
 RUN3(uint64_t, 18)	\
 RUN3(float, 17)	\
 RUN3M(double, 18)	\

int main ()
{
  RUN_ALL()
}
