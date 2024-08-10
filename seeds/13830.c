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

/* INT -> wider-INT */
#define TEST_ALL_X2X_WIDER(T)                                                  \
  T (uint8_t, uint16_t)                                                        \
  T (uint8_t, uint32_t)                                                        \
  T (uint8_t, uint64_t)                                                        \
  T (int8_t, int16_t)                                                          \
  T (int8_t, int32_t)                                                          \
  T (int8_t, int64_t)                                                          \
  T (uint16_t, uint32_t)                                                       \
  T (uint16_t, uint64_t)                                                       \
  T (int16_t, int32_t)                                                         \
  T (int16_t, int64_t)                                                         \
  T (uint32_t, uint64_t)                                                       \
  T (int32_t, int64_t)

/* INT -> narrower-INT */
#define TEST_ALL_X2X_NARROWER(T)                                               \
  T (uint16_t, uint8_t)                                                        \
  T (int16_t, int8_t)                                                          \
  T (uint32_t, uint8_t)                                                        \
  T (int32_t, int8_t)                                                          \
  T (uint64_t, uint8_t)                                                        \
  T (int64_t, int8_t)                                                          \
  T (uint32_t, uint16_t)                                                       \
  T (int32_t, int16_t)                                                         \
  T (uint64_t, uint16_t)                                                       \
  T (int64_t, int16_t)                                                         \
  T (uint64_t, uint32_t)                                                       \
  T (int64_t, int32_t)

TEST_ALL_X2X_WIDER (DEF_LOOP)
TEST_ALL_X2X_NARROWER (DEF_LOOP)

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
  TEST_ALL_X2X_WIDER (TEST_LOOP)
  TEST_ALL_X2X_NARROWER (TEST_LOOP)
  return 0;
}
