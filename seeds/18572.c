/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param=riscv-autovec-preference=scalable -fno-vect-cost-model -ffast-math" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=scalable -fno-vect-cost-model -ffast-math" } */

#include <stdint-gcc.h>

#define DEF_LOOP(TYPE, NAME, OP)			\
  void __attribute__ ((noipa))				\
  test_##TYPE##_##NAME (TYPE *__restrict r,		\
			TYPE *__restrict a,		\
			TYPE *__restrict b, TYPE c,	\
			TYPE *__restrict pred, int n)	\
  {							\
    for (int i = 0; i < n; ++i)				\
      r[i] = pred[i] != 1 ? a[i] OP b[i] * c : a[i];	\
  }

#define TEST_TYPE(T, TYPE) \
  T (TYPE, add, +) \
  T (TYPE, sub, -)

#define TEST_ALL(T) \
  TEST_TYPE (T, uint8_t) \
  TEST_TYPE (T, uint16_t) \
  TEST_TYPE (T, uint32_t) \
  TEST_TYPE (T, uint64_t) \
  TEST_TYPE (T, _Float16) \
  TEST_TYPE (T, float) \
  TEST_TYPE (T, double)

TEST_ALL (DEF_LOOP)

/* { dg-final { scan-assembler-times {vmacc\.vv\s+v[0-9]+,v[0-9]+,v[0-9]+,v0.t} 4 } } */
/* { dg-final { scan-assembler-times {vnmsac\.vv\s+v[0-9]+,v[0-9]+,v[0-9]+,v0.t} 4 } } */
/* { dg-final { scan-assembler-times {vfmacc\.vv\s+v[0-9]+,v[0-9]+,v[0-9]+,v0.t} 3 } } */
/* { dg-final { scan-assembler-times {vfnmsac\.vv\s+v[0-9]+,v[0-9]+,v[0-9]+,v0.t} 3 } } */

#define FACTOR 17
#define N 99

#define TEST_LOOP(TYPE, NAME, OP)				\
  {								\
    TYPE r[N], a[N], b[N], pred[N];				\
    for (int i = 0; i < N; ++i)					\
      {								\
	a[i] = (i & 1 ? i : 3 * i);				\
	b[i] = (i >> 4) << (i & 15);				\
	pred[i] = i % 3 < i % 5;				\
	asm volatile ("" ::: "memory");				\
      }								\
    test_##TYPE##_##NAME (r, a, b, FACTOR, pred, N);		\
    for (int i = 0; i < N; ++i)					\
      {								\
	TYPE expected						\
	  = pred[i] != 1 ? a[i] OP b[i] * (TYPE) FACTOR : a[i];	\
	if (r[i] != expected)					\
	  __builtin_abort ();					\
	asm volatile ("" ::: "memory");				\
      }								\
  }

int
main (void)
{
  TEST_ALL (TEST_LOOP)
  return 0;
}
