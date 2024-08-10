/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param riscv-autovec-preference=scalable -fno-vect-cost-model" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv -mabi=ilp32d --param riscv-autovec-preference=scalable -fno-vect-cost-model" } */

#include <stdint-gcc.h>

#define N1 (19 * 2)

#define VEC_PERM(TYPE)                                                         \
  void __attribute__ ((noinline, noclone))                                     \
  vec_slp_##TYPE (TYPE *restrict a, TYPE *restrict b)                          \
  {                                                                            \
    for (int i = 0; i < N1; ++i)                                               \
      {                                                                        \
	a[i] += 1;                                                             \
	b[i * 4] += 2;                                                         \
	b[i * 4 + 1] += 3;                                                     \
	b[i * 4 + 2] += 4;                                                     \
	b[i * 4 + 3] += 5;                                                     \
      }                                                                        \
  }

#define TEST_ALL(T)                                                            \
  T (int8_t)                                                                   \
  T (uint8_t)                                                                  \
  T (int16_t)                                                                  \
  T (uint16_t)                                                                 \
  T (int32_t)                                                                  \
  T (uint32_t)                                                                 \
  T (int64_t)                                                                  \
  T (uint64_t)

TEST_ALL (VEC_PERM)

/* This testcase is from aarch64 and floating-point operations are removed.
   TODO: We will add floating-point operations back and make them as common test in the future.  */

#define N2 (31 * 2)

#define HARNESS(TYPE)						\
  {								\
    TYPE a[N2], b[N2 * 4];					\
    for (unsigned int i = 0; i < N2; ++i)			\
      {								\
	a[i] = i * 2 + i % 5;					\
	b[i * 4] = i * 3 + i % 7;				\
	b[i * 4 + 1] = i * 5 + i % 9;				\
	b[i * 4 + 2] = i * 7 + i % 11;				\
	b[i * 4 + 3] = i * 9 + i % 13;				\
      }								\
    vec_slp_##TYPE (a, b);					\
    for (unsigned int i = 0; i < N2; ++i)			\
      {								\
	TYPE orig_a = i * 2 + i % 5;				\
	TYPE orig_b1 = i * 3 + i % 7;				\
	TYPE orig_b2 = i * 5 + i % 9;				\
	TYPE orig_b3 = i * 7 + i % 11;				\
	TYPE orig_b4 = i * 9 + i % 13;				\
	TYPE expected_a = orig_a;				\
	TYPE expected_b1 = orig_b1;				\
	TYPE expected_b2 = orig_b2;				\
	TYPE expected_b3 = orig_b3;				\
	TYPE expected_b4 = orig_b4;				\
	if (i < N1)						\
	  {							\
	    expected_a += 1;					\
	    expected_b1 += 2;					\
	    expected_b2 += 3;					\
	    expected_b3 += 4;					\
	    expected_b4 += 5;					\
	  }							\
	if (a[i] != expected_a					\
	    || b[i * 4] != expected_b1				\
	    || b[i * 4 + 1] != expected_b2			\
	    || b[i * 4 + 2] != expected_b3			\
	    || b[i * 4 + 3] != expected_b4)			\
	  __builtin_abort ();					\
      }								\
  }

int __attribute__ ((optimize (1)))
main (void)
{
  TEST_ALL (HARNESS)
}

/* This testcase is from aarch64 and floating-point operations are removed.
   TODO: We will add floating-point operations back and make them as common test in the future.  */
