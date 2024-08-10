/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -msve-vector-bits=scalable" } */

#include <stdint.h>

#define VEC_PERM(TYPE)						\
void __attribute__ ((noinline, noclone))			\
vec_slp_##TYPE (TYPE *restrict a, TYPE *restrict b, int n)	\
{								\
  for (int i = 0; i < n; ++i)					\
    {								\
      a[i] += 1;						\
      b[i * 4] += 2;						\
      b[i * 4 + 1] += 3;					\
      b[i * 4 + 2] += 4;					\
      b[i * 4 + 3] += 5;					\
    }								\
}

#define TEST_ALL(T)				\
  T (int8_t)					\
  T (uint8_t)					\
  T (int16_t)					\
  T (uint16_t)					\
  T (int32_t)					\
  T (uint32_t)					\
  T (int64_t)					\
  T (uint64_t)					\
  T (float)					\
  T (double)

TEST_ALL (VEC_PERM)

/* The loop should be fully-masked.  */
/* { dg-final { scan-assembler-times {\tld1b\t} 10 } } */
/* { dg-final { scan-assembler-times {\tst1b\t} 10 } } */
/* { dg-final { scan-assembler-times {\tld1h\t} 10 } } */
/* { dg-final { scan-assembler-times {\tst1h\t} 10 } } */
/* { dg-final { scan-assembler-times {\tld1w\t} 15 } } */
/* { dg-final { scan-assembler-times {\tst1w\t} 15 } } */
/* { dg-final { scan-assembler-times {\tld1d\t} 15 } } */
/* { dg-final { scan-assembler-times {\tst1d\t} 15 } } */
/* { dg-final { scan-assembler-not {\tldr} } } */
/* { dg-final { scan-assembler-not {\tstr} } } */

/* We should use WHILEs for all accesses.  */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.b} 20 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.h} 20 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.s} 30 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.d} 30 } } */

/* 6 for the 8-bit types and 2 for the 16-bit types.  */
/* { dg-final { scan-assembler-times {\tuqdecb\t} 8 } } */
/* 4 for the 16-bit types and 3 for the 32-bit types.  */
/* { dg-final { scan-assembler-times {\tuqdech\t} 7 } } */
/* 6 for the 32-bit types and 3 for the 64-bit types.  */
/* { dg-final { scan-assembler-times {\tuqdecw\t} 9 } } */
/* { dg-final { scan-assembler-times {\tuqdecd\t} 6 } } */

#define N1 (103 * 2)
#define N2 (111 * 2)

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
    vec_slp_##TYPE (a, b, N1);					\
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
