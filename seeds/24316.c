/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param=riscv-autovec-preference=scalable -fno-vect-cost-model" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=scalable -fno-vect-cost-model" } */

#include <stdint-gcc.h>

#define DEF_LOOP(TYPE1, TYPE2)                                                 \
  void __attribute__ ((noipa))                                                 \
  test_##TYPE1##_##TYPE2 (TYPE2 *__restrict r, TYPE2 *__restrict a,            \
			  TYPE1 *__restrict b, TYPE2 shift, int n)             \
  {                                                                            \
    for (int i = 0; i < n; ++i)                                                \
      r[i] = a[i] > 20 ? (TYPE2) (b[i] >> shift) : r[i];                       \
  }

#define TEST_ALL(T)                                                            \
  T (int16_t, int8_t)                                                          \
  T (int32_t, int16_t)                                                         \
  T (int64_t, int32_t)                                                         \
  T (uint16_t, uint8_t)                                                        \
  T (uint32_t, uint16_t)                                                       \
  T (uint64_t, uint32_t)

TEST_ALL (DEF_LOOP)

/* For int8_t and uint8_t, they are extended to int32_t and then shifted, so
   int8_t and uint8_t are use vnsra.wx and vnsra.wx's dest is used not by vmerge
   but by vncvt, so vncvt + vmerge will combine to a mask operator. */
/* { dg-final { scan-assembler-times {vnsrl\.wx\t} 2 } } */
/* { dg-final { scan-assembler-times {vnsra\.wx\t} 4 } } */
/* { dg-final { scan-assembler-not {\tvmerge\.vvm\t} } } */

#define N 99

#define TEST_LOOP(TYPE1, TYPE2)                                                \
  {                                                                            \
    TYPE2 r[N], a[N];                                                          \
    TYPE2 shift = 5;                                                           \
    TYPE1 b[N];                                                                \
    for (int i = 0; i < N; ++i)                                                \
      {                                                                        \
	a[i] = (i & 1 ? i : 3 * i);                                            \
	b[i] = (i >> 4) << (i & 15);                                           \
	asm volatile ("" ::: "memory");                                        \
      }                                                                        \
    test_##TYPE1##_##TYPE2 (r, a, b, shift, N);                                \
    for (int i = 0; i < N; ++i)                                                \
      if (r[i] != (a[i] > 20 ? (TYPE2) (b[i] >> shift) : r[i]))                \
	__builtin_abort ();                                                    \
  }

int
main ()
{
  TEST_ALL (TEST_LOOP)
  return 0;
}
