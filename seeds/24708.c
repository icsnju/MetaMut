/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param=riscv-autovec-preference=scalable" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv -mabi=ilp32d --param=riscv-autovec-preference=scalable -fno-vect-cost-model" } */

#include <stdint-gcc.h>

#define b_and(A, B) ((A) & (B))
#define b_orr(A, B) ((A) | (B))
#define b_xor(A, B) ((A) ^ (B))
#define b_nand(A, B) (!((A) & (B)))
#define b_nor(A, B) (!((A) | (B)))
#define b_xnor(A, B) (!(A) ^ (B))
#define b_andnot(A, B) ((A) & !(B))
#define b_ornot(A, B) ((A) | !(B))

#define LOOP(TYPE, BINOP)                                                      \
  void __attribute__ ((noinline, noclone))                                     \
  test_##TYPE##_##BINOP (TYPE *restrict dest, TYPE *restrict src,              \
			 TYPE *restrict a, TYPE *restrict b, TYPE *restrict c, \
			 TYPE *restrict d, TYPE fallback, int count)           \
  {                                                                            \
    for (int i = 0; i < count; ++i)                                            \
      {                                                                        \
	TYPE srcv = src[i];                                                    \
	dest[i] = (BINOP (__builtin_isunordered (a[i], b[i]),                  \
			  __builtin_isunordered (c[i], d[i]))                  \
		     ? srcv                                                    \
		     : fallback);                                              \
      }                                                                        \
  }

#define TEST_BINOP(T, BINOP)                                                   \
  T (float, BINOP)                                                             \
  T (double, BINOP)

#define TEST_ALL(T)                                                            \
  TEST_BINOP (T, b_and)                                                        \
  TEST_BINOP (T, b_orr)                                                        \
  TEST_BINOP (T, b_xor)                                                        \
  TEST_BINOP (T, b_nand)                                                       \
  TEST_BINOP (T, b_nor)                                                        \
  TEST_BINOP (T, b_xnor)                                                       \
  TEST_BINOP (T, b_andnot)                                                     \
  TEST_BINOP (T, b_ornot)

TEST_ALL (LOOP)

/* { dg-final { scan-assembler-times {\tvmand\.mm} 2 } } */
/* { dg-final { scan-assembler-times {\tvmor\.mm} 2 } } */
/* { dg-final { scan-assembler-times {\tvmxor\.mm} 2 } } */
/* { dg-final { scan-assembler-times {\tvmnot\.m} 4 } } */
/* { dg-final { scan-assembler-times {\tvmxnor\.mm} 2 } } */
/* { dg-final { scan-assembler-times {\tvmandn\.mm} 4 } } */
/* { dg-final { scan-assembler-times {\tvmorn\.mm} 4 } } */

#define N 401

#define RUN_LOOP(TYPE, BINOP)						\
  {									\
    TYPE dest[N], src[N], a[N], b[N], c[N], d[N];			\
    for (int i = 0; i < N; ++i)						\
      {									\
	src[i] = i * i;							\
	a[i] = i % 5 < 3 ? __builtin_nan("") : i;			\
	b[i] = i % 7 < 4 ? __builtin_nan("") : i;			\
	c[i] = i % 9 < 5 ? __builtin_nan("") : i;			\
	d[i] = i % 11 < 6 ? __builtin_nan("") : i;			\
	asm volatile ("" ::: "memory");					\
      }									\
    test_##TYPE##_##BINOP (dest, src, a, b, c, d, 100, N);		\
    for (int i = 0; i < N; ++i)						\
      {									\
	int res = BINOP (__builtin_isunordered (a[i], b[i]),		\
			 __builtin_isunordered (c[i], d[i]));		\
	if (dest[i] != (res ? src[i] : 100.0))				\
	  __builtin_abort ();						\
      }									\
  }

int __attribute__ ((optimize (1)))
main (void)
{
  TEST_ALL (RUN_LOOP)
  return 0;
}
