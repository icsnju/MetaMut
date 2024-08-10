/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-Ofast -mlow-precision-div" } */

/* { dg-options "-Ofast -mlow-precision-div" } */

#define DEF_LOOP(TYPE)						\
  void								\
  test_##TYPE (TYPE *restrict x, TYPE *restrict y, int n)	\
  {								\
    for (int i = 0; i < n; ++i)					\
      x[i] /= y[i];						\
  }

#define TEST_ALL(T)	\
  T (_Float16)		\
  T (float)		\
  T (double)

TEST_ALL (DEF_LOOP)

/* { dg-final { scan-assembler-not {\tfrecpe\tz[0-9]+\.h} } } */
/* { dg-final { scan-assembler-not {\tfrecps\tz[0-9]+\.h} } } */

/* { dg-final { scan-assembler-times {\tfmul\tz[0-9]+\.s} 2 } } */
/* { dg-final { scan-assembler-times {\tfrecpe\tz[0-9]+\.s} 1 } } */
/* { dg-final { scan-assembler-times {\tfrecps\tz[0-9]+\.s} 1 } } */

/* { dg-final { scan-assembler-times {\tfmul\tz[0-9]+\.d} 3 } } */
/* { dg-final { scan-assembler-times {\tfrecpe\tz[0-9]+\.d} 1 } } */
/* { dg-final { scan-assembler-times {\tfrecps\tz[0-9]+\.d} 2 } } */

#define N 77

#define TEST_LOOP(TYPE)					\
  {							\
    TYPE a[N], b[N];					\
    for (int i = 0; i < N; ++i)				\
      {							\
	a[i] = i + 11;					\
	b[i] = i + 1;					\
      }							\
    test_##TYPE (a, b, N);				\
    for (int i = 0; i < N; ++i)				\
      {							\
	double diff = a[i] - (i + 11.0) / (i + 1);	\
	if (__builtin_fabs (diff) > 0x1.0p-8)		\
	  __builtin_abort ();				\
      }							\
  }

int
main (void)
{
  TEST_ALL (TEST_LOOP);
  return 0;
}
