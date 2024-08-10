/* { dg-do run { target { riscv_vector } } } */

/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvfh -mabi=lp64d -O3 --param riscv-autovec-preference=scalable -fno-vect-cost-model -ffast-math -fdump-tree-optimized-details" } */

#include <stdint-gcc.h>

#ifndef INDEX8
#define INDEX8 int8_t
#define INDEX16 int16_t
#define INDEX32 int32_t
#define INDEX64 int64_t
#endif

#define TEST_LOOP(DATA_TYPE, BITS)                                             \
  void __attribute__ ((noinline, noclone))                                     \
  f_##DATA_TYPE##_##BITS (DATA_TYPE *restrict dest, DATA_TYPE *restrict src,   \
			  INDEX##BITS stride, INDEX##BITS n)                   \
  {                                                                            \
    for (INDEX##BITS i = 0; i < n; ++i)                                        \
      dest[i * stride] = src[i] + BITS;                                        \
  }

#define TEST_TYPE(T, DATA_TYPE)                                                \
  T (DATA_TYPE, 8)                                                             \
  T (DATA_TYPE, 16)                                                            \
  T (DATA_TYPE, 32)                                                            \
  T (DATA_TYPE, 64)

#define TEST_ALL(T)                                                            \
  TEST_TYPE (T, int8_t)                                                        \
  TEST_TYPE (T, uint8_t)                                                       \
  TEST_TYPE (T, int16_t)                                                       \
  TEST_TYPE (T, uint16_t)                                                      \
  TEST_TYPE (T, _Float16)                                                      \
  TEST_TYPE (T, int32_t)                                                       \
  TEST_TYPE (T, uint32_t)                                                      \
  TEST_TYPE (T, float)                                                         \
  TEST_TYPE (T, int64_t)                                                       \
  TEST_TYPE (T, uint64_t)                                                      \
  TEST_TYPE (T, double)

TEST_ALL (TEST_LOOP)

/* { dg-final { scan-tree-dump-times " \.MASK_LEN_SCATTER_STORE" 66 "optimized" } } */
/* { dg-final { scan-tree-dump-not " \.SCATTER_STORE" "optimized" } } */
/* { dg-final { scan-tree-dump-not " \.MASK_SCATTER_STORE" "optimized" } } */
#include <assert.h>

int
main (void)
{
#define RUN_LOOP(DATA_TYPE, BITS)                                              \
  DATA_TYPE dest_##DATA_TYPE##_##BITS[(BITS - 3) * (BITS + 13)];               \
  DATA_TYPE dest2_##DATA_TYPE##_##BITS[(BITS - 3) * (BITS + 13)];              \
  DATA_TYPE src_##DATA_TYPE##_##BITS[(BITS - 3) * (BITS + 13)];                \
  INDEX##BITS stride_##DATA_TYPE##_##BITS = (BITS - 3);                        \
  INDEX##BITS n_##DATA_TYPE##_##BITS = (BITS + 13);                            \
  for (INDEX##BITS i = 0;                                                      \
       i < stride_##DATA_TYPE##_##BITS * n_##DATA_TYPE##_##BITS; i++)          \
    {                                                                          \
      dest_##DATA_TYPE##_##BITS[i]                                             \
	= (DATA_TYPE) ((i * 81 + 735) & (BITS - 1));                           \
      dest2_##DATA_TYPE##_##BITS[i]                                            \
	= (DATA_TYPE) ((i * 81 + 735) & (BITS - 1));                           \
      src_##DATA_TYPE##_##BITS[i]                                              \
	= (DATA_TYPE) ((i * 13 + 9107) & (BITS - 1));                          \
    }                                                                          \
  f_##DATA_TYPE##_##BITS (dest_##DATA_TYPE##_##BITS, src_##DATA_TYPE##_##BITS, \
			  stride_##DATA_TYPE##_##BITS,                         \
			  n_##DATA_TYPE##_##BITS);                             \
  for (int i = 0; i < n_##DATA_TYPE##_##BITS; i++)                             \
    {                                                                          \
      assert (dest_##DATA_TYPE##_##BITS[i * stride_##DATA_TYPE##_##BITS]       \
	      == (src_##DATA_TYPE##_##BITS[i] + BITS));                        \
    }

  RUN_LOOP (int8_t, 8)
  RUN_LOOP (uint8_t, 8)
  RUN_LOOP (int16_t, 8)
  RUN_LOOP (uint16_t, 8)
  RUN_LOOP (_Float16, 8)
  RUN_LOOP (int32_t, 8)
  RUN_LOOP (uint32_t, 8)
  RUN_LOOP (float, 8)
  RUN_LOOP (int64_t, 8)
  RUN_LOOP (uint64_t, 8)
  RUN_LOOP (double, 8)

  RUN_LOOP (int8_t, 16)
  RUN_LOOP (uint8_t, 16)
  RUN_LOOP (int16_t, 16)
  RUN_LOOP (uint16_t, 16)
  RUN_LOOP (_Float16, 16)
  RUN_LOOP (int32_t, 16)
  RUN_LOOP (uint32_t, 16)
  RUN_LOOP (float, 16)
  RUN_LOOP (int64_t, 16)
  RUN_LOOP (uint64_t, 16)
  RUN_LOOP (double, 16)

  RUN_LOOP (int8_t, 32)
  RUN_LOOP (uint8_t, 32)
  RUN_LOOP (int16_t, 32)
  RUN_LOOP (uint16_t, 32)
  RUN_LOOP (_Float16, 32)
  RUN_LOOP (int32_t, 32)
  RUN_LOOP (uint32_t, 32)
  RUN_LOOP (float, 32)
  RUN_LOOP (int64_t, 32)
  RUN_LOOP (uint64_t, 32)
  RUN_LOOP (double, 32)

  RUN_LOOP (int8_t, 64)
  RUN_LOOP (uint8_t, 64)
  RUN_LOOP (int16_t, 64)
  RUN_LOOP (uint16_t, 64)
  RUN_LOOP (_Float16, 64)
  RUN_LOOP (int32_t, 64)
  RUN_LOOP (uint32_t, 64)
  RUN_LOOP (float, 64)
  RUN_LOOP (int64_t, 64)
  RUN_LOOP (uint64_t, 64)
  RUN_LOOP (double, 64)
  return 0;
}
