/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv -mabi=ilp32d --param riscv-autovec-preference=scalable -fno-vect-cost-model -fdump-tree-optimized-details" } */

#include <stdint-gcc.h>

void __attribute__ ((noipa))
f (int16_t *restrict a, int16_t *restrict b, int n)
{
  for (int i = 0; i < n; ++i)
    {
      a[i * 8 + 0] = b[i * 8 + 1] + 1;
      a[i * 8 + 1] = b[i * 8 + 7] + 2;
      a[i * 8 + 2] = b[i * 8 + 1] + 3;
      a[i * 8 + 3] = b[i * 8 + 7] + 4;
      a[i * 8 + 4] = b[i * 8 + 1] + 5;
      a[i * 8 + 5] = b[i * 8 + 7] + 6;
      a[i * 8 + 6] = b[i * 8 + 1] + 7;
      a[i * 8 + 7] = b[i * 8 + 7] + 8;
    }
}

/* FIXME: Since we don't have VECT cost model yet, LOAD_LANES/STORE_LANES are chosen
   instead of SLP when riscv-autovec-lmul=m1.  */
/* { dg-final { scan-tree-dump-times "\.VEC_PERM" 1 "optimized" { xfail { any-opts "--param riscv-autovec-lmul=m1" } } } } */
