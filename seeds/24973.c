/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d --param riscv-autovec-preference=scalable -fno-vect-cost-model -ffast-math" } */

#include <stdint.h>

#define DEF_LOOP(OLD_TYPE, NEW_TYPE)                                           \
  void __attribute__ ((noipa))                                                 \
  test_##OLD_TYPE##_2_##NEW_TYPE (NEW_TYPE *__restrict r,                      \
				  OLD_TYPE *__restrict a, NEW_TYPE b,          \
				  OLD_TYPE *__restrict pred, int n)            \
  {                                                                            \
    for (int i = 0; i < n; ++i)                                                \
      {                                                                        \
	r[i] = pred[i] ? (NEW_TYPE) a[i] : b;                                  \
      }                                                                        \
  }

/* FP -> INT */
#define TEST_ALL_F2X_SAME(T)                                                   \
  T (_Float16, uint16_t)                                                       \
  T (_Float16, int16_t)                                                        \
  T (float, uint32_t)                                                          \
  T (float, int32_t)                                                           \
  T (double, uint64_t)                                                         \
  T (double, int64_t)

/* FP -> wider-INT */
#define TEST_ALL_F2X_WIDER(T)                                                  \
  T (_Float16, uint32_t)                                                       \
  T (_Float16, int32_t)                                                        \
  T (_Float16, uint64_t)                                                       \
  T (_Float16, int64_t)                                                        \
  T (float, uint64_t)                                                          \
  T (float, int64_t)

/* FP -> narrower-INT */
#define TEST_ALL_F2X_NARROWER(T)                                               \
  T (_Float16, uint8_t)                                                        \
  T (_Float16, int8_t)                                                         \
  T (float, uint8_t)                                                           \
  T (float, int8_t)                                                            \
  T (float, uint16_t)                                                          \
  T (float, int16_t)                                                           \
  T (double, uint8_t)                                                          \
  T (double, int8_t)                                                           \
  T (double, uint16_t)                                                         \
  T (double, int16_t)                                                          \
  T (double, uint32_t)                                                         \
  T (double, int32_t)

TEST_ALL_F2X_SAME (DEF_LOOP)
TEST_ALL_F2X_WIDER (DEF_LOOP)
TEST_ALL_F2X_NARROWER (DEF_LOOP)

/* { dg-final { scan-assembler-times {\tvfcvt\.rtz\.xu\.f\.v\tv[0-9]+,v[0-9]+,v0\.t} 3 } } */
/* { dg-final { scan-assembler-times {\tvfcvt\.rtz\.x\.f\.v\tv[0-9]+,v[0-9]+,v0\.t} 3 } } */

/* { dg-final { scan-assembler-times {\tvfwcvt\.rtz\.xu\.f\.v\tv[0-9]+,v[0-9]+,v0\.t} 2 } } */
/* { dg-final { scan-assembler-times {\tvfwcvt\.rtz\.x\.f\.v\tv[0-9]+,v[0-9]+,v0\.t} 2 } } */
/* { dg-final { scan-assembler-times {\tvfwcvt\.rtz\.x\.f\.v\tv[0-9]+,v[0-9]+\n} 2 } } */

/* { dg-final { scan-assembler-times {\tvfncvt\.rtz\.xu\.f\.w\tv[0-9]+,v[0-9]+,v0\.t} 3 } } */
/* { dg-final { scan-assembler-times {\tvfncvt\.rtz\.x\.f\.w\tv[0-9]+,v[0-9]+,v0\.t} 3 } } */
/* { dg-final { scan-assembler-times {\tvfncvt\.rtz\.x\.f\.w\tv[0-9]+,v[0-9]+\n} 6 } } */

/* { dg-final { scan-assembler {\tvsetvli\t[a-z0-9]+,[a-z0-9]+,e[0-9]+,m[f0-9]+,t[au],mu} } } */
