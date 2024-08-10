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


/* Test the case loop iteration is unknown.  */
#define N 255

#define test(TYPE)                                                             \
  extern TYPE a_##TYPE[N];                                                     \
  extern TYPE b_##TYPE[N];                                                     \
  extern TYPE c_##TYPE[N];                                                     \
  void __attribute__ ((noinline, noclone)) test##TYPE (unsigned int n)         \
  {                                                                            \
    unsigned int i = 0;                                                        \
    for (i = 0; i < n; i++)                                                    \
      c_##TYPE[i] = a_##TYPE[i] + b_##TYPE[i];                                 \
  }

TEST_ALL (test)

/* { dg-final { scan-assembler-not   {\mlxv\M} } } */
/* { dg-final { scan-assembler-not   {\mstxv\M} } } */
/* { dg-final { scan-assembler-not   {\mlxvx\M} } } */
/* { dg-final { scan-assembler-not   {\mstxvx\M} } } */
/* { dg-final { scan-assembler-times {\mlxvl\M} 20 } } */
/* { dg-final { scan-assembler-times {\mstxvl\M} 10 } } */
