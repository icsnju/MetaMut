/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param=riscv-autovec-preference=scalable -fno-vect-cost-model" } */

/* { dg-do compile } */
/* { dg-additional-options "-std=c99 -march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=scalable -fno-vect-cost-model" } */

#include <stdint-gcc.h>

#define TEST_LOOP(NAME, OUTTYPE, INTYPE, MASKTYPE)		\
  void __attribute__ ((noinline, noclone))			\
  NAME##_6 (OUTTYPE *__restrict dest, INTYPE *__restrict src,	\
	    MASKTYPE *__restrict cond, INTYPE bias, intptr_t n)	\
  {								\
    for (intptr_t i = 0; i < n; ++i)				\
      {								\
	INTYPE value = src[i] + bias;				\
	if (cond[i])						\
	  {							\
	    dest[i * 6] = value;				\
	    dest[i * 6 + 1] = value;				\
	    dest[i * 6 + 2] = value;				\
	    dest[i * 6 + 3] = value;				\
      dest[i * 6 + 4] = value;				\
      dest[i * 6 + 5] = value;				\
	  }							\
      }								\
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

/* { dg-final { scan-assembler-times {vsseg6e8\.v\s+v[0-9]+,\s*\([a-x0-9]+\),\s*v0.t} 16 } } */
/* { dg-final { scan-assembler-times {vsseg6e16\.v\s+v[0-9]+,\s*\([a-x0-9]+\),\s*v0.t} 20 } } */
/* { dg-final { scan-assembler-times {vsseg6e32\.v\s+v[0-9]+,\s*\([a-x0-9]+\),\s*v0.t} 20 } } */
/* { dg-final { scan-assembler-times {vsseg6e64\.v\s+v[0-9]+,\s*\([a-x0-9]+\),\s*v0.t} 20 } } */

#define N 100

#undef TEST_LOOP
#define TEST_LOOP(NAME, OUTTYPE, INTYPE, MASKTYPE)		\
  {								\
    OUTTYPE out[N * 6];						\
    INTYPE in[N];						\
    MASKTYPE mask[N];						\
    for (int i = 0; i < N; ++i)					\
      {								\
	in[i] = i * 7 / 2;					\
	mask[i] = i % 5 <= i % 3;				\
	asm volatile ("" ::: "memory");				\
      }								\
    for (int i = 0; i < N * 6; ++i)				\
      out[i] = i * 9 / 2;					\
    NAME##_6 (out, in, mask, 42, N);				\
    for (int i = 0; i < N * 6; ++i)				\
      {								\
	OUTTYPE if_true = (INTYPE) (in[i / 6] + 42);		\
	OUTTYPE if_false = i * 9 / 2;				\
	if (out[i] != (mask[i / 6] ? if_true : if_false))	\
	  __builtin_abort ();					\
	asm volatile ("" ::: "memory");				\
      }								\
  }

int __attribute__ ((optimize (1)))
main (void)
{
  TEST (test);
  return 0;
}