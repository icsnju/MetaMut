/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model --param=riscv-autovec-preference=fixed-vlmax -ffast-math" } */

#include <stdint-gcc.h>

#define TEST_TYPE(TYPE) 				\
  __attribute__((noipa))				\
  void vsub_##TYPE (TYPE *dst, TYPE *a, TYPE *b, int n)	\
  {							\
    for (int i = 0; i < n; i++)				\
      dst[i] = a[i] - b[i];				\
  }

#define TEST2_TYPE(TYPE) 				\
  __attribute__((noipa))				\
  void vsubs_##TYPE (TYPE *dst, TYPE *a, TYPE b, int n)	\
  {							\
    for (int i = 0; i < n; i++)				\
      dst[i] = a[i] - b;				\
  }

#define TEST3_TYPE(TYPE) 				\
  __attribute__((noipa))				\
  void vsubi_##TYPE (TYPE *dst, TYPE *a, int n)		\
  {							\
    for (int i = 0; i < n; i++)				\
      dst[i] = -16 - a[i];				\
  }

#define TEST4_TYPE(TYPE) 				\
  __attribute__((noipa))				\
  void vsubi2_##TYPE (TYPE *dst, TYPE *a, int n) 	\
  {							\
    for (int i = 0; i < n; i++)				\
      dst[i] = 15 - a[i];				\
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
 TEST2_TYPE(uint64_t)
 TEST2_TYPE(_Float16)	\
 TEST2_TYPE(float)	\
 TEST2_TYPE(double)	\

 TEST3_TYPE(int8_t)	\
 TEST3_TYPE(uint8_t)	\
 TEST3_TYPE(int16_t)	\
 TEST3_TYPE(uint16_t)	\
 TEST3_TYPE(int32_t)	\
 TEST3_TYPE(uint32_t)	\
 TEST3_TYPE(int64_t)	\
 TEST3_TYPE(uint64_t)	\
 TEST3_TYPE(_Float16)	\
 TEST3_TYPE(float)	\
 TEST3_TYPE(double)	\

 TEST4_TYPE(int8_t)	\
 TEST4_TYPE(uint8_t)	\
 TEST4_TYPE(int16_t)	\
 TEST4_TYPE(uint16_t)	\
 TEST4_TYPE(int32_t)	\
 TEST4_TYPE(uint32_t)	\
 TEST4_TYPE(int64_t)	\
 TEST4_TYPE(uint64_t)	\
 TEST4_TYPE(_Float16)	\
 TEST4_TYPE(float)	\
 TEST4_TYPE(double)	\

TEST_ALL()

#include <assert.h>

#define SZ 512

#define RUN(TYPE,VAL)				\
  TYPE a##TYPE[SZ];				\
  TYPE b##TYPE[SZ];	  			\
  for (int i = 0; i < SZ; i++)			\
  {                             		\
    a##TYPE[i] = 123;           		\
    b##TYPE[i] = VAL;           		\
  }                             		\
  vsub_##TYPE (a##TYPE, a##TYPE, b##TYPE, SZ);	\
  for (int i = 0; i < SZ; i++)			\
    assert (a##TYPE[i] == 123 - VAL);

#define RUN2(TYPE,VAL)				\
  TYPE as##TYPE[SZ];				\
  for (int i = 0; i < SZ; i++)			\
    as##TYPE[i] = 123;            		\
  vsubs_##TYPE (as##TYPE, as##TYPE, VAL, SZ);	\
  for (int i = 0; i < SZ; i++)			\
    assert (as##TYPE[i] == 123 - VAL);

#define RUN3(TYPE)				\
  TYPE as2##TYPE[SZ];				\
  for (int i = 0; i < SZ; i++)			\
    as2##TYPE[i] = i * 33 - 779;            	\
  vsubi_##TYPE (as2##TYPE, as2##TYPE, SZ);	\
  for (int i = 0; i < SZ; i++)			\
    assert (as2##TYPE[i] == (TYPE)(-16 - (i * 33 - 779)));

#define RUN4(TYPE)				\
  TYPE as3##TYPE[SZ];				\
  for (int i = 0; i < SZ; i++)			\
    as3##TYPE[i] = i * -17 + 667;            	\
  vsubi2_##TYPE (as3##TYPE, as3##TYPE, SZ);	\
  for (int i = 0; i < SZ; i++)			\
    assert (as3##TYPE[i] == (TYPE)(15 - (i * -17 + 667)));

#define RUN_ALL()	\
 RUN(int8_t, 1)	\
 RUN(uint8_t, 2)	\
 RUN(int16_t, 1)	\
 RUN(uint16_t, 2)	\
 RUN(int32_t, 3)	\
 RUN(uint32_t, 4)	\
 RUN(int64_t, 5)	\
 RUN(uint64_t, 6)	\
 RUN(float, 5)		\
 RUN(double, 6)		\
 RUN2(int8_t, 7)	\
 RUN2(uint8_t, 8)	\
 RUN2(int16_t, 7)	\
 RUN2(uint16_t, 8)	\
 RUN2(int32_t, 9)	\
 RUN2(uint32_t, 10)	\
 RUN2(int64_t, 11)	\
 RUN2(uint64_t, 12)	\
 RUN2(float, 11)	\
 RUN2(double, 12)	\
 RUN3(int8_t)		\
 RUN3(uint8_t)		\
 RUN3(int16_t)		\
 RUN3(uint16_t)		\
 RUN3(int32_t)		\
 RUN3(uint32_t)		\
 RUN3(int64_t)		\
 RUN3(uint64_t)		\
 RUN3(float)		\
 RUN3(double)		\
 RUN4(int8_t)		\
 RUN4(uint8_t)		\
 RUN4(int16_t)		\
 RUN4(uint16_t)		\
 RUN4(int32_t)		\
 RUN4(uint32_t)		\
 RUN4(int64_t)		\
 RUN4(uint64_t)		\
 RUN4(float)		\
 RUN4(double)		\

int main ()
{
  RUN_ALL()
}
