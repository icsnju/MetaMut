/* { dg-do compile } */
/* { dg-options "-march=rv32gcv -mabi=ilp32 --param riscv-autovec-preference=scalable -fno-schedule-insns -fdump-tree-vect-details" } */

#include <stdint-gcc.h>

void
foo (int32_t *__restrict a, int32_t *__restrict b,    int32_t *__restrict c,
      int32_t *__restrict a2, int32_t *__restrict b2, int32_t *__restrict c2,
      int32_t *__restrict a3, int32_t *__restrict b3, int32_t *__restrict c3,
      int32_t *__restrict a4, int32_t *__restrict b4, int32_t *__restrict c4,
      int32_t *__restrict a5, int32_t *__restrict b5, int32_t *__restrict c5,
      int32_t *__restrict d,
      int32_t *__restrict d2,
      int32_t *__restrict d3,
      int32_t *__restrict d4,
      int32_t *__restrict d5,
      int32_t *__restrict e,
      int32_t *__restrict e2,
      int32_t *__restrict e3,
      int32_t *__restrict e4,
      int32_t *__restrict e5,
      int n)
{
  for (int i = 0; i < n; i++)
    {
      a[i] = b[i] + c[i];
      b5[i] = b[i] + c[i];
      a2[i] = b2[i] + c2[i];
      a3[i] = b3[i] + c3[i];
      a4[i] = b4[i] + c4[i];
      a5[i] = a[i] + a4[i];
      e[i] = a2[i] + c2[i];
      e2[i] = d2[i] + a2[i];
      e3[i] = d3[i] + a3[i];
      e4[i] = d4[i] + a4[i];
      e5[i] = a[i] + a4[i];
      d2[i] = a2[i] + c2[i];
      d3[i] = a3[i] + c3[i];
      d4[i] = a4[i] + c4[i];
      d5[i] = a[i] + a4[i];
      a[i] = a5[i] + b5[i] + a[i];

      c2[i] = a[i] + c[i];
      c3[i] = b5[i] * a5[i];
      c4[i] = a2[i] * a3[i];
      c5[i] = b5[i] * a2[i];
      c[i] = a[i] + c3[i];
      c2[i] = a[i] + c4[i];
      a5[i] = a[i] + a4[i];
      a[i] = a[i] + b5[i] + a[i] * a2[i] * a3[i] * a4[i] 
      * a5[i] * c[i] * c2[i] * c3[i] * c4[i] * c5[i]
      * d[i] * d2[i] * d3[i] * d4[i] * d5[i]
      * e[i] * e2[i] * e3[i] * e4[i] * e5[i];
    }
}

/* FIXME: Choosing LMUL = 1 is not the optimal since it can be LMUL = 2 if we apply instruction scheduler.  */
/* { dg-final { scan-assembler {e32,m1} } } */
/* { dg-final { scan-assembler-not {csrr} } } */
/* { dg-final { scan-tree-dump-times "Maximum lmul = 8" 1 "vect" } } */
/* { dg-final { scan-tree-dump-times "Maximum lmul = 4" 1 "vect" } } */
/* { dg-final { scan-tree-dump-times "Maximum lmul = 2" 1 "vect" } } */
/* { dg-final { scan-tree-dump-times "Maximum lmul = 1" 1 "vect" } } */
