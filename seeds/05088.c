/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -ffast-math" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -msve-vector-bits=scalable -ffast-math" } */

#include <stdint.h>

#define VEC_PERM(TYPE)						\
void __attribute__ ((noinline, noclone))			\
vec_slp_##TYPE (TYPE *restrict a, TYPE *restrict b, int n)	\
{								\
  TYPE x0 = b[0];						\
  TYPE x1 = b[1];						\
  for (int i = 0; i < n; ++i)					\
    {								\
      x0 += a[i * 2];						\
      x1 += a[i * 2 + 1];					\
    }								\
  b[0] = x0;							\
  b[1] = x1;							\
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

/* { dg-final { scan-assembler-times {\tld1b\t} 2 } } */
/* { dg-final { scan-assembler-times {\tld1h\t} 3 } } */
/* { dg-final { scan-assembler-times {\tld1w\t} 3 } } */
/* { dg-final { scan-assembler-times {\tld1d\t} 3 } } */
/* { dg-final { scan-assembler-not {\tld2b\t} } } */
/* { dg-final { scan-assembler-not {\tld2h\t} } } */
/* { dg-final { scan-assembler-not {\tld2w\t} } } */
/* { dg-final { scan-assembler-not {\tld2d\t} } } */
/* { dg-final { scan-assembler-times {\tuaddv\td[0-9]+, p[0-7], z[0-9]+\.b} 4 } } */
/* { dg-final { scan-assembler-times {\tuaddv\td[0-9]+, p[0-7], z[0-9]+\.h} 4 } } */
/* { dg-final { scan-assembler-times {\tuaddv\td[0-9]+, p[0-7], z[0-9]+\.s} 4 } } */
/* { dg-final { scan-assembler-times {\tuaddv\td[0-9]+, p[0-7], z[0-9]+\.d} 4 } } */
/* { dg-final { scan-assembler-times {\tfaddv\th[0-9]+, p[0-7], z[0-9]+\.h} 2 } } */
/* { dg-final { scan-assembler-times {\tfaddv\ts[0-9]+, p[0-7], z[0-9]+\.s} 2 } } */
/* { dg-final { scan-assembler-times {\tfaddv\td[0-9]+, p[0-7], z[0-9]+\.d} 2 } } */

/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.b} 4 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.h} 6 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.s} 6 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.d} 6 } } */

/* { dg-final { scan-assembler-not {\tuqdec} } } */

#define N (141 * 2)

#define HARNESS(TYPE)					\
  {							\
    TYPE a[N], b[2] = { 40, 22 };			\
    for (unsigned int i = 0; i < N; ++i)		\
      {							\
	a[i] = i * 2 + i % 5;				\
	asm volatile ("" ::: "memory");			\
      }							\
    vec_slp_##TYPE (a, b, N / 2);			\
    TYPE x0 = 40;					\
    TYPE x1 = 22;					\
    for (unsigned int i = 0; i < N; i += 2)		\
      {							\
	x0 += a[i];					\
	x1 += a[i + 1];					\
	asm volatile ("" ::: "memory");			\
      }							\
    /* _Float16 isn't precise enough for this.  */	\
    if ((TYPE) 0x1000 + 1 != (TYPE) 0x1000		\
	&& (x0 != b[0] || x1 != b[1]))			\
      __builtin_abort ();				\
  }

int __attribute__ ((optimize (1)))
main (void)
{
  TEST_ALL (HARNESS)
}
