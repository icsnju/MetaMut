/* { dg-do run { target { riscv_vector } } } */


/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d  -fdump-tree-vect-details" } */

#include <stdint-gcc.h>

#define INDEX16 uint16_t

#define TEST_LOOP(DATA_TYPE, BITS)                                             \
  void __attribute__ ((noinline, noclone))                                     \
  f_##DATA_TYPE (DATA_TYPE *restrict dest, DATA_TYPE *restrict src,            \
		 INDEX##BITS *restrict indices, INDEX##BITS *restrict cond)    \
  {                                                                            \
    for (int i = 0; i < 128; ++i)                                              \
      if (cond[i])                                                             \
	dest[indices[i]] = src[i] + 1;                                         \
  }

#define TEST_ALL(T)                                                            \
  T (int8_t, 16)                                                                \
  T (uint8_t, 16)                                                               \
  T (int16_t, 16)                                                              \
  T (uint16_t, 16)                                                             \
  T (_Float16, 16)                                                             \
  T (int32_t, 16)                                                              \
  T (uint32_t, 16)                                                             \
  T (float, 16)                                                                \
  T (int64_t, 16)                                                              \
  T (uint64_t, 16)                                                             \
  T (double, 16)

TEST_ALL (TEST_LOOP)

/* { dg-final { scan-tree-dump-times "vectorized 1 loops in function" 11 "vect" } } */
/* { dg-final { scan-tree-dump " \.MASK_LEN_SCATTER_STORE" "vect" } } */
/* { dg-final { scan-tree-dump-not " \.SCATTER_STORE" "vect" } } */
/* { dg-final { scan-tree-dump-not " \.MASK_SCATTER_STORE" "vect" } } */
#include <assert.h>

int
main (void)
{
#define RUN_LOOP(DATA_TYPE, BITS)                                              \
  DATA_TYPE dest_##DATA_TYPE[128];                                             \
  DATA_TYPE dest2_##DATA_TYPE[128];                                            \
  DATA_TYPE src_##DATA_TYPE[128];                                              \
  INDEX##BITS indices_##DATA_TYPE##_##BITS[128];                               \
  INDEX##BITS cond_##DATA_TYPE##_##BITS[128] = {0};                            \
  for (int i = 0; i < 128; i++)                                                \
    {                                                                          \
      dest_##DATA_TYPE[i] = (DATA_TYPE) ((i * 81 + 735) & (BITS - 1));         \
      dest2_##DATA_TYPE[i] = (DATA_TYPE) ((i * 81 + 735) & (BITS - 1));        \
      src_##DATA_TYPE[i] = (DATA_TYPE) ((i * 13 + 9107) & (BITS - 1));         \
      indices_##DATA_TYPE##_##BITS[i] = (DATA_TYPE) ((i * 3 + 67) % 128);      \
      cond_##DATA_TYPE##_##BITS[i] = (DATA_TYPE) ((i & 0x3) == 3);             \
    }                                                                          \
  f_##DATA_TYPE (dest_##DATA_TYPE, src_##DATA_TYPE,                            \
		 indices_##DATA_TYPE##_##BITS, cond_##DATA_TYPE##_##BITS);     \
  for (int i = 0; i < 128; i++)                                                \
    {                                                                          \
      if (cond_##DATA_TYPE##_##BITS[i])                                        \
	assert (dest_##DATA_TYPE[indices_##DATA_TYPE##_##BITS[i]]              \
		== (src_##DATA_TYPE[i] + 1));                                  \
      else                                                                     \
	assert (dest_##DATA_TYPE[indices_##DATA_TYPE##_##BITS[i]]              \
		== dest2_##DATA_TYPE[indices_##DATA_TYPE##_##BITS[i]]);        \
    }

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
  return 0;
}
