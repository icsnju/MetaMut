/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize" } */

#define NUM_ELEMS(TYPE) ((int)(5 * (256 / sizeof (TYPE)) + 3))

#define DEF_REDUC_PLUS(TYPE)			\
  TYPE __attribute__ ((noinline, noclone))	\
  reduc_plus_##TYPE (TYPE *a, TYPE *b)		\
  {						\
    TYPE r = 0, q = 3;				\
    for (int i = 0; i < NUM_ELEMS (TYPE); i++)	\
      {						\
	r += a[i];				\
	q -= b[i];				\
      }						\
    return r * q;				\
  }

#define TEST_ALL(T) \
  T (_Float16) \
  T (float) \
  T (double)

TEST_ALL (DEF_REDUC_PLUS)

/* { dg-final { scan-assembler-times {\tfadda\th[0-9]+, p[0-7], h[0-9]+, z[0-9]+\.h} 2 } } */
/* { dg-final { scan-assembler-times {\tfadda\ts[0-9]+, p[0-7], s[0-9]+, z[0-9]+\.s} 2 } } */
/* { dg-final { scan-assembler-times {\tfadda\td[0-9]+, p[0-7], d[0-9]+, z[0-9]+\.d} 2 } } */

#define TEST_REDUC_PLUS(TYPE)			\
  {						\
    TYPE a[NUM_ELEMS (TYPE)];			\
    TYPE b[NUM_ELEMS (TYPE)];			\
    TYPE r = 0, q = 3;				\
    for (int i = 0; i < NUM_ELEMS (TYPE); i++)	\
      {						\
	a[i] = (i * 0.1) * (i & 1 ? 1 : -1);	\
	b[i] = (i * 0.3) * (i & 1 ? 1 : -1);	\
	r += a[i];				\
	q -= b[i];				\
	asm volatile ("" ::: "memory");		\
      }						\
    TYPE res = reduc_plus_##TYPE (a, b);	\
    if (res != r * q)				\
      __builtin_abort ();			\
  }

int __attribute__ ((optimize (1)))
main ()
{
  TEST_ALL (TEST_REDUC_PLUS);
  return 0;
}
