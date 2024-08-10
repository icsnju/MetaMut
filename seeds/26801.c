/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d --param riscv-autovec-preference=scalable -fno-vect-cost-model" } */

#include <stdint.h>

#define DEF_LOOP(OLD_TYPE, NEW_TYPE)                                           \
  void __attribute__ ((noipa))                                                 \
  test_##OLD_TYPE##_2_##NEW_TYPE (NEW_TYPE *__restrict r,                      \
				  OLD_TYPE *__restrict a,                      \
				  NEW_TYPE *__restrict b,                      \
				  OLD_TYPE *__restrict pred, int n)            \
  {                                                                            \
    for (int i = 0; i < n; ++i)                                                \
      {                                                                        \
	r[i] = pred[i] ? (NEW_TYPE) a[i] : b[i];                               \
      }                                                                        \
  }

/* INT -> wider-INT */
#define TEST_ALL_X2X_WIDER(T)                                                  \
  T (uint8_t, uint16_t)                                                        \
  T (uint8_t, uint32_t)                                                        \
  T (uint8_t, uint64_t)                                                        \
  T (int8_t, int16_t)                                                          \
  T (int8_t, int32_t)                                                          \
  T (int8_t, int64_t)                                                          \
  T (uint16_t, uint32_t)                                                       \
  T (uint16_t, uint64_t)                                                       \
  T (int16_t, int32_t)                                                         \
  T (int16_t, int64_t)                                                         \
  T (uint32_t, uint64_t)                                                       \
  T (int32_t, int64_t)

/* INT -> narrower-INT */
#define TEST_ALL_X2X_NARROWER(T)                                               \
  T (uint16_t, uint8_t)                                                        \
  T (int16_t, int8_t)                                                          \
  T (uint32_t, uint8_t)                                                        \
  T (int32_t, int8_t)                                                          \
  T (uint64_t, uint8_t)                                                        \
  T (int64_t, int8_t)                                                          \
  T (uint32_t, uint16_t)                                                       \
  T (int32_t, int16_t)                                                         \
  T (uint64_t, uint16_t)                                                       \
  T (int64_t, int16_t)                                                         \
  T (uint64_t, uint32_t)                                                       \
  T (int64_t, int32_t)

TEST_ALL_X2X_WIDER (DEF_LOOP)
TEST_ALL_X2X_NARROWER (DEF_LOOP)

/* { dg-final { scan-assembler-times {\tvzext\.vf2\tv[0-9]+,v[0-9]+,v0\.t} 3 } } */
/* { dg-final { scan-assembler-times {\tvsext\.vf2\tv[0-9]+,v[0-9]+,v0\.t} 3 } } */
/* { dg-final { scan-assembler-times {\tvzext\.vf4\tv[0-9]+,v[0-9]+,v0\.t} 2 } } */
/* { dg-final { scan-assembler-times {\tvsext\.vf4\tv[0-9]+,v[0-9]+,v0\.t} 2 } } */
/* { dg-final { scan-assembler-times {\tvzext\.vf8\tv[0-9]+,v[0-9]+,v0\.t} 1 } } */
/* { dg-final { scan-assembler-times {\tvsext\.vf8\tv[0-9]+,v[0-9]+,v0\.t} 1 } } */

/* { dg-final { scan-assembler-times {\tvncvt\.x\.x\.w\tv[0-9]+,v[0-9]+,v0\.t} 12 } } */
/* { dg-final { scan-assembler-times {\tvncvt\.x\.x\.w\tv[0-9]+,v[0-9]+\n} 8 } } */

/* { dg-final { scan-assembler {\tvsetvli\t[a-z0-9]+,[a-z0-9]+,e[0-9]+,m[f0-9]+,t[au],mu} } } */

