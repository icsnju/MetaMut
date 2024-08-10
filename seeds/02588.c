/* { dg-do run } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -dp" } */

#include <stdint.h>

#define DO_REGREG_OPS(TYPE)                                                    \
  void __attribute__ ((noclone))                                               \
  varith_##TYPE##_reg (TYPE *__restrict x, TYPE *__restrict y,                 \
		       TYPE *__restrict pred, int count)                       \
  {                                                                            \
    for (int i = 0; i < count; ++i)                                            \
      x[i] = (pred[i] != 1) ? (x[i] > y[i] ? x[i] : y[i]) : 4;                 \
  }

#define DO_IMMEDIATE_OPS(VALUE, TYPE)                                          \
  void __attribute__ ((noclone))                                               \
  varithimm_##VALUE##_##TYPE (TYPE *__restrict x, TYPE *__restrict pred,       \
			      int count)                                       \
  {                                                                            \
    for (int i = 0; i < count; ++i)                                            \
      x[i] = (pred[i] != 1) ? (x[i] > (TYPE) VALUE ? x[i] : (TYPE) VALUE) : 4; \
  }

#define DO_ARITH_OPS(REG, IMM, TYPE)                                           \
  REG (TYPE);                                                                  \
  IMM (2, TYPE);                                                               \
  IMM (86, TYPE);                                                              \
  IMM (109, TYPE);                                                             \
  IMM (141, TYPE);                                                             \
  IMM (229, TYPE);                                                             \
  IMM (255, TYPE);                                                             \
  IMM (992137445376, TYPE);

#define TEST_ALL(REG, IMM)                                                     \
  DO_ARITH_OPS (REG, IMM, uint8_t)                                             \
  DO_ARITH_OPS (REG, IMM, uint16_t)                                            \
  DO_ARITH_OPS (REG, IMM, uint32_t)                                            \
  DO_ARITH_OPS (REG, IMM, uint64_t)

TEST_ALL (DO_REGREG_OPS, DO_IMMEDIATE_OPS)

/* Two per test case < 64 bits with wide-enough type:*/
/* { dg-final { scan-assembler-times {umaxv64si3_exec} 20 } } */
/* { dg-final { scan-assembler-not {umaxv64si3/0} } } */
/* { dg-final { scan-assembler-not {\tv_writelane_b32\tv[0-9]+, vcc_??, 0} } } */

/* Two per test case with wide-enough type:*/
/* { dg-final { scan-assembler-times {\tv_cmp_gt_i32\tvcc, s[0-9]+, v[0-9]+} 56 } } */
/* { dg-final { scan-assembler-not {\ts_cmpk_lg_u32\tvcc_lo, 0} } } */

/* One per 64-bit test case: */
/* { dg-final { scan-assembler-times {\tv_cmp_ne_u64\ts\[[0-9]+:[0-9]+\], v\[[0-9]+:[0-9]+\], 1} 8 } } */
/* { dg-final { scan-assembler-times {\tv_cmp_gt_u64\tvcc, v[[0-9]+:[0-9]+], v[[0-9]+:[0-9]+]} 8 } } */


#define N 99

#define TEST_REGREG_OPS(TYPE)                                                  \
  {                                                                            \
    TYPE x[N], y[N];                                                           \
    TYPE pred[N];                                                              \
    for (int i = 0; i < N; ++i)                                                \
      {                                                                        \
	x[i] = i % 13;                                                         \
	y[i] = i * i;                                                          \
	pred[i] = i % 3;                                                       \
      }                                                                        \
    varith_##TYPE##_reg (x, y, pred, N);                                       \
    for (int i = 0; i < N; ++i)                                                \
      {                                                                        \
	TYPE expected = i % 3 != 1 ? (x[i] > y[i] ? x[i] : y[i]) : 4;          \
	if (x[i] != expected)                                                  \
	  __builtin_abort ();                                                  \
	asm volatile ("" ::: "memory");                                        \
      }                                                                        \
  }

#define TEST_IMMEDIATE_OPS(VALUE, TYPE)                                        \
  {                                                                            \
    TYPE x[N], y[N];                                                           \
    TYPE pred[N];                                                              \
    for (int i = 0; i < N; ++i)                                                \
      {                                                                        \
	x[i] = i * i;                                                          \
	pred[i] = i % 3;                                                       \
      }                                                                        \
    varithimm_##VALUE##_##TYPE (x, pred, N);                                   \
    for (int i = 0; i < N; ++i)                                                \
      {                                                                        \
	TYPE expected                                                          \
	  = i % 3 != 1 ? (x[i] > (TYPE) VALUE ? x[i] : (TYPE) VALUE) : 4;      \
	if (x[i] != expected)                                                  \
	  __builtin_abort ();                                                  \
	asm volatile ("" ::: "memory");                                        \
      }                                                                        \
  }

int
main (void)
{
  TEST_ALL (TEST_REGREG_OPS, TEST_IMMEDIATE_OPS)
  return 0;
}