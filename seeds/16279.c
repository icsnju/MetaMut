/* { dg-do compile } */
/* { dg-require-effective-target power10_ok } */
/* { dg-options "-mdejagnu-cpu=power10 -O2 -ftree-vectorize -fno-vect-cost-model -fno-unroll-loops -fdump-tree-vect-details" } */

/* Test if signed/unsigned int extended divisions get vectorized.  */

#define N 128

typedef signed int si;
typedef unsigned int ui;

si si_a[N], si_b[N], si_c[N];
ui ui_a[N], ui_b[N], ui_c[N];

__attribute__ ((noipa)) void
test_divwe ()
{
  for (int i = 0; i < N; i++)
    si_c[i] = __builtin_divwe (si_a[i], si_b[i]);
}

__attribute__ ((noipa)) void
test_divweu ()
{
  for (int i = 0; i < N; i++)
    ui_c[i] = __builtin_divweu (ui_a[i], ui_b[i]);
}


/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 2 "vect" } } */
/* { dg-final { scan-assembler-times {\mvdivesw\M} 1 } } */
/* { dg-final { scan-assembler-times {\mvdiveuw\M} 1 } } */
