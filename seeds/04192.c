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

/* FP -> INT */
#define TEST_ALL_F2X_SAME(T)                                                   \
  T (_Float16, uint16_t)                                                       \
  T (_Float16, int16_t)                                                        \
  T (float, uint32_t)                                                          \
  T (float, int32_t)                                                           \
  T (double, uint64_t)                                                         \
  T (double, int64_t)

/* FP -> wider-INT */
#define TEST_ALL_F2X_WIDER(T)                                                  \
  T (_Float16, uint32_t)                                                       \
  T (_Float16, int32_t)                                                        \
  T (_Float16, uint64_t)                                                       \
  T (_Float16, int64_t)                                                        \
  T (float, uint64_t)                                                          \
  T (float, int64_t)

/* FP -> narrower-INT */
#define TEST_ALL_F2X_NARROWER(T)                                               \
  T (_Float16, uint8_t)                                                        \
  T (_Float16, int8_t)                                                         \
  T (float, uint8_t)                                                           \
  T (float, int8_t)                                                            \
  T (float, uint16_t)                                                          \
  T (float, int16_t)                                                           \
  T (double, uint8_t)                                                          \
  T (double, int8_t)                                                           \
  T (double, uint16_t)                                                         \
  T (double, int16_t)                                                          \
  T (double, uint32_t)                                                         \
  T (double, int32_t)

TEST_ALL_F2X_SAME (DEF_LOOP)
TEST_ALL_F2X_WIDER (DEF_LOOP)
TEST_ALL_F2X_NARROWER (DEF_LOOP)

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
  TEST_ALL_F2X_SAME (TEST_LOOP)
  TEST_ALL_F2X_WIDER (TEST_LOOP)
  TEST_ALL_F2X_NARROWER (TEST_LOOP)
  return 0;
}
