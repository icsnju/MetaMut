/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize" } */

#include <stdint.h>

#define abs(A) ((A) < 0 ? -(A) : (A))
#define neg(A) (-(A))

#define DEF_LOOP(TYPE, OP)					\
  void __attribute__ ((noipa))					\
  test_##TYPE##_##OP (TYPE *__restrict r, TYPE *__restrict a,	\
		      TYPE *__restrict pred, int n)		\
  {								\
    for (int i = 0; i < n; ++i)					\
      r[i] = pred[i] ? OP (a[i]) : 0;				\
  }

#define TEST_INT_TYPE(T, TYPE) \
  T (TYPE, abs) \
  T (TYPE, neg)

#define TEST_FLOAT_TYPE(T, TYPE, SUFFIX) \
  T (TYPE, __builtin_fabs##SUFFIX) \
  T (TYPE, neg)

#define TEST_ALL(T) \
  TEST_INT_TYPE (T, int8_t) \
  TEST_INT_TYPE (T, int16_t) \
  TEST_INT_TYPE (T, int32_t) \
  TEST_INT_TYPE (T, int64_t) \
  TEST_FLOAT_TYPE (T, _Float16, f16) \
  TEST_FLOAT_TYPE (T, float, f) \
  TEST_FLOAT_TYPE (T, double, )

TEST_ALL (DEF_LOOP)

/* { dg-final { scan-assembler-times {\tabs\tz[0-9]+\.b, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tabs\tz[0-9]+\.h, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tabs\tz[0-9]+\.s, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tabs\tz[0-9]+\.d, p[0-7]/m,} 1 } } */

/* { dg-final { scan-assembler-times {\tneg\tz[0-9]+\.b, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tneg\tz[0-9]+\.h, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tneg\tz[0-9]+\.s, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tneg\tz[0-9]+\.d, p[0-7]/m,} 1 } } */

/* { dg-final { scan-assembler-times {\tfabs\tz[0-9]+\.h, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tfabs\tz[0-9]+\.s, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tfabs\tz[0-9]+\.d, p[0-7]/m,} 1 } } */

/* { dg-final { scan-assembler-times {\tfneg\tz[0-9]+\.h, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tfneg\tz[0-9]+\.s, p[0-7]/m,} 1 } } */
/* { dg-final { scan-assembler-times {\tfneg\tz[0-9]+\.d, p[0-7]/m,} 1 } } */

/* Really we should be able to use MOVPRFX /z here, but at the moment
   we're relying on combine to merge a SEL and an arithmetic operation,
   and the SEL doesn't allow the "false" value to be zero when the "true"
   value is a register.  */
/* { dg-final { scan-assembler-times {\tmovprfx\tz[0-9]+, z[0-9]+\n} 7 } } */
/* { dg-final { scan-assembler-times {\tmovprfx\tz[0-9]+\.b, p[0-7]/z, z[0-9]+\.b} 1 } } */
/* { dg-final { scan-assembler-times {\tmovprfx\tz[0-9]+\.h, p[0-7]/z, z[0-9]+\.h} 2 } } */
/* { dg-final { scan-assembler-times {\tmovprfx\tz[0-9]+\.s, p[0-7]/z, z[0-9]+\.s} 2 } } */
/* { dg-final { scan-assembler-times {\tmovprfx\tz[0-9]+\.d, p[0-7]/z, z[0-9]+\.d} 2 } } */

/* { dg-final { scan-assembler-not {\tmov\tz[^\n]*z} } } */
/* { dg-final { scan-assembler-not {\tsel\t} } } */

#define N 99

#define TEST_LOOP(TYPE, OP)					\
  {								\
    TYPE r[N], a[N], pred[N];					\
    for (int i = 0; i < N; ++i)					\
      {								\
	a[i] = (i & 1 ? i : 3 * i) * (i % 3 == 0 ? 1 : -1);	\
	pred[i] = (i % 7 < 4);					\
	asm volatile ("" ::: "memory");				\
      }								\
    test_##TYPE##_##OP (r, a, pred, N);				\
    for (int i = 0; i < N; ++i)					\
      if (r[i] != (pred[i] ? OP (a[i]) : 0))			\
	__builtin_abort ();					\
  }

int main ()
{
  TEST_ALL (TEST_LOOP)
  return 0;
}
