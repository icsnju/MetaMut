/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param=riscv-autovec-preference=scalable -fno-vect-cost-model  -ffast-math" } */

#include <stdint.h>

#define DEF_LOOP(OLD_TYPE, NEW_TYPE)                                           \
  void __attribute__ ((noipa))                                                 \
  test_##OLD_TYPE##_2_##NEW_TYPE (NEW_TYPE *__restrict r,                      \
				  OLD_TYPE *__restrict a,                      \
				  NEW_TYPE *__restrict b,                      \
				  OLD_TYPE *__restrict pred, int n)            \
  {                                                                            \
    for (int i = 0; i < n; ++i)                                                \
      {                                                                        \
	r[i] = pred[i] ? (NEW_TYPE) a[i] : b[i];                               \
      }                                                                        \
  }

/* FP -> wider-FP */
#define TEST_ALL_F2F_WIDER(T)                                                  \
  T (_Float16, float)                                                          \
  T (_Float16, double)                                                         \
  T (float, double)

/* FP -> narrower-FP */
#define TEST_ALL_F2F_NARROWER(T)                                               \
  T (float, _Float16)                                                          \
  T (double, _Float16)                                                         \
  T (double, float)

TEST_ALL_F2F_WIDER (DEF_LOOP)
TEST_ALL_F2F_NARROWER (DEF_LOOP)

#define N 99

#define TEST_LOOP(OLD_TYPE, NEW_TYPE)                                          \
  {                                                                            \
    NEW_TYPE r[N], b[N];                                                       \
    OLD_TYPE a[N], pred[N];                                                    \
    for (int i = 0; i < N; ++i)                                                \
      {                                                                        \
	a[i] = (i & 1 ? i : 3 * i) * (i % 3 == 0 ? 1 : -1);                    \
	b[i] = (i % 9) * (i % 7 + 1);                                          \
	pred[i] = (i % 7 < 4);                                                 \
	asm volatile("" ::: "memory");                                         \
      }                                                                        \
    test_##OLD_TYPE##_2_##NEW_TYPE (r, a, b, pred, N);                         \
    for (int i = 0; i < N; ++i)                                                \
      if (r[i] != (pred[i] ? (NEW_TYPE) a[i] : b[i]))                          \
	__builtin_abort ();                                                    \
  }

int
main ()
{
  TEST_ALL_F2F_WIDER (TEST_LOOP)
  TEST_ALL_F2F_NARROWER (TEST_LOOP)
  return 0;
}
