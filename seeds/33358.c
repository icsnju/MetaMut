/* { dg-do compile { target { lp64 && s390_vx } } } */
/* { dg-options "-march=native -O2 -ftree-vectorize -fno-vect-cost-model -fno-unroll-loops -fno-trapping-math" } */

/* { dg-additional-options "--param=vect-partial-vector-usage=2" } */

/* Test that the loop body uses vector load/store with length,
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

/* { dg-final { scan-assembler-times {\mvll\M} 20 } } */
/* { dg-final { scan-assembler-times {\mvstl\M} 10 } } */
