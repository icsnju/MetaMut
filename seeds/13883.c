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

#define DEF_LOOP(FN, TYPE, NAME, CONST)                                        \
  void __attribute__ ((noipa))                                                 \
  test_##TYPE##_##NAME (TYPE *__restrict x, TYPE *__restrict y,                \
			TYPE *__restrict z, int n)                             \
  {                                                                            \
    for (int i = 0; i < n; ++i)                                                \
      x[i] = y[i] < 8 ? FN (z[i], CONST) : y[i];                               \
  }

#define TEST_TYPE(T, FN, TYPE)                                                 \
  T (FN, TYPE, zero, 0)                                                        \
  T (FN, TYPE, one, 1)                                                         \
  T (FN, TYPE, two, 2)

#define TEST_ALL(T)                                                            \
  TEST_TYPE (T, FN (f32), float)                                               \
  TEST_TYPE (T, FN (f64), double)

TEST_ALL (DEF_LOOP)

/* { dg-final { scan-assembler-times {smaxv64sf3_exec} 3 } } */
/* { dg-final { scan-assembler-times {smaxv64df3_exec} 3 } } */

/* { dg-final { scan-assembler-not {\tv_writelane_b32\tv[0-9]+, vcc_..} } } */

#define N 99

#define TEST_LOOP(FN, TYPE, NAME, CONST)                                       \
  {                                                                            \
    TYPE x[N], y[N], z[N];                                                     \
    for (int i = 0; i < N; ++i)                                                \
      {                                                                        \
	y[i] = i % 13;                                                         \
	z[i] = i * i;                                                          \
      }                                                                        \
    test_##TYPE##_##NAME (x, y, z, N);                                         \
    for (int i = 0; i < N; ++i)                                                \
      {                                                                        \
	TYPE expected = y[i] < 8 ? FN (z[i], CONST) : y[i];                    \
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
