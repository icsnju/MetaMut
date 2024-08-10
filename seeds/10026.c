/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize" } */

#define DEF_LOOP(TYPE, NAME, CONST)			\
  void							\
  test_##TYPE##_##NAME (TYPE *restrict x,		\
			TYPE *restrict pred, int n)	\
  {							\
    for (int i = 0; i < n; ++i)				\
      x[i] = pred[i] > 0 ? CONST : 12.0;		\
  }

#define TEST_TYPE(T, TYPE)			\
  T (TYPE, 2, 2.0)				\
  T (TYPE, 1p25, 1.25)				\
  T (TYPE, 32p25, 32.25)			\
  T (TYPE, m4, -4.0)				\
  T (TYPE, m2p5, -2.5)				\
  T (TYPE, m64p5, -64.5)

#define TEST_ALL(T)				\
  TEST_TYPE (T, _Float16)			\
  TEST_TYPE (T, float)				\
  TEST_TYPE (T, double)

TEST_ALL (DEF_LOOP)

/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.h, p[0-9]+/m, #16384\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.h, p[0-9]+/m, #15616\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.h, p[0-9]+/m, #-15360\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.h, p[0-9]+/m, #-16128\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsel\tz[0-9]+\.h, p[0-9]+, z[0-9]+\.h, z[0-9]+\.h\n} 2 } } */

/* { dg-final { scan-assembler {\tfmov\t(z[0-9]+\.s), p[0-9]+/m, #2\.0(?:e[+]0)?\n} } } */
/* { dg-final { scan-assembler {\tfmov\t(z[0-9]+\.s), p[0-9]+/m, #1\.25(?:e[+]0)?\n} } } */
/* { dg-final { scan-assembler {\tfmov\t(z[0-9]+\.s), p[0-9]+/m, #-4\.0(?:e[+]0)?\n} } } */
/* { dg-final { scan-assembler {\tfmov\t(z[0-9]+\.s), p[0-9]+/m, #-2\.5(?:e[+]0)?\n} } } */
/* { dg-final { scan-assembler-times {\tsel\tz[0-9]+\.s, p[0-9]+, z[0-9]+\.s, z[0-9]+\.s\n} 2 } } */

/* { dg-final { scan-assembler {\tfmov\t(z[0-9]+\.d), p[0-9]+/m, #2\.0(?:e[+]0)?\n} } } */
/* { dg-final { scan-assembler {\tfmov\t(z[0-9]+\.d), p[0-9]+/m, #1\.25(?:e[+]0)?\n} } } */
/* { dg-final { scan-assembler {\tfmov\t(z[0-9]+\.d), p[0-9]+/m, #-4\.0(?:e[+]0)?\n} } } */
/* { dg-final { scan-assembler {\tfmov\t(z[0-9]+\.d), p[0-9]+/m, #-2\.5(?:e[+]0)?\n} } } */
/* { dg-final { scan-assembler-times {\tsel\tz[0-9]+\.d, p[0-9]+, z[0-9]+\.d, z[0-9]+\.d\n} 2 } } */

/* { dg-final { scan-assembler-times {\tmovprfx\tz[0-9]+, z[0-9]+\n} 12 } } */

#define N 97

#define TEST_LOOP(TYPE, NAME, CONST)				\
  {								\
    TYPE x[N], pred[N];						\
    for (int i = 0; i < N; ++i)					\
      {								\
	pred[i] = i % 5 <= i % 6;				\
	asm volatile ("" ::: "memory");				\
      }								\
    test_##TYPE##_##NAME (x, pred, N);				\
    for (int i = 0; i < N; ++i)					\
      {								\
	if (x[i] != (TYPE) (pred[i] > 0 ? CONST : 12.0))	\
	  __builtin_abort ();					\
	asm volatile ("" ::: "memory");				\
      }								\
  }

int __attribute__ ((optimize (1)))
main (int argc, char **argv)
{
  TEST_ALL (TEST_LOOP)
  return 0;
}
