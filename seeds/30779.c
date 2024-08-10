/* { dg-do compile } */
/* We scan for vdive*d which are only supported on 64-bit env.  */
/* { dg-require-effective-target lp64 } */
/* { dg-require-effective-target power10_ok } */
/* { dg-options "-mdejagnu-cpu=power10 -O2 -ftree-vectorize -fno-vect-cost-model -fno-unroll-loops -fdump-tree-vect-details" } */

/* Test if signed/unsigned long long extended divisions get vectorized.  */

#define N 128

typedef signed long long sLL;
typedef unsigned long long uLL;

sLL sll_a[N], sll_b[N], sll_c[N];
uLL ull_a[N], ull_b[N], ull_c[N];

__attribute__ ((noipa)) void
test_divde ()
{
  for (int i = 0; i < N; i++)
    sll_c[i] = __builtin_divde (sll_a[i], sll_b[i]);
}

__attribute__ ((noipa)) void
test_divdeu ()
{
  for (int i = 0; i < N; i++)
    ull_c[i] = __builtin_divdeu (ull_a[i], ull_b[i]);
}


/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 2 "vect" } } */
/* { dg-final { scan-assembler-times {\mvdivesd\M} 1 } } */
/* { dg-final { scan-assembler-times {\mvdiveud\M} 1 } } */
