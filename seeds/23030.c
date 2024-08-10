/* { dg-do assemble { target aarch64_asm_sve_ok } } */
/* { dg-options "-O2 -ftree-vectorize --save-temps" } */

#define INDEX8 uint8_t
#define INDEX16 uint16_t
#define INDEX32 uint32_t
#define INDEX64 uint64_t

/* { dg-do assemble { target aarch64_asm_sve_ok } } */
/* { dg-options "-O2 -ftree-vectorize --save-temps" } */

#include <stdint.h>

#ifndef INDEX8
#define INDEX8 int8_t
#define INDEX16 int16_t
#define INDEX32 int32_t
#define INDEX64 int64_t
#endif

#define TEST_LOOP(DATA_TYPE, BITS)				\
  void __attribute__ ((noinline, noclone))			\
  f_##DATA_TYPE##_##BITS (DATA_TYPE *restrict dest,		\
			  DATA_TYPE *restrict src,		\
			  INDEX##BITS stride, INDEX##BITS n)	\
  {								\
    for (INDEX##BITS i = 0; i < n; ++i)				\
      dest[i * stride] = src[i] + 1;				\
  }

#define TEST_TYPE(T, DATA_TYPE)			\
  T (DATA_TYPE, 8)				\
  T (DATA_TYPE, 16)				\
  T (DATA_TYPE, 32)				\
  T (DATA_TYPE, 64)

#define TEST_ALL(T)				\
  TEST_TYPE (T, int32_t)			\
  TEST_TYPE (T, uint32_t)			\
  TEST_TYPE (T, float)				\
  TEST_TYPE (T, int64_t)			\
  TEST_TYPE (T, uint64_t)			\
  TEST_TYPE (T, double)

TEST_ALL (TEST_LOOP)

/* { dg-final { scan-assembler-times {\tst1w\tz[0-9]+\.s, p[0-7], \[x[0-9]+, z[0-9]+.s, sxtw 2\]\n} 9 } } */
/* { dg-final { scan-assembler-times {\tst1d\tz[0-9]+\.d, p[0-7], \[x[0-9]+, z[0-9]+.d, lsl 3\]\n} 12 } } */

/* 8 and 16 bits are signed because the multiplication promotes to int.
   Using uxtw for all 9 would be OK.  */
/* { dg-final { scan-assembler-times {\tst1w\tz[0-9]+\.s, p[0-7], \[x[0-9]+, z[0-9]+.s, sxtw 2\]\n} 6 } } */
/* The 32-bit loop needs to honor the defined overflow in uint32_t,
   so we vectorize the offset calculation.  This means that the
   64-bit version needs two copies.  */
/* { dg-final { scan-assembler-times {\tst1w\tz[0-9]+\.s, p[0-7], \[x[0-9]+, z[0-9]+.s, uxtw 2\]\n} 3 } } */
/* { dg-final { scan-assembler-times {\tst1d\tz[0-9]+\.d, p[0-7], \[x[0-9]+, z[0-9]+.d, lsl 3\]\n} 15 } } */
