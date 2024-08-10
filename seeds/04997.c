/* { dg-do compile { target { lp64 && powerpc_p9vector_ok } } } */
/* { dg-options "-mdejagnu-cpu=power9 -O2 -ftree-vectorize -fno-vect-cost-model -fno-unroll-loops -fno-trapping-math" } */

/* { dg-additional-options "--param=vect-partial-vector-usage=1" } */

/* Test for that only vectorize the epilogue with vector access with length,
   the main body still use normal vector load/store.  */

#include <stdint.h>

#define TEST_ALL(T)                                                            \
  T (int8_t)                                                                   \
  T (uint8_t)                                                                  \
  T (int16_t)                                                                  \
  T (uint16_t)                                                                 \
  T (int32_t)                                                                  \
  T (uint32_t)                                                                 \
  T (int64_t)                                                                  \
  T (uint64_t)                                                                 \
  T (float)                                                                    \
  T (double)


/* Test the case that the loop which has multiple vectors (concatenated vectors)
   but with same vector type.  */

#define test(TYPE)                                                             \
  void __attribute__ ((noinline, noclone))                                     \
    test_mv_##TYPE (TYPE *restrict a, TYPE *restrict b, TYPE *restrict c,      \
		    int n)                                                     \
  {                                                                            \
    for (int i = 0; i < n; ++i)                                                \
      {                                                                        \
	a[i] += 1;                                                             \
	b[i * 2] += 2;                                                         \
	b[i * 2 + 1] += 3;                                                     \
	c[i * 4] += 4;                                                         \
	c[i * 4 + 1] += 5;                                                     \
	c[i * 4 + 2] += 6;                                                     \
	c[i * 4 + 3] += 7;                                                     \
      }                                                                        \
  }

TEST_ALL (test)


/* { dg-final { scan-assembler-times {\mlxvx?\M} 120 } } */
/* { dg-final { scan-assembler-times {\mstxvx?\M} 70 } } */
/* { dg-final { scan-assembler-times {\mlxvl\M} 70 } } */
/* { dg-final { scan-assembler-times {\mstxvl\M} 70 } } */
