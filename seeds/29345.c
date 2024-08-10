/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param=riscv-autovec-preference=scalable -fno-vect-cost-model" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=scalable -fno-vect-cost-model" } */

#include <stdint-gcc.h>

#define DEF_LOOP(TYPE, PRED_TYPE, NAME, CONST)		\
  void __attribute__ ((noipa))				\
  test_##TYPE##_##NAME (TYPE *__restrict x,		\
			TYPE *__restrict y,		\
			PRED_TYPE *__restrict pred,	\
			int n)				\
  {							\
    for (int i = 0; i < n; ++i)				\
      x[i] = pred[i] != 1 ? y[i] * (TYPE) CONST : 0;	\
  }

#define TEST_TYPE(T, TYPE, PRED_TYPE) \
  T (TYPE, PRED_TYPE, half, 0.5) \
  T (TYPE, PRED_TYPE, two, 2.0) \
  T (TYPE, PRED_TYPE, four, 4.0)

#define TEST_ALL(T) \
  TEST_TYPE (T, _Float16, int16_t) \
  TEST_TYPE (T, float, int32_t) \
  TEST_TYPE (T, double, int64_t)

TEST_ALL (DEF_LOOP)

/* { dg-final { scan-assembler-times {vfmul\.vv\s+v[0-9]+,v[0-9]+,v[0-9]+,v0.t} 9 } } */

#define N 99

#define TEST_LOOP(TYPE, PRED_TYPE, NAME, CONST)				\
  {									\
    TYPE x[N], y[N];							\
    PRED_TYPE pred[N];							\
    for (int i = 0; i < N; ++i)						\
      {									\
	y[i] = i * i;							\
	pred[i] = i % 3;						\
      }									\
    test_##TYPE##_##NAME (x, y, pred, N);				\
    for (int i = 0; i < N; ++i)						\
      {									\
	TYPE expected = i % 3 != 1 ? y[i] * (TYPE) CONST : 0;		\
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