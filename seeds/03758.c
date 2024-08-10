/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param=riscv-autovec-preference=fixed-vlmax -fno-vect-cost-model -ffast-math" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=fixed-vlmax -fno-vect-cost-model -fdump-tree-optimized-details" } */

#include <stdint-gcc.h>

#define TEST(TYPE, NAME, OP)                                                   \
  void __attribute__ ((noinline, noclone))                                     \
  test_##TYPE##_##NAME (TYPE *__restrict x, TYPE *__restrict y,                \
			TYPE *__restrict z, TYPE *__restrict pred, int n)      \
  {                                                                            \
    for (int i = 0; i < 128; ++i)                                              \
      x[i] = pred[i] != 1 ? y[i] OP z[i] : y[i];                               \
  }

#define TEST_TYPE(TYPE)                                                        \
  TEST (TYPE, add, +)                                                          \
  TEST (TYPE, sub, -)                                                          \
  TEST (TYPE, mul, *)                                                          \
  TEST (TYPE, div, /)

#define TEST_TYPE2(TYPE) TEST (TYPE, rem, %)

#define TEST_ALL                                                               \
  TEST_TYPE (int8_t)                                                           \
  TEST_TYPE (uint8_t)                                                          \
  TEST_TYPE (int16_t)                                                          \
  TEST_TYPE (uint16_t)                                                         \
  TEST_TYPE (int32_t)                                                          \
  TEST_TYPE (uint32_t)                                                         \
  TEST_TYPE (int64_t)                                                          \
  TEST_TYPE2 (int8_t)                                                          \
  TEST_TYPE2 (uint8_t)                                                         \
  TEST_TYPE2 (int16_t)                                                         \
  TEST_TYPE2 (uint16_t)                                                        \
  TEST_TYPE2 (int32_t)                                                         \
  TEST_TYPE2 (uint32_t)                                                        \
  TEST_TYPE2 (int64_t)                                                         \
  TEST_TYPE2 (uint64_t)                                                        \
  TEST_TYPE (_Float16)                                                         \
  TEST_TYPE (float)                                                            \
  TEST_TYPE (double)

TEST_ALL

#define N 128

#undef TEST
#define TEST(TYPE, NAME, OP)					\
  {								\
    TYPE x[N], y[N], z[N], pred[N];				\
    for (int i = 0; i < N; ++i)					\
      {								\
	y[i] = i * i;						\
	z[i] = ((i + 2) % 3) * (i + 1);				\
	pred[i] = i % 3;					\
      }								\
    test_##TYPE##_##NAME (x, y, z, pred, N);			\
    for (int i = 0; i < N; ++i)					\
      {								\
	TYPE expected = i % 3 != 1 ? y[i] OP z[i] : y[i];	\
	if (x[i] != expected)					\
	  __builtin_abort ();					\
	asm volatile ("" ::: "memory");				\
      }								\
  }

int
main (void)
{
  TEST_ALL
  return 0;
}
