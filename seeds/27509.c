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


/* Check more to ensure vector access with out of bound.  */
#define N 155
/* Array size used for test function actually.  */
#define NF 127

#define run(TYPE1, TYPE2)                                                      \
  {                                                                            \
    unsigned int i = 0;                                                        \
    TYPE1 a[N * 2];                                                            \
    TYPE2 b[N * 2];                                                            \
    for (i = 0; i < N; i++)                                                    \
      {                                                                        \
	a[i * 2] = i * 2 + i % 3;                                              \
	a[i * 2 + 1] = i * 3 + i % 4;                                          \
	b[i * 2] = i * 7 + i / 5;                                              \
	b[i * 2 + 1] = i * 8 + i / 6;                                          \
      }                                                                        \
    test_mv_##TYPE1##TYPE2 (a, b, NF);                                         \
    for (i = 0; i < N; i++)                                                    \
      {                                                                        \
	TYPE1 exp_a1 = i * 2 + i % 3;                                          \
	TYPE1 exp_a2 = i * 3 + i % 4;                                          \
	TYPE2 exp_b1 = i * 7 + i / 5;                                          \
	TYPE2 exp_b2 = i * 8 + i / 6;                                          \
	if (i < NF)                                                            \
	  {                                                                    \
	    exp_a1 += 1;                                                        \
	    exp_a2 += 2;                                                       \
	    exp_b1 += 3;                                                       \
	    exp_b2 += 4;                                                       \
	  }                                                                    \
	if (a[i * 2] != exp_a1 || a[i * 2 + 1] != exp_a2 || b[i * 2] != exp_b1 \
	    || b[i * 2 + 1] != exp_b2)                                         \
	  __builtin_abort ();                                                  \
      }                                                                        \
  }

int
main (void)
{
  TEST_ALL2 (run)
  return 0;
}

