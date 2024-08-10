/* { dg-do compile { target { lp64 && powerpc_p9vector_ok } } } */
/* { dg-options "-mdejagnu-cpu=power9 -O2 -ftree-vectorize -fno-vect-cost-model -fno-unroll-loops -fno-trapping-math" } */

/* { dg-additional-options "--param=vect-partial-vector-usage=2" } */

/* Test for fully with length, the loop body uses vector access with length,
   there should not be any epilogues.  */

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
   with different types.  */

#define test(TYPE1, TYPE2)                                                     \
  void __attribute__ ((noinline, noclone))                                     \
    test_mv_##TYPE1##TYPE2 (TYPE1 *restrict a, TYPE2 *restrict b, int n)       \
  {                                                                            \
    for (int i = 0; i < n; ++i)                                                \
      {                                                                        \
	a[i * 2] += 1;                                                         \
	a[i * 2 + 1] += 2;                                                     \
	b[i * 2] += 3;                                                         \
	b[i * 2 + 1] += 4;                                                     \
      }                                                                        \
  }

#define TEST_ALL2(T)                                                           \
  T (int8_t, uint16_t)                                                         \
  T (uint8_t, int16_t)                                                         \
  T (int16_t, uint32_t)                                                        \
  T (uint16_t, int32_t)                                                        \
  T (int32_t, double)                                                          \
  T (uint32_t, int64_t)                                                        \
  T (float, uint64_t)

TEST_ALL2 (test)


/* It can use normal vector load for constant vector load.  */
/* { dg-final { scan-assembler-not   {\mstxv\M} } } */
/* { dg-final { scan-assembler-not   {\mlxvx\M} } } */
/* { dg-final { scan-assembler-not   {\mstxvx\M} } } */
/* { dg-final { scan-assembler-times {\mlxvl\M} 21 } } */
/* { dg-final { scan-assembler-times {\mstxvl\M} 21 } } */
