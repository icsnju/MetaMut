/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param=riscv-autovec-preference=scalable" } */
/* { dg-require-effective-target fenv_exceptions } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv -mabi=ilp32d --param=riscv-autovec-preference=scalable -fno-vect-cost-model" } */

#include <stdint-gcc.h>

#define eq(A, B) ((A) == (B))
#define ne(A, B) ((A) != (B))
#define olt(A, B) ((A) < (B))
#define ole(A, B) ((A) <= (B))
#define oge(A, B) ((A) >= (B))
#define ogt(A, B) ((A) > (B))
#define ordered(A, B) (!__builtin_isunordered (A, B))
#define unordered(A, B) (__builtin_isunordered (A, B))
#define ueq(A, B) (!__builtin_islessgreater (A, B))
#define ult(A, B) (__builtin_isless (A, B))
#define ule(A, B) (__builtin_islessequal (A, B))
#define uge(A, B) (__builtin_isgreaterequal (A, B))
#define ugt(A, B) (__builtin_isgreater (A, B))
#define nueq(A, B) (__builtin_islessgreater (A, B))
#define nult(A, B) (!__builtin_isless (A, B))
#define nule(A, B) (!__builtin_islessequal (A, B))
#define nuge(A, B) (!__builtin_isgreaterequal (A, B))
#define nugt(A, B) (!__builtin_isgreater (A, B))

#define TEST_LOOP(TYPE1, TYPE2, CMP)				\
  void __attribute__ ((noinline, noclone))			\
  test_##TYPE1##_##TYPE2##_##CMP##_var (TYPE1 *restrict dest,	\
					TYPE1 *restrict src,	\
					TYPE1 fallback,		\
					TYPE2 *restrict a,	\
					TYPE2 *restrict b,	\
					int count)		\
  {								\
    for (int i = 0; i < count; ++i)				\
      {\
        TYPE2 aval = a[i]; \
        TYPE2 bval = b[i]; \
        TYPE1 srcval = src[i]; \
        dest[i] = CMP (aval, bval) ? srcval : fallback;		\
      }\
  }

#define TEST_CMP(CMP) \
  TEST_LOOP (int32_t, float, CMP) \
  TEST_LOOP (uint32_t, float, CMP) \
  TEST_LOOP (float, float, CMP) \
  TEST_LOOP (int64_t, double, CMP) \
  TEST_LOOP (uint64_t, double, CMP) \
  TEST_LOOP (double, double, CMP)

TEST_CMP (eq)
TEST_CMP (ne)
TEST_CMP (olt)
TEST_CMP (ole)
TEST_CMP (oge)
TEST_CMP (ogt)
TEST_CMP (ordered)
TEST_CMP (unordered)
TEST_CMP (ueq)
TEST_CMP (ult)
TEST_CMP (ule)
TEST_CMP (uge)
TEST_CMP (ugt)
TEST_CMP (nueq)
TEST_CMP (nult)
TEST_CMP (nule)
TEST_CMP (nuge)
TEST_CMP (nugt)

/* { dg-final { scan-assembler-times {\tvmfeq} 150 } } */
/* { dg-final { scan-assembler-times {\tvmfne} 6 } } */
/* { dg-final { scan-assembler-times {\tvmfgt} 30 } } */
/* { dg-final { scan-assembler-times {\tvmflt} 30 } } */
/* { dg-final { scan-assembler-times {\tvmfge} 18 } } */
/* { dg-final { scan-assembler-times {\tvmfle} 18 } } */

#ifndef TEST_EXCEPTIONS
#define TEST_EXCEPTIONS 1
#endif

#include <fenv.h>

#define N 401

#define RUN_LOOP(TYPE1, TYPE2, CMP, EXPECT_INVALID)			\
  {									\
    TYPE1 dest[N], src[N];						\
    TYPE2 a[N], b[N];							\
    for (int i = 0; i < N; ++i)						\
      {									\
	src[i] = i * i;							\
	if (i % 5 == 0)							\
	  a[i] = 0;							\
	else if (i % 3)							\
	  a[i] = i * 0.1;						\
	else								\
	  a[i] = i;							\
	if (i % 7 == 0)							\
	  b[i] = __builtin_nan ("");					\
	else if (i % 6)							\
	  b[i] = i * 0.1;						\
	else								\
	  b[i] = i;							\
	asm volatile ("" ::: "memory");					\
      }									\
    feclearexcept (FE_ALL_EXCEPT);					\
    test_##TYPE1##_##TYPE2##_##CMP##_var (dest, src, 11, a, b, N);	\
    if (TEST_EXCEPTIONS							\
	&& !fetestexcept (FE_INVALID) != !(EXPECT_INVALID))		\
      __builtin_abort ();						\
    for (int i = 0; i < N; ++i)						\
      if (dest[i] != (CMP (a[i], b[i]) ? src[i] : 11))			\
	__builtin_abort ();						\
  }

#define RUN_CMP(CMP, EXPECT_INVALID) \
  RUN_LOOP (int32_t, float, CMP, EXPECT_INVALID) \
  RUN_LOOP (uint32_t, float, CMP, EXPECT_INVALID) \
  RUN_LOOP (float, float, CMP, EXPECT_INVALID) \
  RUN_LOOP (int64_t, double, CMP, EXPECT_INVALID) \
  RUN_LOOP (uint64_t, double, CMP, EXPECT_INVALID) \
  RUN_LOOP (double, double, CMP, EXPECT_INVALID)

int __attribute__ ((optimize (1)))
main (void)
{
  RUN_CMP (eq, 0)
  RUN_CMP (ne, 0)
  RUN_CMP (olt, 1)
  RUN_CMP (ole, 1)
  RUN_CMP (oge, 1)
  RUN_CMP (ogt, 1)
  RUN_CMP (ordered, 0)
  RUN_CMP (unordered, 0)
  RUN_CMP (ueq, 0)
  RUN_CMP (ult, 0)
  RUN_CMP (ule, 0)
  RUN_CMP (uge, 0)
  RUN_CMP (ugt, 0)
  RUN_CMP (nueq, 0)
  RUN_CMP (nult, 0)
  RUN_CMP (nule, 0)
  RUN_CMP (nuge, 0)
  RUN_CMP (nugt, 0)
  return 0;
}
