/* { dg-do compile } */
/* What we scan for are only supported on 64-bit env.  */
/* { dg-require-effective-target lp64 } */
/* { dg-require-effective-target power10_ok } */
/* { dg-options "-mdejagnu-cpu=power10 -O2 -ftree-vectorize -fno-vect-cost-model -fno-unroll-loops -fdump-tree-vect-details" } */

/* Test if some Power10 built-in functions get vectorized.  */

#define N 32

typedef unsigned long long uLL;
uLL ull_a[N], ull_b[N], ull_c[N];

__attribute__ ((noipa)) void
test_cfuged ()
{
  for (int i = 0; i < N; i++)
    ull_c[i] = __builtin_cfuged (ull_a[i], ull_b[i]);
}

__attribute__ ((noipa)) void
test_cntlzdm ()
{
  for (int i = 0; i < N; i++)
    ull_c[i] = __builtin_cntlzdm (ull_a[i], ull_b[i]);
}

__attribute__ ((noipa)) void
test_cnttzdm ()
{
  for (int i = 0; i < N; i++)
    ull_c[i] = __builtin_cnttzdm (ull_a[i], ull_b[i]);
}

__attribute__ ((noipa)) void
test_pdepd ()
{
  for (int i = 0; i < N; i++)
    ull_c[i] = __builtin_pdepd (ull_a[i], ull_b[i]);
}

__attribute__ ((noipa)) void
test_pextd ()
{
  for (int i = 0; i < N; i++)
    ull_c[i] = __builtin_pextd (ull_a[i], ull_b[i]);
}


/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 5 "vect" } } */
/* { dg-final { scan-assembler-times {\mvcfuged\M} 1 } } */
/* { dg-final { scan-assembler-times {\mvclzdm\M} 1 } } */
/* { dg-final { scan-assembler-times {\mvctzdm\M} 1 } } */
/* { dg-final { scan-assembler-times {\mvpdepd\M} 1 } } */
/* { dg-final { scan-assembler-times {\mvpextd\M} 1 } } */
