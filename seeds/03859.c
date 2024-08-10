/* { dg-do run } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do run } */
/* { dg-options "-O2 -ftree-vectorize -ffast-math" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -ffast-math -dp" } */

#include <stdint.h>

#ifndef FN
#define FN(X) __builtin_fmax##X
#endif

#define DEF_LOOP(FN, TYPE, PRED_TYPE, NAME, CONST)                             \
  void __attribute__ ((noipa))                                                 \
  test_##TYPE##_##NAME (TYPE *__restrict x, TYPE *__restrict y,                \
			PRED_TYPE *__restrict pred, int n)                     \
  {                                                                            \
    for (int i = 0; i < n; ++i)                                                \
      x[i] = pred[i] != 1 ? FN (y[i], CONST) : 0;                              \
  }

#define TEST_TYPE(T, FN, TYPE, PRED_TYPE)                                      \
  T (FN, TYPE, PRED_TYPE, zero, 0)                                             \
  T (FN, TYPE, PRED_TYPE, one, 1)                                              \
  T (FN, TYPE, PRED_TYPE, two, 2)

#define TEST_ALL(T)                                                            \
  TEST_TYPE (T, FN (f32), float, int32_t)                                      \
  TEST_TYPE (T, FN (f64), double, int64_t)

TEST_ALL (DEF_LOOP)

/* { dg-final { scan-assembler-times {smaxv64sf3} 3 } } */
/* { dg-final { scan-assembler-times {movv64sf_exec} 3 } } */
/* { dg-final { scan-assembler-times {smaxv64sf3} 3 } } */
/* { dg-final { scan-assembler-times {movv64df_exec} 3 } } */

/* { dg-final { scan-assembler-not {\tv_writelane_b32\tv[0-9]+, vcc_..} } } */
#define N 99

#define TEST_LOOP(FN, TYPE, PRED_TYPE, NAME, CONST)                            \
  {                                                                            \
    TYPE x[N], y[N];                                                           \
    PRED_TYPE pred[N];                                                         \
    for (int i = 0; i < N; ++i)                                                \
      {                                                                        \
	y[i] = i * i;                                                          \
	pred[i] = i % 3;                                                       \
      }                                                                        \
    test_##TYPE##_##NAME (x, y, pred, N);                                      \
    for (int i = 0; i < N; ++i)                                                \
      {                                                                        \
	TYPE expected = i % 3 != 1 ? FN (y[i], CONST) : 0;                     \
	if (x[i] != expected)                                                  \
	  __builtin_abort ();                                                  \
	asm volatile ("" ::: "memory");                                        \
      }                                                                        \
  }

int
main (void)
{
  TEST_ALL (TEST_LOOP)
  return 0;
}
