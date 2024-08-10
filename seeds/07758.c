/* { dg-do run { target { lp64 && s390_vx } } } */
/* { dg-options "-march=native -O2 -ftree-vectorize -fno-vect-cost-model -fno-trapping-math" } */

/* { dg-additional-options "--param=vect-partial-vector-usage=1" } */

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

#define decl(TYPE)                                                             \
  TYPE a_##TYPE[N];                                                            \
  TYPE b_##TYPE[N];                                                            \
  TYPE c_##TYPE[N];

#define N1 195

#define run(TYPE)                                                              \
  {                                                                            \
    unsigned int i = 0;                                                        \
    for (i = 0; i < N; i++)                                                    \
      {                                                                        \
	a_##TYPE[i] = i * 2 + 1;                                               \
	b_##TYPE[i] = i % 2 - 2;                                               \
      }                                                                        \
    test##TYPE (N1);                                                           \
    for (i = 0; i < N1; i++)                                                   \
      {                                                                        \
	TYPE a1 = i * 2 + 1;                                                   \
	TYPE b1 = i % 2 - 2;                                                   \
	TYPE exp_c = a1 + b1;                                                  \
	if (c_##TYPE[i] != exp_c)                                              \
	  __builtin_abort ();                                                  \
      }                                                                        \
  }

TEST_ALL (decl)

int
main (void)
{
  TEST_ALL (run)
  return 0;
}

