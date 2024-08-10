/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize" } */

#include <stdint.h>

#define VEC_PERM(TYPE)						\
void __attribute__ ((noinline, noclone))			\
vec_slp_##TYPE (TYPE *restrict a, TYPE *restrict b, int n)	\
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

/* The loop should be fully-masked.  The load XFAILs for fixed-length
   SVE account for extra loads from the constant pool.  */
/* { dg-final { scan-assembler-times {\tld1b\t} 6 { xfail { aarch64_sve && { ! vect_variable_length } } } } } */
/* { dg-final { scan-assembler-times {\tst1b\t} 6 } } */
/* { dg-final { scan-assembler-times {\tld1h\t} 6 { xfail { aarch64_sve && { ! vect_variable_length } } } } } */
/* { dg-final { scan-assembler-times {\tst1h\t} 6 } } */
/* { dg-final { scan-assembler-times {\tld1w\t} 9 { xfail { aarch64_sve && { ! vect_variable_length } } } } } */
/* { dg-final { scan-assembler-times {\tst1w\t} 9 } } */
/* { dg-final { scan-assembler-times {\tld1d\t} 9 { xfail { aarch64_sve && { ! vect_variable_length } } } } } */
/* { dg-final { scan-assembler-times {\tst1d\t} 9 } } */
/* { dg-final { scan-assembler-not {\tldr} } } */
/* { dg-final { scan-assembler-not {\tstr} } } */

/* We should use WHILEs for the accesses to "a" and ZIPs for the accesses
   to "b".  */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.b} 4 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.h} 4 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.s} 6 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.d} 6 } } */
/* { dg-final { scan-assembler-times {\tzip1\tp[0-7]\.b} 2 } } */
/* { dg-final { scan-assembler-times {\tzip1\tp[0-7]\.h} 2 } } */
/* { dg-final { scan-assembler-times {\tzip1\tp[0-7]\.s} 3 } } */
/* { dg-final { scan-assembler-times {\tzip1\tp[0-7]\.d} 3 } } */
/* { dg-final { scan-assembler-times {\tzip2\tp[0-7]\.b} 2 } } */
/* { dg-final { scan-assembler-times {\tzip2\tp[0-7]\.h} 2 } } */
/* { dg-final { scan-assembler-times {\tzip2\tp[0-7]\.s} 3 } } */
/* { dg-final { scan-assembler-times {\tzip2\tp[0-7]\.d} 3 } } */

/* { dg-final { scan-assembler-not {\tuqdec} } } */

#define N1 (103 * 2)
#define N2 (111 * 2)

#define HARNESS(TYPE)						\
  {								\
    TYPE a[N2], b[N2 * 2];					\
    for (unsigned int i = 0; i < N2; ++i)			\
      {								\
	a[i] = i * 2 + i % 5;					\
	b[i * 2] = i * 3 + i % 7;				\
	b[i * 2 + 1] = i * 5 + i % 9;				\
      }								\
    vec_slp_##TYPE (a, b, N1 / 2);				\
    for (unsigned int i = 0; i < N2; ++i)			\
      {								\
	TYPE orig_a = i * 2 + i % 5;				\
	TYPE orig_b1 = i * 3 + i % 7;				\
	TYPE orig_b2 = i * 5 + i % 9;				\
	TYPE expected_a = orig_a;				\
	TYPE expected_b1 = orig_b1;				\
	TYPE expected_b2 = orig_b2;				\
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
