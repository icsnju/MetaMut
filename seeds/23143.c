/* { dg-do compile { target { lp64 && s390_vx } } } */
/* { dg-options "-march=native -O2 -ftree-vectorize -fno-vect-cost-model -fno-unroll-loops -ffast-math -fno-trapping-math" } */

/* { dg-additional-options "--param=vect-partial-vector-usage=1 --param=min-vect-loop-bound=0" } */

/* Test that we only vectorize the epilogue with vector load/store with length,
   the main body still uses normal vector load/store.  */

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

/* { dg-final { scan-assembler-times {\mvstl\M} 4 } } */
