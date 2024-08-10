/* { dg-do run { target { riscv_vector && riscv_zvfh_hw } } } */
/* { dg-additional-options "--param=riscv-autovec-preference=scalable -ffast-math" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=scalable -ffast-math -fno-vect-cost-model" } */

#include <stdint-gcc.h>

#define TEST_TYPE(TYPE)                                                        \
  __attribute__ ((noipa)) void ternop_##TYPE (TYPE *__restrict dst,            \
					      TYPE *__restrict a,              \
					      TYPE *__restrict b, int n)       \
  {                                                                            \
    for (int i = 0; i < n; i++)                                                \
      dst[i] += a[i] * b[i];                                                   \
  }

#define TEST_ALL()                                                             \
  TEST_TYPE (int8_t)                                                           \
  TEST_TYPE (uint8_t)                                                          \
  TEST_TYPE (int16_t)                                                          \
  TEST_TYPE (uint16_t)                                                         \
  TEST_TYPE (int32_t)                                                          \
  TEST_TYPE (uint32_t)                                                         \
  TEST_TYPE (int64_t)                                                          \
  TEST_TYPE (uint64_t)                                                         \
  TEST_TYPE (_Float16)                                                         \
  TEST_TYPE (float)                                                            \
  TEST_TYPE (double)

TEST_ALL ()

/* { dg-final { scan-assembler-times {\tvmadd\.vv} 8 } } */
/* { dg-final { scan-assembler-times {\tvfmadd\.vv} 3 } } */
/* { dg-final { scan-assembler-not {\tvmv} } } */

#define TEST_LOOP(TYPE, NUM)                                                   \
  {                                                                            \
    TYPE array1_##NUM[NUM] = {};                                               \
    TYPE array2_##NUM[NUM] = {};                                               \
    TYPE array3_##NUM[NUM] = {};                                               \
    TYPE array4_##NUM[NUM] = {};                                               \
    for (int i = 0; i < NUM; ++i)                                              \
      {                                                                        \
	array1_##NUM[i] = (i & 1) + 5;                                         \
	array2_##NUM[i] = i - NUM / 3;                                         \
	array3_##NUM[i] = NUM - NUM / 3 - i;                                   \
	array4_##NUM[i] = NUM - NUM / 3 - i;                                   \
	asm volatile("" ::: "memory");                                         \
      }                                                                        \
    ternop_##TYPE (array3_##NUM, array1_##NUM, array2_##NUM, NUM);             \
    for (int i = 0; i < NUM; i++)                                              \
      if (array3_##NUM[i]                                                      \
	  != (TYPE) (array1_##NUM[i] * array2_##NUM[i] + array4_##NUM[i]))     \
	__builtin_abort ();                                                    \
  }

int __attribute__ ((optimize (0))) main ()
{
  TEST_LOOP (_Float16, 7)
  TEST_LOOP (_Float16, 16)
  TEST_LOOP (_Float16, 77)
  TEST_LOOP (_Float16, 128)
  TEST_LOOP (_Float16, 795)
  return 0;
}
