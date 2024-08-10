/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-require-effective-target supports_stack_clash_protection } */
/* { dg-options "-O3 -fopenmp-simd -fstack-clash-protection --param stack-clash-protection-guard-size=16" } */

/* { dg-do compile } */
/* { dg-require-effective-target supports_stack_clash_protection } */
/* { dg-options "-O3 -fopenmp-simd -fstack-clash-protection --param stack-clash-protection-guard-size=16" } */

#include <stdint.h>

#define N 50
#define S 2 * 64 * 1024

/* Invoke X (P##n) for n in [0, 9].  */
#define REPEAT8(X, P) \
  X (P##0) X (P##1) X (P##2) X (P##3) X (P##4) X (P##5) X (P##6) X (P##7) \
  X (P##8)  X (P##9)

/* Invoke X (n) for all n in [0, 49].  */
#define REPEAT50(X) \
  REPEAT8 (X, ) REPEAT8 (X, 1)  REPEAT8 (X, 2) REPEAT8 (X, 3) REPEAT8 (X, 4)

  /* Try to force some spilling.  */
#define DECLARE(N) int src##N = src[N * 4];
#define INC(N) dest[i] += src##N;

#define TEST_LOOP(NAME, TYPE)				\
  void __attribute__ ((noinline, noclone, simd))	\
  NAME (TYPE *restrict dest, TYPE *restrict src)	\
  {							\
    REPEAT50 (DECLARE);					\
    volatile char foo[S];				\
    foo[S-1]=1;						\
    for (int i = 0; i < N; i++)				\
      {							\
	REPEAT50 (INC);					\
      }							\
  }

#define TEST(NAME) \
  TEST_LOOP (NAME##_i32, int32_t) \
  TEST_LOOP (NAME##_i64, int64_t) \
  TEST_LOOP (NAME##_f32, float) \
  TEST_LOOP (NAME##_f64, double)

TEST (test)

/* Check the vectorized loop for stack clash probing.  */

/* { dg-final { scan-assembler-times {str\s+xzr, \[sp, 0\]} 4 } } */
/* { dg-final { scan-assembler-times {cmp\s+x[0-9]+, 61440} 4 } } */
/* { dg-final { scan-assembler-times {sub\s+x[0-9]+, x[0-9]+, 61440} 4 } } */

#undef TEST_LOOP
#define TEST_LOOP(NAME, TYPE)				\
  {							\
    TYPE out[N];					\
    TYPE in[N * 4];					\
    for (int i = 0; i < N; ++i)				\
      {							\
	out[i] = i * 7 / 2;				\
	asm volatile ("" ::: "memory");			\
      }							\
    for (int i = 0; i < N * 4; ++i)			\
      {							\
	in[i] = i * 9 / 2;				\
	asm volatile ("" ::: "memory");			\
      }							\
    NAME (out, in);					\
    for (int i = 0; i < N; ++i)				\
      {							\
	TYPE expected = i * 7 / 2;			\
	if (out[i] != out[0] + expected)		\
	  __builtin_abort ();				\
	asm volatile ("" ::: "memory");			\
      }							\
  }

int __attribute__ ((optimize (0)))
main (void)
{
  TEST (test);
  return 0;
}
