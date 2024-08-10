/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize" } */

#define NUM_ELEMS(TYPE) ((int) (5 * (256 / sizeof (TYPE)) + 3))

#define DEF_REDUC_PLUS(TYPE)					\
void __attribute__ ((noinline, noclone))			\
reduc_plus_##TYPE (TYPE (*restrict a)[NUM_ELEMS (TYPE)],	\
		   TYPE *restrict r, int n)			\
{								\
  for (int i = 0; i < n; i++)					\
    {								\
      r[i] = 0;							\
      for (int j = 0; j < NUM_ELEMS (TYPE); j++)		\
        r[i] += a[i][j];					\
    }								\
}

#define TEST_ALL(T) \
  T (_Float16) \
  T (float) \
  T (double)

TEST_ALL (DEF_REDUC_PLUS)

/* { dg-final { scan-assembler-times {\tfadda\th[0-9]+, p[0-7], h[0-9]+, z[0-9]+\.h} 1 } } */
/* { dg-final { scan-assembler-times {\tfadda\ts[0-9]+, p[0-7], s[0-9]+, z[0-9]+\.s} 1 } } */
/* { dg-final { scan-assembler-times {\tfadda\td[0-9]+, p[0-7], d[0-9]+, z[0-9]+\.d} 1 } } */

#define NROWS 5

#define TEST_REDUC_PLUS(TYPE)					\
  {								\
    TYPE a[NROWS][NUM_ELEMS (TYPE)];				\
    TYPE r[NROWS];						\
    TYPE expected[NROWS] = {};					\
    for (int i = 0; i < NROWS; ++i)				\
      for (int j = 0; j < NUM_ELEMS (TYPE); ++j)		\
	{							\
	  a[i][j] = (i * 0.1 + j * 0.6) * (j & 1 ? 1 : -1);	\
	  expected[i] += a[i][j];				\
	  asm volatile ("" ::: "memory");			\
	}							\
    reduc_plus_##TYPE (a, r, NROWS);				\
    for (int i = 0; i < NROWS; ++i)				\
      if (r[i] != expected[i])					\
	__builtin_abort ();					\
  }

int __attribute__ ((optimize (1)))
main ()
{
  TEST_ALL (TEST_REDUC_PLUS);
  return 0;
}
