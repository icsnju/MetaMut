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


/* Test the case loop iteration less than VF.  */

/* For char.  */
#define N_uint8_t 15
#define N_int8_t 15
/* For short.  */
#define N_uint16_t 6
#define N_int16_t 6
/* For int/float.  */
#define N_uint32_t 3
#define N_int32_t 3
#define N_float 3
/* For long/double.  */
#define N_uint64_t 1
#define N_int64_t 1
#define N_double 1

#define test(TYPE)                                                             \
  extern TYPE a_##TYPE[N_##TYPE];                                              \
  extern TYPE b_##TYPE[N_##TYPE];                                              \
  extern TYPE c_##TYPE[N_##TYPE];                                              \
  void __attribute__ ((noinline, noclone)) test##TYPE ()                       \
  {                                                                            \
    unsigned int i = 0;                                                        \
    for (i = 0; i < N_##TYPE; i++)                                             \
      c_##TYPE[i] = a_##TYPE[i] + b_##TYPE[i];                                 \
  }

TEST_ALL (test)

/* { dg-final { scan-assembler-not   {\mlxv\M} } } */
/* { dg-final { scan-assembler-not   {\mstxv\M} } } */
/* { dg-final { scan-assembler-not   {\mlxvx\M} } } */
/* { dg-final { scan-assembler-not   {\mstxvx\M} } } */
/* 64bit types get completely unrolled, so only check the others.  */
/* { dg-final { scan-assembler-times {\mlxvl\M} 14 } } */
/* { dg-final { scan-assembler-times {\mstxvl\M} 7 } } */
