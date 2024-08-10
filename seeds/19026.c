/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param=riscv-autovec-preference=scalable -fno-vect-cost-model" } */

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

/* INT -> FP */
#define TEST_ALL_X2F_SAME(T)                                                   \
  T (uint16_t, _Float16)                                                       \
  T (int16_t, _Float16)                                                        \
  T (uint32_t, float)                                                          \
  T (int32_t, float)                                                           \
  T (uint64_t, double)                                                         \
  T (int64_t, double)

/* INT -> wider-FP */
#define TEST_ALL_X2F_WIDER(T)                                                  \
  T (uint16_t, float)                                                          \
  T (int16_t, float)                                                           \
  T (uint16_t, double)                                                         \
  T (int16_t, double)                                                          \
  T (uint32_t, double)                                                         \
  T (int32_t, double)

/* INT -> narrower-FP */
#define TEST_ALL_X2F_NARROWER(T)                                               \
  T (uint32_t, _Float16)                                                       \
  T (int32_t, _Float16)                                                        \
  T (uint64_t, _Float16)                                                       \
  T (int64_t, _Float16)                                                        \
  T (uint64_t, float)                                                          \
  T (int64_t, float)

TEST_ALL_X2F_SAME (DEF_LOOP)
TEST_ALL_X2F_WIDER (DEF_LOOP)
TEST_ALL_X2F_NARROWER (DEF_LOOP)

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
  TEST_ALL_X2F_SAME (TEST_LOOP)
  TEST_ALL_X2F_WIDER (TEST_LOOP)
  TEST_ALL_X2F_NARROWER (TEST_LOOP)
  return 0;
}
