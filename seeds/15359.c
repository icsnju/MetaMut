/* { dg-do compile } */
/* { dg-additional-options "-std=c99 -O3 -fno-vect-cost-model -march=rv64gcv_zvfh -mabi=lp64d --param=riscv-autovec-preference=fixed-vlmax -ffast-math" } */

#include <stdint-gcc.h>

#define TEST_TYPE(TYPE, SUFFIX)                                                \
  __attribute__ ((noipa)) void copysign_##TYPE (TYPE *restrict dst,            \
						TYPE *restrict a,              \
						TYPE *restrict b,              \
						int *restrict pred, int n)     \
  {                                                                            \
    for (int i = 0; i < n; i++)                                                \
      dst[i] = pred[i] ? __builtin_copysign##SUFFIX (a[i], b[i]) : dst[i];     \
  }

#define TEST_TYPE2(TYPE, SUFFIX)                                               \
  __attribute__ ((noipa)) void copysigns_##TYPE (TYPE *restrict dst,           \
						 TYPE *restrict a, TYPE b,     \
						 int *restrict pred, int n)    \
  {                                                                            \
    for (int i = 0; i < n; i++)                                                \
      dst[i] = pred[i] ? __builtin_copysign##SUFFIX (a[i], b) : dst[i];        \
  }

#define TEST_TYPE3(TYPE, SUFFIX)                                               \
  __attribute__ ((noipa)) void xorsign_##TYPE (TYPE *restrict dst,             \
					       TYPE *restrict a,               \
					       TYPE *restrict b,               \
					       int *restrict pred, int n)      \
  {                                                                            \
    for (int i = 0; i < n; i++)                                                \
      dst[i]                                                                   \
	= pred[i] ? a[i] * __builtin_copysign##SUFFIX (1.0, b[i]) : dst[i];    \
  }

#define TEST_TYPE4(TYPE, SUFFIX)                                               \
  __attribute__ ((noipa)) void xorsigns_##TYPE (TYPE *restrict dst,            \
						TYPE *restrict a, TYPE b,      \
						int *restrict pred, int n)     \
  {                                                                            \
    for (int i = 0; i < n; i++)                                                \
      dst[i] = pred[i] ? a[i] * __builtin_copysign##SUFFIX (1.0, b) : dst[i];  \
  }

#define TEST_TYPE5(TYPE, SUFFIX)                                               \
  __attribute__ ((noipa)) void ncopysign_##TYPE (TYPE *restrict dst,           \
						 TYPE *restrict a,             \
						 TYPE *restrict b,             \
						 int *restrict pred, int n)    \
  {                                                                            \
    for (int i = 0; i < n; i++)                                                \
      dst[i] = pred[i] ? -__builtin_copysign##SUFFIX (a[i], b[i]) : dst[i];    \
  }

#define TEST_TYPE6(TYPE, SUFFIX)                                               \
  __attribute__ ((noipa)) void ncopysigns_##TYPE (TYPE *restrict dst,          \
						  TYPE *restrict a, TYPE b,    \
						  int *restrict pred, int n)   \
  {                                                                            \
    for (int i = 0; i < n; i++)                                                \
      dst[i] = pred[i] ? -__builtin_copysign##SUFFIX (a[i], b) : dst[i];       \
  }

#define TEST_ALL()                                                             \
  TEST_TYPE (_Float16, f16)                                                    \
  TEST_TYPE (float, f)                                                         \
  TEST_TYPE (double, )                                                         \
  TEST_TYPE2 (_Float16, f16)                                                   \
  TEST_TYPE2 (float, f)                                                        \
  TEST_TYPE2 (double, )                                                        \
  TEST_TYPE3 (_Float16, f16)                                                   \
  TEST_TYPE3 (float, f)                                                        \
  TEST_TYPE3 (double, )                                                        \
  TEST_TYPE4 (_Float16, f16)                                                   \
  TEST_TYPE4 (float, f)                                                        \
  TEST_TYPE4 (double, )                                                        \
  TEST_TYPE5 (_Float16, f16)                                                   \
  TEST_TYPE5 (float, f)                                                        \
  TEST_TYPE5 (double, )                                                        \
  TEST_TYPE6 (_Float16, f16)                                                   \
  TEST_TYPE6 (float, f)                                                        \
  TEST_TYPE6 (double, )

TEST_ALL ()

/* { dg-final { scan-assembler-times {\tvfsgnj\.vv} 6 } } */
/* 1. The vectorizer wraps scalar variants of copysign into vector constants which
      expand cannot handle currently.
   2. match.pd convert .COPYSIGN (1, b) + COND_MUL to AND + XOR currently.  */
/* { dg-final { scan-assembler-times {\tvfsgnjx\.vv} 6 { xfail riscv*-*-* } } } */
/* { dg-final { scan-assembler-times {\tvfsgnjn\.vv} 6 } } */
/* { dg-final { scan-assembler-not {\tvmerge\.vvm} } } */
