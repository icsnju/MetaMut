/* { dg-do run { target { riscv_vector } } } */


/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d  -fdump-tree-vect-details" } */

#include <stdint-gcc.h>

#define TEST_LOOP(DATA_TYPE)                                                   \
  void __attribute__ ((noinline, noclone))                                     \
  f_##DATA_TYPE (DATA_TYPE *restrict dest, DATA_TYPE *restrict *src)           \
  {                                                                            \
    for (int i = 0; i < 128; ++i)                                              \
      dest[i] += *src[i];                                                      \
  }

#define TEST_ALL(T)                                                            \
  T (int8_t)                                                                   \
  T (uint8_t)                                                                  \
  T (int16_t)                                                                  \
  T (uint16_t)                                                                 \
  T (_Float16)                                                                 \
  T (int32_t)                                                                  \
  T (uint32_t)                                                                 \
  T (float)                                                                    \
  T (int64_t)                                                                  \
  T (uint64_t)                                                                 \
  T (double)

TEST_ALL (TEST_LOOP)

/* { dg-final { scan-tree-dump-times "vectorized 1 loops in function" 11 "vect" } } */
/* { dg-final { scan-tree-dump " \.MASK_LEN_GATHER_LOAD" "vect" } } */
/* { dg-final { scan-tree-dump-not " \.GATHER_LOAD" "vect" } } */
/* { dg-final { scan-tree-dump-not " \.MASK_GATHER_LOAD" "vect" } } */
#include <assert.h>

int
main (void)
{
#define RUN_LOOP(DATA_TYPE, BITS)                                              \
  DATA_TYPE dest_##DATA_TYPE[128];                                             \
  DATA_TYPE dest2_##DATA_TYPE[128];                                            \
  DATA_TYPE *src_##DATA_TYPE[128];                                             \
  DATA_TYPE src2_##DATA_TYPE[128];                                             \
  for (int i = 0; i < 128; i++)                                                \
    {                                                                          \
      dest_##DATA_TYPE[i] = (DATA_TYPE) ((i * 81 + 735) & (BITS - 1));         \
      dest2_##DATA_TYPE[i] = (DATA_TYPE) ((i * 81 + 735) & (BITS - 1));        \
      src2_##DATA_TYPE[i] = (DATA_TYPE) ((i * 13 + 9107) & (BITS - 1));        \
      src_##DATA_TYPE[i] = src2_##DATA_TYPE + i;                               \
    }                                                                          \
  f_##DATA_TYPE (dest_##DATA_TYPE, src_##DATA_TYPE);                           \
  for (int i = 0; i < 128; i++)                                                \
    assert (dest_##DATA_TYPE[i]                                                \
	    == (dest2_##DATA_TYPE[i] + src_##DATA_TYPE[i][0]));

  RUN_LOOP (int8_t, 8)
  RUN_LOOP (uint8_t, 8)
  RUN_LOOP (int16_t, 16)
  RUN_LOOP (uint16_t, 16)
  RUN_LOOP (_Float16, 16)
  RUN_LOOP (int32_t, 32)
  RUN_LOOP (uint32_t, 32)
  RUN_LOOP (float, 32)
  RUN_LOOP (int64_t, 64)
  RUN_LOOP (uint64_t, 64)
  RUN_LOOP (double, 64)
  return 0;
}
