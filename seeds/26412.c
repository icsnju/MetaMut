/* { dg-do run { target { riscv_vector && riscv_zvfh_hw } } } */
/* { dg-additional-options "--param=riscv-autovec-preference=scalable -ffast-math" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=scalable -fno-schedule-insns -ffast-math -fno-vect-cost-model" } */

#include <stdint-gcc.h>

#define TEST_TYPE(TYPE)                                                        \
  __attribute__ ((noipa)) void ternop_##TYPE (TYPE *__restrict dest1,          \
					      TYPE *__restrict dest2,          \
					      TYPE *__restrict dest3,          \
					      TYPE *__restrict src1,           \
					      TYPE *__restrict src2, int n)    \
  {                                                                            \
    for (int i = 0; i < n; ++i)                                                \
      {                                                                        \
	dest1[i] = -(src1[i] * src2[i]) - dest1[i];                            \
	dest2[i] = src1[i] * dest1[i] - dest2[i];                              \
	dest3[i] = src2[i] * dest2[i] - dest3[i];                              \
      }                                                                        \
  }

#define TEST_ALL()                                                             \
  TEST_TYPE (_Float16)                                                         \
  TEST_TYPE (float)                                                            \
  TEST_TYPE (double)

TEST_ALL ()

/* { dg-final { scan-assembler-times {\tvfnmacc\.vv} 3 } } */
/* { dg-final { scan-assembler-not {\tvmv} } } */

#define TEST_LOOP(TYPE, NUM)                                                   \
  {                                                                            \
    TYPE array1_##NUM[NUM] = {};                                               \
    TYPE array2_##NUM[NUM] = {};                                               \
    TYPE array3_##NUM[NUM] = {};                                               \
    TYPE array4_##NUM[NUM] = {};                                               \
    TYPE array5_##NUM[NUM] = {};                                               \
    TYPE array6_##NUM[NUM] = {};                                               \
    TYPE array7_##NUM[NUM] = {};                                               \
    TYPE array8_##NUM[NUM] = {};                                               \
    for (int i = 0; i < NUM; ++i)                                              \
      {                                                                        \
 array1_##NUM[i] = (i & 1) + 5;                                         \
 array2_##NUM[i] = i - NUM / 3;                                         \
 array3_##NUM[i] = NUM - NUM / 3 - i;                                   \
 array6_##NUM[i] = NUM - NUM / 3 - i;                                   \
 array4_##NUM[i] = NUM - NUM / 2 + i;                                   \
 array7_##NUM[i] = NUM - NUM / 2 + i;                                   \
 array5_##NUM[i] = NUM + i * 7;                                         \
 array8_##NUM[i] = NUM + i * 7;                                         \
 asm volatile("" ::: "memory");                                         \
      }                                                                        \
    ternop_##TYPE (array3_##NUM, array4_##NUM, array5_##NUM, array1_##NUM,     \
    array2_##NUM, NUM);                                         \
    for (int i = 0; i < NUM; i++)                                              \
      {                                                                        \
 array6_##NUM[i]                                                        \
   = (TYPE) (-(array1_##NUM[i] * array2_##NUM[i]) - array6_##NUM[i]);   \
 if (array3_##NUM[i] != array6_##NUM[i])                                \
   __builtin_abort ();                                                  \
 array7_##NUM[i]                                                        \
   = (TYPE) (array1_##NUM[i] * array6_##NUM[i] - array7_##NUM[i]);      \
 if (array4_##NUM[i] != array7_##NUM[i])                                \
   __builtin_abort ();                                                  \
 array8_##NUM[i]                                                        \
   = (TYPE) (array2_##NUM[i] * array7_##NUM[i] - array8_##NUM[i]);      \
 if (array5_##NUM[i] != array8_##NUM[i])                                \
   __builtin_abort ();                                                  \
      }                                                                        \
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
