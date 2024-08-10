/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -msve-vector-bits=scalable" } */

#include <stdint.h>

#define VEC_PERM(TYPE1, TYPE2)					\
void __attribute__ ((noinline, noclone))			\
vec_slp_##TYPE1##_##TYPE2 (TYPE1 *restrict a,			\
			   TYPE2 *restrict b, int n)		\
{								\
  for (int i = 0; i < n; ++i)					\
    {								\
      a[i * 2] += 1;						\
      a[i * 2 + 1] += 2;					\
      b[i * 4] += 3;						\
      b[i * 4 + 1] += 4;					\
      b[i * 4 + 2] += 5;					\
      b[i * 4 + 3] += 6;					\
    }								\
}

#define TEST_ALL(T)				\
  T (int16_t, uint8_t)				\
  T (uint16_t, int8_t)				\
  T (int32_t, uint16_t)				\
  T (uint32_t, int16_t)				\
  T (float, uint16_t)				\
  T (int64_t, float)				\
  T (uint64_t, int32_t)				\
  T (double, uint32_t)

TEST_ALL (VEC_PERM)

/* The loop should be fully-masked.  */
/* { dg-final { scan-assembler-times {\tld1b\t} 2 } } */
/* { dg-final { scan-assembler-times {\tst1b\t} 2 } } */
/* { dg-final { scan-assembler-times {\tld1h\t} 5 } } */
/* { dg-final { scan-assembler-times {\tst1h\t} 5 } } */
/* { dg-final { scan-assembler-times {\tld1w\t} 6 } } */
/* { dg-final { scan-assembler-times {\tst1w\t} 6 } } */
/* { dg-final { scan-assembler-times {\tld1d\t} 3 } } */
/* { dg-final { scan-assembler-times {\tst1d\t} 3 } } */
/* { dg-final { scan-assembler-not {\tldr} } } */
/* { dg-final { scan-assembler-not {\tstr} } } */

/* We should use the same WHILEs for both accesses.  */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.b} 4 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.h} 6 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.s} 6 } } */
/* { dg-final { scan-assembler-not {\twhilelo\tp[0-7]\.d} } } */

/* { dg-final { scan-assembler-not {\tuqdec} } } */

#define N1 (103 * 2)
#define N2 (111 * 2)

#define HARNESS(TYPE1, TYPE2)					\
  {								\
    TYPE1 a[N2];						\
    TYPE2 b[N2 * 2];						\
    for (unsigned int i = 0; i < N2; ++i)			\
      {								\
	a[i] = i * 2 + i % 5;					\
	b[i * 2] = i * 3 + i % 7;				\
	b[i * 2 + 1] = i * 5 + i % 9;				\
      }								\
    vec_slp_##TYPE1##_##TYPE2 (a, b, N1 / 2);			\
    for (unsigned int i = 0; i < N2; ++i)			\
      {								\
	TYPE1 orig_a = i * 2 + i % 5;				\
	TYPE2 orig_b1 = i * 3 + i % 7;				\
	TYPE2 orig_b2 = i * 5 + i % 9;				\
	TYPE1 expected_a = orig_a;				\
	TYPE2 expected_b1 = orig_b1;				\
	TYPE2 expected_b2 = orig_b2;				\
	if (i < N1)						\
	  {							\
	    expected_a += i & 1 ? 2 : 1;			\
	    expected_b1 += i & 1 ? 5 : 3;			\
	    expected_b2 += i & 1 ? 6 : 4;			\
	  }							\
	if (a[i] != expected_a					\
	    || b[i * 2] != expected_b1				\
	    || b[i * 2 + 1] != expected_b2)			\
	  __builtin_abort ();					\
      }								\
  }

int __attribute__ ((optimize (1)))
main (void)
{
  TEST_ALL (HARNESS)
}
