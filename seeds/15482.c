/* { dg-do run { target { lp64 && p9vector_hw } } } */
/* { dg-options "-mdejagnu-cpu=power9 -O2 -ftree-vectorize -fno-vect-cost-model" } */

/* { dg-additional-options "--param=vect-partial-vector-usage=1" } */

/* Check whether it runs successfully if we only vectorize the epilogue
   with vector access with length.  */

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

#define run(TYPE)                                                              \
  {                                                                            \
    unsigned int i = 0;                                                        \
                                                                               \
    TYPE out_##TYPE[N];                                                        \
    TYPE in_##TYPE[N * 2];                                                     \
    for (int i = 0; i < N; ++i) {                                              \
      out_##TYPE[i] = i * 7 / 2;                                               \
    }                                                                          \
    for (int i = 0; i < N * 2; ++i) {                                          \
      in_##TYPE[i] = i * 9 / 2;                                                \
    }                                                                          \
                                                                               \
    test_##TYPE(out_##TYPE, in_##TYPE);                                        \
    for (int i = 0; i < N; ++i) {                                              \
      TYPE expected = i * 7 / 2 + in_##TYPE[i * 2];                            \
      if (out_##TYPE[i] != expected)                                           \
        __builtin_abort();                                                     \
    }                                                                          \
  }

int main(void) {
  TEST_ALL(run)
  return 0;
}

