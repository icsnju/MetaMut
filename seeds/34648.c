/* { dg-do run { target { lp64 && p9vector_hw } } } */
/* { dg-options "-mdejagnu-cpu=power9 -O2 -ftree-vectorize -fno-vect-cost-model" } */

/* { dg-additional-options "--param=vect-partial-vector-usage=2" } */

/* Check whether it runs successfully if we vectorize the loop fully
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

#define decl(TYPE)                                                             \
  TYPE a_##TYPE[N_##TYPE];                                                     \
  TYPE b_##TYPE[N_##TYPE];                                                     \
  TYPE c_##TYPE[N_##TYPE];

#define run(TYPE)                                                              \
  {                                                                            \
    unsigned int i = 0;                                                        \
    for (i = 0; i < N_##TYPE; i++)                                             \
      {                                                                        \
	a_##TYPE[i] = i * 2 + 1;                                               \
	b_##TYPE[i] = i % 2 - 2;                                               \
      }                                                                        \
    test##TYPE ();                                                             \
    for (i = 0; i < N_##TYPE; i++)                                             \
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

