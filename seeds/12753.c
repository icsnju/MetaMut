/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param=riscv-autovec-preference=scalable -fno-vect-cost-model" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=scalable -fno-vect-cost-model" } */

#include <stdint-gcc.h>

#define DEF_LOOP(TYPE, NAME, OP)					\
  void __attribute__ ((noipa))						\
  test_##TYPE##_##NAME (TYPE *__restrict r, TYPE *__restrict a,		\
			TYPE *__restrict b, int n)			\
  {									\
    for (int i = 0; i < n; ++i)						\
      r[i] = a[i] > 20 ? b[i] OP 3 : a[i];				\
  }

#define TEST_TYPE(T, TYPE) \
  T (TYPE, shl, <<) \
  T (TYPE, shr, >>)

#define TEST_ALL(T) \
  TEST_TYPE (T, int8_t) \
  TEST_TYPE (T, uint8_t) \
  TEST_TYPE (T, int16_t) \
  TEST_TYPE (T, uint16_t) \
  TEST_TYPE (T, int32_t) \
  TEST_TYPE (T, uint32_t) \
  TEST_TYPE (T, int64_t) \
  TEST_TYPE (T, uint64_t)

TEST_ALL (DEF_LOOP)

/* { dg-final { scan-assembler-times {vsll\.vi\s+v[0-9]+,v[0-9]+,3,v0.t} 8 } } */
/* { dg-final { scan-assembler-times {vsra\.vi\s+v[0-9]+,v[0-9]+,3,v0.t} 4 } } */
/* { dg-final { scan-assembler-times {vsrl\.vi\s+v[0-9]+,v[0-9]+,3,v0.t} 4 } } */

#define N 99

#define TEST_LOOP(TYPE, NAME, OP)				\
  {								\
    TYPE r[N], a[N], b[N];					\
    for (int i = 0; i < N; ++i)					\
      {								\
	a[i] = (i & 1 ? i : 3 * i);				\
	b[i] = (i >> 4) << (i & 15);				\
	asm volatile ("" ::: "memory");				\
      }								\
    test_##TYPE##_##NAME (r, a, b, N);				\
    for (int i = 0; i < N; ++i)					\
      if (r[i] != (TYPE) (a[i] > 20 ? b[i] OP 3 : a[i]))	\
	__builtin_abort ();					\
  }

int main ()
{
  TEST_ALL (TEST_LOOP)
  return 0;
}

