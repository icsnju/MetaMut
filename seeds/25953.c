/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param riscv-autovec-preference=scalable" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv -mabi=ilp32d --param riscv-autovec-preference=scalable -fno-vect-cost-model -fdump-tree-optimized-details" } */

#include <stdint-gcc.h>

void
f (uint8_t *restrict a, uint8_t *restrict b,
   uint8_t *restrict c, uint8_t *restrict d,
   int n)
{
  for (int i = 0; i < n; ++i)
    {
      a[i * 8] = c[i * 8] + d[i * 8];
      a[i * 8 + 1] = c[i * 8] + d[i * 8 + 1];
      a[i * 8 + 2] = c[i * 8 + 2] + d[i * 8 + 2];
      a[i * 8 + 3] = c[i * 8 + 2] + d[i * 8 + 3];
      a[i * 8 + 4] = c[i * 8 + 4] + d[i * 8 + 4];
      a[i * 8 + 5] = c[i * 8 + 4] + d[i * 8 + 5];
      a[i * 8 + 6] = c[i * 8 + 6] + d[i * 8 + 6];
      a[i * 8 + 7] = c[i * 8 + 6] + d[i * 8 + 7];
      b[i * 8] = c[i * 8 + 1] + d[i * 8];
      b[i * 8 + 1] = c[i * 8 + 1] + d[i * 8 + 1];
      b[i * 8 + 2] = c[i * 8 + 3] + d[i * 8 + 2];
      b[i * 8 + 3] = c[i * 8 + 3] + d[i * 8 + 3];
      b[i * 8 + 4] = c[i * 8 + 5] + d[i * 8 + 4];
      b[i * 8 + 5] = c[i * 8 + 5] + d[i * 8 + 5];
      b[i * 8 + 6] = c[i * 8 + 7] + d[i * 8 + 6];
      b[i * 8 + 7] = c[i * 8 + 7] + d[i * 8 + 7];
    }
}

/* FIXME: Since we don't have VECT cost model yet, LOAD_LANES/STORE_LANES are chosen
   instead of SLP when riscv-autovec-lmul=m1.  */
/* { dg-final { scan-tree-dump-times "\.VEC_PERM" 2 "optimized" { xfail { any-opts "--param riscv-autovec-lmul=m1" } } } } */
/* { dg-final { scan-assembler {\tvid\.v} { xfail { any-opts "--param riscv-autovec-lmul=m1" } } } } */
/* { dg-final { scan-assembler-not {\tvmul} } } */

#define LIMIT 256
void __attribute__ ((optimize (0)))
f_golden (uint8_t *restrict a, uint8_t *restrict b, uint8_t *restrict c,
	  uint8_t *restrict d, int n)
{
  for (int i = 0; i < n; ++i)
    {
      a[i * 8] = c[i * 8] + d[i * 8];
      a[i * 8 + 1] = c[i * 8] + d[i * 8 + 1];
      a[i * 8 + 2] = c[i * 8 + 2] + d[i * 8 + 2];
      a[i * 8 + 3] = c[i * 8 + 2] + d[i * 8 + 3];
      a[i * 8 + 4] = c[i * 8 + 4] + d[i * 8 + 4];
      a[i * 8 + 5] = c[i * 8 + 4] + d[i * 8 + 5];
      a[i * 8 + 6] = c[i * 8 + 6] + d[i * 8 + 6];
      a[i * 8 + 7] = c[i * 8 + 6] + d[i * 8 + 7];
      b[i * 8] = c[i * 8 + 1] + d[i * 8];
      b[i * 8 + 1] = c[i * 8 + 1] + d[i * 8 + 1];
      b[i * 8 + 2] = c[i * 8 + 3] + d[i * 8 + 2];
      b[i * 8 + 3] = c[i * 8 + 3] + d[i * 8 + 3];
      b[i * 8 + 4] = c[i * 8 + 5] + d[i * 8 + 4];
      b[i * 8 + 5] = c[i * 8 + 5] + d[i * 8 + 5];
      b[i * 8 + 6] = c[i * 8 + 7] + d[i * 8 + 6];
      b[i * 8 + 7] = c[i * 8 + 7] + d[i * 8 + 7];
    }
}

int
main (void)
{
#define RUN(NUM)                                                               \
  uint8_t a_##NUM[NUM * 8 + 8] = {0};                                          \
  uint8_t a_golden_##NUM[NUM * 8 + 8] = {0};                                   \
  uint8_t b_##NUM[NUM * 8 + 8] = {0};                                          \
  uint8_t b_golden_##NUM[NUM * 8 + 8] = {0};                                   \
  uint8_t c_##NUM[NUM * 8 + 8] = {0};                                          \
  uint8_t d_##NUM[NUM * 8 + 8] = {0};                                          \
  for (int i = 0; i < NUM * 8 + 8; i++)                                        \
    {                                                                          \
      if (i % NUM == 0)                                                        \
	c_##NUM[i] = (i + NUM) % LIMIT;                                        \
      else                                                                     \
	c_##NUM[i] = (i * 3) % LIMIT;                                          \
      if (i % 2 == 0)                                                          \
	d_##NUM[i] = i % LIMIT;                                                \
      else                                                                     \
	d_##NUM[i] = (i * 7) % LIMIT;                                          \
    }                                                                          \
  f (a_##NUM, b_##NUM, c_##NUM, d_##NUM, NUM);                                 \
  f_golden (a_golden_##NUM, b_golden_##NUM, c_##NUM, d_##NUM, NUM);            \
  for (int i = 0; i < NUM * 8 + 8; i++)                                        \
    {                                                                          \
      if (a_##NUM[i] != a_golden_##NUM[i])                                     \
	__builtin_abort ();                                                    \
      if (b_##NUM[i] != b_golden_##NUM[i])                                     \
	__builtin_abort ();                                                    \
    }

  RUN (3);
  RUN (5);
  RUN (15);
  RUN (16);
  RUN (17);
  RUN (31);
  RUN (32);
  RUN (33);
  RUN (63);
  RUN (64);
  RUN (65);
  RUN (127);
  RUN (128);
  RUN (129);
  RUN (239);
  RUN (359);
  RUN (498);
  RUN (799);
  RUN (977);
  RUN (5789);
  return 0;
}
