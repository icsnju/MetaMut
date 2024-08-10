/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param=riscv-autovec-preference=scalable -fno-vect-cost-model" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=scalable -fno-vect-cost-model" } */

#include <stdint-gcc.h>

#define DEF_LOOP(TYPE, WTYPE)                                                  \
  void __attribute__ ((noipa))                                                 \
  test_##TYPE##_##WTYPE (TYPE *__restrict a, TYPE *__restrict b,               \
			 TYPE *__restrict c, TYPE *__restrict pred, int n)     \
  {                                                                            \
    for (int i = 0; i < n; ++i)                                                \
      c[i] = pred[i]                                                           \
	       ? (TYPE) (((WTYPE) a[i] * (WTYPE) b[i]) >> sizeof (TYPE) * 8)   \
	       : c[i];                                                         \
  }

#define TEST_ALL(T)                                                            \
  T (int8_t, int16_t)                                                          \
  T (int16_t, int32_t)                                                         \
  T (int32_t, int64_t)                                                         \
  T (uint8_t, uint16_t)                                                        \
  T (uint16_t, uint32_t)                                                       \
  T (uint32_t, uint64_t)

TEST_ALL (DEF_LOOP)

/* { dg-final { scan-assembler-times {\tvmulh\.vv\tv[0-9]+,v[0-9]+,v[0-9]+,v0.t} 3 } } */
/* { dg-final { scan-assembler-times {\tvmulhu\.vv\tv[0-9]+,v[0-9]+,v[0-9]+,v0.t} 3 } } */
/* { dg-final { scan-assembler-not {\tvmerge\.vvm\t} } } */

#define N 99

#define TEST_LOOP(TYPE, WTYPE)                                                 \
  {                                                                            \
    TYPE a[N], b[N], c[N], pred[N];                                            \
    for (int i = 0; i < N; ++i)                                                \
      {                                                                        \
	a[i] = i * i;                                                          \
	b[i] = ((i + 2) % 3) * (i + 1);                                        \
	pred[i] = i % 3;                                                       \
	asm volatile ("" ::: "memory");                                        \
      }                                                                        \
    test_##TYPE##_##WTYPE (a, b, c, pred, N);                                  \
    for (int i = 0; i < N; ++i)                                                \
      if (c[i]                                                                 \
	  != (pred[i]                                                          \
		? (TYPE) (((WTYPE) a[i] * (WTYPE) b[i]) >> sizeof (TYPE) * 8)  \
		: c[i]))                                                       \
	__builtin_abort ();                                                    \
  }

int
main ()
{
  TEST_ALL (TEST_LOOP)
  return 0;
}
