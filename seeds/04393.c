/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -ffast-math" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -msve-vector-bits=scalable -ffast-math -fno-vect-cost-model" } */

#include <stdint.h>

#define VEC_PERM(TYPE)						\
void __attribute__ ((noinline, noclone))			\
vec_slp_##TYPE (TYPE *restrict a, TYPE *restrict b, int n)	\
{								\
  TYPE x0 = b[0];						\
  TYPE x1 = b[1];						\
  TYPE x2 = b[2];						\
  for (int i = 0; i < n; ++i)					\
    {								\
      x0 += a[i * 3];						\
      x1 += a[i * 3 + 1];					\
      x2 += a[i * 3 + 2];					\
    }								\
  b[0] = x0;							\
  b[1] = x1;							\
  b[2] = x2;							\
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
  T (_Float16)					\
  T (float)					\
  T (double)

TEST_ALL (VEC_PERM)

/* These loops can't use SLP.  */
/* { dg-final { scan-assembler-not {\tld1b\t} } } */
/* { dg-final { scan-assembler-not {\tld1h\t} } } */
/* { dg-final { scan-assembler-not {\tld1w\t} } } */
/* { dg-final { scan-assembler-not {\tld1d\t} } } */
/* { dg-final { scan-assembler {\tld3b\t} } } */
/* { dg-final { scan-assembler {\tld3h\t} } } */
/* { dg-final { scan-assembler {\tld3w\t} } } */
/* { dg-final { scan-assembler {\tld3d\t} } } */

/* { dg-final { scan-assembler-not {\tuqdec} } } */

#define N (77 * 3)

#define HARNESS(TYPE)					\
  {							\
    TYPE a[N], b[3] = { 40, 22, 75 };			\
    for (unsigned int i = 0; i < N; ++i)		\
      {							\
	a[i] = i * 2 + i % 5;				\
	asm volatile ("" ::: "memory");			\
      }							\
    vec_slp_##TYPE (a, b, N / 3);			\
    TYPE x0 = 40;					\
    TYPE x1 = 22;					\
    TYPE x2 = 75;					\
    for (unsigned int i = 0; i < N; i += 3)		\
      {							\
	x0 += a[i];					\
	x1 += a[i + 1];					\
	x2 += a[i + 2];					\
	asm volatile ("" ::: "memory");			\
      }							\
    /* _Float16 isn't precise enough for this.  */	\
    if ((TYPE) 0x1000 + 1 != (TYPE) 0x1000		\
	&& (x0 != b[0] || x1 != b[1] || x2 != b[2]))	\
      __builtin_abort ();				\
  }

int __attribute__ ((optimize (1)))
main (void)
{
  TEST_ALL (HARNESS)
}
