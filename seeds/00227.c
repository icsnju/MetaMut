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


/* Test the case that the loop requires to peel for gaps.  */

#define N 200

#define test(TYPE)                                                             \
  void __attribute__((noinline, noclone))                                      \
      test_##TYPE(TYPE *restrict dest, TYPE *restrict src) {                   \
    for (unsigned int i = 0; i < N; ++i)                                       \
      dest[i] += src[i * 2];                                                   \
  }

TEST_ALL(test)

/* { dg-final { scan-assembler-times {\mlxvl\M} 16 } } */
/* { dg-final { scan-assembler-times {\mstxvl\M} 7 } } */
