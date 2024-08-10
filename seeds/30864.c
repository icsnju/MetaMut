/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-Ofast -mlow-precision-sqrt" } */

/* { dg-options "-Ofast -mlow-precision-sqrt" } */

#define DEF_LOOP(TYPE, FN)			\
  void						\
  test_##TYPE (TYPE *x, int n)			\
  {						\
    for (int i = 0; i < n; ++i)			\
      x[i] = (TYPE) 1 / FN (x[i]);		\
  }

#define TEST_ALL(T)			\
  T (_Float16, __builtin_sqrtf16)	\
  T (float, __builtin_sqrtf)		\
  T (double, __builtin_sqrt)

TEST_ALL (DEF_LOOP)

/* { dg-final { scan-assembler-not {\tfrsqrte\tz[0-9]+\.h} } } */
/* { dg-final { scan-assembler-not {\tfrsqrts\tz[0-9]+\.h} } } */

/* { dg-final { scan-assembler-times {\tfmul\tz[0-9]+\.s} 2 } } */
/* { dg-final { scan-assembler-times {\tfrsqrte\tz[0-9]+\.s} 1 } } */
/* { dg-final { scan-assembler-times {\tfrsqrts\tz[0-9]+\.s} 1 } } */

/* { dg-final { scan-assembler-times {\tfmul\tz[0-9]+\.d} 4 } } */
/* { dg-final { scan-assembler-times {\tfrsqrte\tz[0-9]+\.d} 1 } } */
/* { dg-final { scan-assembler-times {\tfrsqrts\tz[0-9]+\.d} 2 } } */

#define N 77

#define TEST_LOOP(TYPE, FN)					\
  {								\
    TYPE a[N];							\
    for (int i = 0; i < N; ++i)					\
      a[i] = i + 1;						\
    test_##TYPE (a, N);						\
    for (int i = 0; i < N; ++i)					\
      {								\
	double diff = a[i] - 1.0 / __builtin_sqrt (i + 1);	\
	if (__builtin_fabs (diff) > 0x1.0p-8)			\
	  __builtin_abort ();					\
      }								\
  }

int
main (void)
{
  TEST_ALL (TEST_LOOP);
  return 0;
}
