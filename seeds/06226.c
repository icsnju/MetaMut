/* { dg-do compile { target { lp64 && powerpc_p9vector_ok } } } */
/* Pass cunroll isn't disabled by -fno-unroll-loops, so use explicit
   disabling option for it.  */
/* { dg-options "-mdejagnu-cpu=power9 -O2 -ftree-vectorize -fno-vect-cost-model -fno-unroll-loops -ffast-math -fdisable-tree-cunroll" } */

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


/* Test the case that the loop requires to have peeled prologues for
   alignment.  */

#define N 64
#define START 1
#define END 59

#define test(TYPE)                                                             \
  TYPE x_##TYPE[N] __attribute__((aligned(16)));                                \
  void __attribute__((noinline, noclone)) test_npeel_##TYPE() {                \
    TYPE v = 0;                                                                \
    for (unsigned int i = START; i < END; i++) {                               \
      x_##TYPE[i] = v;                                                         \
      v += 1;                                                                  \
    }                                                                          \
  }

TEST_ALL (test)

/* Each type has one stxvl excepting for int8 and uint8, that have two due to
   rtl pass bbro duplicating the block which has one stxvl.  */
/* { dg-final { scan-assembler-times {\mstxvl\M} 12 } } */
