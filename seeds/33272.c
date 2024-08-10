/* { dg-do compile } */
/* { dg-options "-march=rv32gcv -mabi=ilp32 --param riscv-autovec-preference=scalable -fdump-tree-vect-details" } */

#include <stdint-gcc.h>

void foo2 (int8_t *__restrict a,
          int64_t *__restrict b,
          int64_t *__restrict c,
          int64_t *__restrict a2,
          int64_t *__restrict b2,
          int64_t *__restrict c2,
          int64_t *__restrict a3,
          int64_t *__restrict b3,
          int64_t *__restrict c3,
          int64_t *__restrict a4,
          int64_t *__restrict b4,
          int64_t *__restrict c4,
          int8_t *__restrict a5,
          int64_t *__restrict b5,
          int64_t *__restrict c5,
          int n)
{
    for (int i = 0; i < n; i++){
      a[i] = b[i] + c[i];
      b5[i] = b[i] + c[i];
      a2[i] = b2[i] + c2[i];
      a3[i] = b3[i] + c3[i];
      a4[i] = b4[i] + c4[i];
      a5[i] = a[i] + a4[i];
      a[i] = a5[i] + b5[i]+ a[i];

      a[i] = a[i] + c[i];
      b5[i] = a[i] + c[i];
      a2[i] = a[i] + c2[i];
      a3[i] = a[i] + c3[i];
      a4[i] = a[i] + c4[i];
      a5[i] = a[i] + a4[i];
      a[i] = a[i] + b5[i]+ a[i];
    }
}

/* { dg-final { scan-assembler {e64,m4} } } */
/* { dg-final { scan-assembler-not {csrr} } } */
/* { dg-final { scan-tree-dump-times "Maximum lmul = 8" 1 "vect" } } */
/* { dg-final { scan-tree-dump-times "Maximum lmul = 4" 1 "vect" } } */
/* { dg-final { scan-tree-dump-not "Maximum lmul = 2" "vect" } } */
/* { dg-final { scan-tree-dump-not "Maximum lmul = 1" "vect" } } */
