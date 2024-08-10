/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize" } */

#include <stdint.h>

#define DEF_LOOP(TYPE, NAME, CONST)			\
  void __attribute__ ((noipa))				\
  test_##TYPE##_##NAME (TYPE *__restrict x,		\
			TYPE *__restrict y,		\
			TYPE *__restrict z,		\
			int n)				\
  {							\
    for (int i = 0; i < n; ++i)				\
      x[i] = y[i] < 8 ? (TYPE) CONST - z[i] : y[i];	\
  }

#define TEST_TYPE(T, TYPE) \
  T (TYPE, half, 0.5) \
  T (TYPE, one, 1.0) \
  T (TYPE, two, 2.0)

#define TEST_ALL(T) \
  TEST_TYPE (T, float) \
  TEST_TYPE (T, double)

TEST_ALL (DEF_LOOP)

/* { dg-final { scan-assembler-times {\tfsubr\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, #0\.5\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfsubr\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, #0\.5\n} 1 } } */

/* { dg-final { scan-assembler-times {\tfsubr\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, #1\.0\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfsubr\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, #1\.0\n} 1 } } */

/* { dg-final { scan-assembler-times {\tfmov\tz[0-9]+\.s, #2\.0} 1 } } */
/* { dg-final { scan-assembler-times {\tfmov\tz[0-9]+\.d, #2\.0} 1 } } */

/* { dg-final { scan-assembler-times {\tfsub\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfsub\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, z[0-9]+\.d\n} 1 } } */

/* { dg-final { scan-assembler-times {\tmovprfx\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s\n} 3 } } */
/* { dg-final { scan-assembler-times {\tmovprfx\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d\n} 3 } } */

/* { dg-final { scan-assembler-not {\tmov\tz} } } */
/* { dg-final { scan-assembler-not {\tsel\t} } } */

#define N 99

#define TEST_LOOP(TYPE, NAME, CONST)					\
  {									\
    TYPE x[N], y[N], z[N];						\
    for (int i = 0; i < N; ++i)						\
      {									\
	y[i] = i % 13;							\
	z[i] = i * i;							\
      }									\
    test_##TYPE##_##NAME (x, y, z, N);					\
    for (int i = 0; i < N; ++i)						\
      {									\
	TYPE expected = y[i] < 8 ? (TYPE) CONST - z[i] : y[i];		\
	if (x[i] != expected)						\
	  __builtin_abort ();						\
	asm volatile ("" ::: "memory");					\
      }									\
  }

int
main (void)
{
  TEST_ALL (TEST_LOOP)
  return 0;
}
