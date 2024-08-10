/* { dg-do compile } */
/* { dg-additional-options "-march=rv64gcv_zvfh -mabi=lp64d --param riscv-autovec-preference=scalable -fno-vect-cost-model" } */

#include <stdint.h>

#define abs(A) ((A) < 0 ? -(A) : (A))
#define neg(A) (-(A))
#define not(A) (~(A))

#define DEF_LOOP(TYPE, OP)                                                     \
  void __attribute__ ((noipa))                                                 \
  test_##TYPE##_##OP (TYPE *__restrict r, TYPE *__restrict a,                  \
		      TYPE *__restrict pred, int n)                            \
  {                                                                            \
    for (int i = 0; i < n; ++i)                                                \
      r[i] = pred[i] ? OP (a[i]) : 0;                                          \
  }

#define TEST_INT_TYPE(T, TYPE)                                                 \
  T (TYPE, abs)                                                                \
  T (TYPE, neg)                                                                \
  T (TYPE, not )

#define TEST_FLOAT_TYPE(T, TYPE, SUFFIX)                                       \
  T (TYPE, __builtin_fabs##SUFFIX)                                             \
  T (TYPE, neg)

#define TEST_ALL(T)                                                            \
  TEST_INT_TYPE (T, int8_t)                                                    \
  TEST_INT_TYPE (T, int16_t)                                                   \
  TEST_INT_TYPE (T, int32_t)                                                   \
  TEST_INT_TYPE (T, int64_t)                                                   \
  TEST_FLOAT_TYPE (T, _Float16, f16)                                           \
  TEST_FLOAT_TYPE (T, float, f)                                                \
  TEST_FLOAT_TYPE (T, double, )

TEST_ALL (DEF_LOOP)

/* NOTE: int abs operator is converted to vmslt + vneg.v */
/* { dg-final { scan-assembler-times {\tvneg\.v\tv[0-9]+,v[0-9]+,v0\.t} 8 } } */
/* { dg-final { scan-assembler-times {\tvnot\.v\tv[0-9]+,v[0-9]+,v0\.t} 4 } } */
/* { dg-final { scan-assembler-times {\tvfabs\.v\tv[0-9]+,v[0-9]+,v0\.t} 3 } } */
/* { dg-final { scan-assembler-times {\tvfneg\.v\tv[0-9]+,v[0-9]+,v0\.t} 3 } } */
