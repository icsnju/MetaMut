/* { dg-do compile } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model -march=rv64gcv -mabi=lp64d --param=riscv-autovec-preference=fixed-vlmax -fdump-tree-optimized-details" } */

#include <stdint-gcc.h>

#define TEST_TYPE(TYPE) 				\
  __attribute__((noipa))				\
  void vrem_##TYPE (TYPE *dst, TYPE *a, TYPE *b, int n)	\
  {							\
    for (int i = 0; i < n; i++)				\
      dst[i] = a[i] % b[i];				\
  }

#define TEST2_TYPE(TYPE) 				\
  __attribute__((noipa))				\
  void vrems_##TYPE (TYPE *dst, TYPE *a, TYPE b, int n)	\
  {							\
    for (int i = 0; i < n; i++)				\
      dst[i] = a[i] % b;				\
  }

#define TEST_ALL()	\
 TEST_TYPE(int8_t)	\
 TEST_TYPE(uint8_t)	\
 TEST_TYPE(int16_t)	\
 TEST_TYPE(uint16_t)	\
 TEST_TYPE(int32_t)	\
 TEST_TYPE(uint32_t)	\
 TEST_TYPE(int64_t)	\
 TEST_TYPE(uint64_t)    \
 TEST2_TYPE(int8_t)	\
 TEST2_TYPE(uint8_t)	\
 TEST2_TYPE(int16_t)	\
 TEST2_TYPE(uint16_t)	\
 TEST2_TYPE(int32_t)	\
 TEST2_TYPE(uint32_t)	\
 TEST2_TYPE(int64_t)	\
 TEST2_TYPE(uint64_t)

TEST_ALL()

/* { dg-final { scan-assembler-times {\tvrem\.vv} 8 } } */
/* { dg-final { scan-assembler-times {\tvremu\.vv} 8 } } */
/* { dg-final { scan-tree-dump-times "\.COND_LEN_MOD" 16 "optimized" } } */
/* { dg-final { scan-assembler-not {\tvmv1r\.v} } } */
/* { dg-final { scan-assembler-not {\tvmv2r\.v} } } */
/* { dg-final { scan-assembler-not {\tvmv4r\.v} } } */
/* { dg-final { scan-assembler-not {\tvmv8r\.v} } } */
/* { dg-final { scan-assembler-not {\tvmv\.v\.v} } } */
/* { dg-final { scan-assembler-not {\tvmv\.v\.i} } } */
