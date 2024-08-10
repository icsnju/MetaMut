/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize" } */

#include <stdint.h>

#define b_and(A, B) ((A) & (B))
#define b_orr(A, B) ((A) | (B))
#define b_eor(A, B) ((A) ^ (B))
#define b_nand(A, B) (!((A) & (B)))
#define b_nor(A, B) (!((A) | (B)))
#define b_bic(A, B) ((A) & !(B))
#define b_orn(A, B) ((A) | !(B))

#define LOOP(TYPE, BINOP)						\
  void __attribute__ ((noinline, noclone))				\
  test_##TYPE##_##BINOP (TYPE *restrict dest, TYPE *restrict src,	\
			 TYPE *restrict a, TYPE *restrict b,		\
			 TYPE *restrict c, TYPE *restrict d,		\
			 TYPE fallback, int count)			\
  {									\
    for (int i = 0; i < count; ++i)					\
      {									\
	TYPE srcv = src[i];						\
	dest[i] = (BINOP (__builtin_isunordered (a[i], b[i]),		\
			  __builtin_isunordered (c[i], d[i]))		\
		   ? srcv : fallback);					\
      }									\
  }

#define TEST_BINOP(T, BINOP) \
  T (_Float16, BINOP) \
  T (float, BINOP) \
  T (double, BINOP)

#define TEST_ALL(T) \
  TEST_BINOP (T, b_and) \
  TEST_BINOP (T, b_orr) \
  TEST_BINOP (T, b_eor) \
  TEST_BINOP (T, b_nand) \
  TEST_BINOP (T, b_nor) \
  TEST_BINOP (T, b_bic) \
  TEST_BINOP (T, b_orn)

TEST_ALL (LOOP)

/* ??? We predicate one of the comparisons on the result of the other,
   but whether that's a win or a loss will depend on the schedule.  */
/* { dg-final { scan-assembler-not {\tand\t} } } */
/* { dg-final { scan-assembler-times {\torr\tp[0-9]+\.b, p[0-9]+/z, p[0-9]+\.b, p[0-9]+\.b} 3 } } */
/* { dg-final { scan-assembler-times {\teor\tp[0-9]+\.b, p[0-9]+/z, p[0-9]+\.b, p[0-9]+\.b} 3 } } */
/* { dg-final { scan-assembler-times {\tnand\tp[0-9]+\.b, p[0-9]+/z, p[0-9]+\.b, p[0-9]+\.b} 3 } } */
/* { dg-final { scan-assembler-times {\tnor\tp[0-9]+\.b, p[0-9]+/z, p[0-9]+\.b, p[0-9]+\.b} 3 } } */
/* Currently we predicate one of the comparisons on the result of the other
   and then use NOT, but the original BIC sequence is better.  It's a fairly
   niche failure though.  We'd handle most other types of comparison by
   using the inverse operation instead of a separate NOT.  */
/* { dg-final { scan-assembler-times {\tbic\tp[0-9]+\.b, p[0-9]+/z, p[0-9]+\.b, p[0-9]+\.b} 3 { xfail *-*-* } } } */
/* { dg-final { scan-assembler-times {\torn\tp[0-9]+\.b, p[0-9]+/z, p[0-9]+\.b, p[0-9]+\.b} 3 } } */

#define N 401

#define RUN_LOOP(TYPE, BINOP)						\
  {									\
    TYPE dest[N], src[N], a[N], b[N], c[N], d[N];			\
    for (int i = 0; i < N; ++i)						\
      {									\
	src[i] = i * i;							\
	a[i] = i % 5 < 3 ? __builtin_nan("") : i;			\
	b[i] = i % 7 < 4 ? __builtin_nan("") : i;			\
	c[i] = i % 9 < 5 ? __builtin_nan("") : i;			\
	d[i] = i % 11 < 6 ? __builtin_nan("") : i;			\
	asm volatile ("" ::: "memory");					\
      }									\
    test_##TYPE##_##BINOP (dest, src, a, b, c, d, 100, N);		\
    for (int i = 0; i < N; ++i)						\
      {									\
	int res = BINOP (__builtin_isunordered (a[i], b[i]),		\
			 __builtin_isunordered (c[i], d[i]));		\
	if (dest[i] != (res ? src[i] : 100.0))				\
	  __builtin_abort ();						\
      }									\
  }

int __attribute__ ((optimize (1)))
main (void)
{
  TEST_ALL (RUN_LOOP)
  return 0;
}
