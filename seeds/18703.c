/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param=riscv-autovec-preference=scalable -fno-vect-cost-model" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=scalable -fno-vect-cost-model" } */

#include <stdint-gcc.h>

#define DEF_LOOP(TYPE, NAME, CONST)			\
  void __attribute__ ((noipa))				\
  test_##TYPE##_##NAME (TYPE *__restrict x,		\
			TYPE *__restrict y,		\
			TYPE *__restrict z,		\
			int n)				\
  {							\
    for (int i = 0; i < n; ++i)				\
      x[i] = y[i] < 8 ? z[i] + (TYPE) CONST : y[i];	\
  }

#define TEST_TYPE(T, TYPE) \
  T (TYPE, half, 0.5) \
  T (TYPE, one, 1.0) \
  T (TYPE, two, 2.0) \
  T (TYPE, minus_half, -0.5) \
  T (TYPE, minus_one, -1.0) \
  T (TYPE, minus_two, -2.0)

#define TEST_ALL(T) \
  TEST_TYPE (T, float) \
  TEST_TYPE (T, double)

TEST_ALL (DEF_LOOP)

/* { dg-final { scan-assembler-times {vfadd\.vv\s+v[0-9]+,v[0-9]+,v[0-9]+,v0.t} 12 } } */

#define N 99

#define TEST_LOOP(TYPE, NAME, CONST)					\
  {									\
    TYPE x[N], y[N], z[N];						\
    for (int i = 0; i < N; ++i)						\
      {									\
	y[i] = i % 13;							\
	z[i] = i * i;							\
      }									\
    test_##TYPE##_##NAME (x, y, z, N);					\
    for (int i = 0; i < N; ++i)						\
      {									\
	TYPE expected = y[i] < 8 ? z[i] + (TYPE) CONST : y[i];		\
	if (x[i] != expected)						\
	  __builtin_abort ();						\
	asm volatile ("" ::: "memory");					\
      }									\
  }

int
main (void)
{
  TEST_ALL (TEST_LOOP)
  return 0;
}
