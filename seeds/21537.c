/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -ffast-math" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -ffast-math --param aarch64-sve-compare-costs=0" } */

#include <stdint.h>

#define TEST_LOOP(NAME, OUTTYPE, INTYPE, MASKTYPE)		\
  void __attribute__ ((noinline, noclone))			\
  NAME##_4 (OUTTYPE *__restrict dest, INTYPE *__restrict src,	\
	    MASKTYPE *__restrict cond, intptr_t n)		\
  {								\
    for (intptr_t i = 0; i < n; ++i)				\
      if (cond[i])						\
	dest[i] = (src[i * 4]					\
		   + src[i * 4 + 1]				\
		   + src[i * 4 + 2]				\
		   + src[i * 4 + 3]);				\
  }

#define TEST2(NAME, OUTTYPE, INTYPE) \
  TEST_LOOP (NAME##_i8, OUTTYPE, INTYPE, int8_t) \
  TEST_LOOP (NAME##_i16, OUTTYPE, INTYPE, uint16_t) \
  TEST_LOOP (NAME##_f32, OUTTYPE, INTYPE, float) \
  TEST_LOOP (NAME##_f64, OUTTYPE, INTYPE, double)

#define TEST1(NAME, OUTTYPE) \
  TEST2 (NAME##_i8, OUTTYPE, int8_t) \
  TEST2 (NAME##_i16, OUTTYPE, uint16_t) \
  TEST2 (NAME##_i32, OUTTYPE, int32_t) \
  TEST2 (NAME##_i64, OUTTYPE, uint64_t)

#define TEST(NAME) \
  TEST1 (NAME##_i8, int8_t) \
  TEST1 (NAME##_i16, uint16_t) \
  TEST1 (NAME##_i32, int32_t) \
  TEST1 (NAME##_i64, uint64_t) \
  TEST2 (NAME##_f16_f16, _Float16, _Float16) \
  TEST2 (NAME##_f32_f32, float, float) \
  TEST2 (NAME##_f64_f64, double, double)

TEST (test)

/*    Mask |  8 16 32 64
    -------+------------
    Out  8 |  1  1  1  1
        16 |  1  1  1  1
        32 |  1  1  1  1
        64 |  1  1  1  1.  */
/* { dg-final { scan-assembler-times {\tld4b\t.z[0-9]} 16 } } */

/*    Mask |  8 16 32 64
    -------+------------
    Out  8 |  2  2  2  2
        16 |  2  1  1  1 x2 (for half float)
        32 |  2  1  1  1
        64 |  2  1  1  1.  */
/* { dg-final { scan-assembler-times {\tld4h\t.z[0-9]} 28 } } */

/*    Mask |  8 16 32 64
    -------+------------
    Out  8 |  4  4  4  4
        16 |  4  2  2  2
        32 |  4  2  1  1 x2 (for float)
        64 |  4  2  1  1.  */
/* { dg-final { scan-assembler-times {\tld4w\t.z[0-9]} 50 } } */

/*    Mask |  8 16 32 64
    -------+------------
    Out  8 |  8  8  8  8
        16 |  8  4  4  4
        32 |  8  4  2  2
        64 |  8  4  2  1 x2 (for double).  */
/* { dg-final { scan-assembler-times {\tld4d\t.z[0-9]} 98 } } */

#define N 100

#undef TEST_LOOP
#define TEST_LOOP(NAME, OUTTYPE, INTYPE, MASKTYPE)	\
  {							\
    OUTTYPE out[N];					\
    INTYPE in[N * 4];					\
    MASKTYPE mask[N];					\
    for (int i = 0; i < N; ++i)				\
      {							\
	out[i] = i * 7 / 2;				\
	mask[i] = i % 5 <= i % 3;			\
	asm volatile ("" ::: "memory");			\
      }							\
    for (int i = 0; i < N * 4; ++i)			\
      in[i] = i * 9 / 2;				\
    NAME##_4 (out, in, mask, N);			\
    for (int i = 0; i < N; ++i)				\
      {							\
	OUTTYPE if_true = (in[i * 4]			\
			   + in[i * 4 + 1]		\
			   + in[i * 4 + 2]		\
			   + in[i * 4 + 3]);		\
	OUTTYPE if_false = i * 7 / 2;			\
	if (out[i] != (mask[i] ? if_true : if_false))	\
	  __builtin_abort ();				\
	asm volatile ("" ::: "memory");			\
      }							\
  }

int __attribute__ ((optimize (1)))
main (void)
{
  TEST (test);
  return 0;
}
