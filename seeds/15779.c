/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param=riscv-autovec-preference=scalable -fno-vect-cost-model" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=scalable -fno-vect-cost-model" } */

#include <stdint-gcc.h>

#define DEF_LOOP(TYPE1, WTYPE1, TYPE2, WTYPE2, TYPE3)                          \
  void __attribute__ ((noipa))                                                 \
  test_##TYPE1##_##TYPE2 (TYPE1 *__restrict a, TYPE2 *__restrict b,            \
			  TYPE3 *__restrict c, TYPE3 *__restrict pred, int n)  \
  {                                                                            \
    for (int i = 0; i < n; ++i)                                                \
      c[i]                                                                     \
	= pred[i]                                                              \
	    ? (TYPE3) (((WTYPE1) a[i] * (WTYPE2) b[i]) >> sizeof (TYPE1) * 8)  \
	    : c[i];                                                            \
  }

#define TEST_ALL(T)                                                            \
  T (int8_t, int16_t, uint8_t, uint16_t, int8_t)                               \
  T (uint8_t, uint16_t, int8_t, int16_t, int8_t)                               \
  T (int16_t, int32_t, uint16_t, uint32_t, int16_t)                            \
  T (uint16_t, uint32_t, int16_t, int32_t, int16_t)                            \
  T (int32_t, int64_t, uint32_t, uint64_t, int32_t)                            \
  T (uint32_t, uint64_t, int32_t, int64_t, int32_t)

TEST_ALL (DEF_LOOP)

/* FIXME: need midend support: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=111381 */
/* { dg-final { scan-assembler-times {\tvmulhsu\.vv\tv[0-9]+,v[0-9]+,v[0-9]+,v0.t} 6 {xfail riscv*-*-*} } } */
/* { dg-final { scan-assembler-not {\tvmerge\.vvm\t} } } */

#define N 99

#define TEST_LOOP(TYPE1, WTYPE1, TYPE2, WTYPE2, TYPE3)                         \
  {                                                                            \
    TYPE1 a[N];                                                                \
    TYPE2 b[N];                                                                \
    TYPE3 c[N], pred[N];                                                       \
    for (int i = 0; i < N; ++i)                                                \
      {                                                                        \
	a[i] = i * i;                                                          \
	b[i] = ((i + 2) % 3) * (i + 1);                                        \
	pred[i] = i % 3;                                                       \
	asm volatile ("" ::: "memory");                                        \
      }                                                                        \
    test_##TYPE1##_##TYPE2 (a, b, c, pred, N);                                 \
    for (int i = 0; i < N; ++i)                                                \
      if (c[i]                                                                 \
	  != (pred[i] ? (TYPE3) (((WTYPE1) a[i] * (WTYPE2) b[i])               \
				 >> sizeof (TYPE1) * 8)                        \
		      : c[i]))                                                 \
	__builtin_abort ();                                                    \
  }

int
main ()
{
  TEST_ALL (TEST_LOOP)
  return 0;
}
