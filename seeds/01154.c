/* { dg-do compile } */
/* { dg-options "-march=rv64gcv -mabi=lp64d --param riscv-autovec-preference=scalable -fdump-tree-vect-details" } */

#include <stdint-gcc.h>

int64_t
foo (int64_t *__restrict a, int64_t init, int n)
{
  for (int i = 0; i < n; i++)
    init += a[i];
  return init;
}

/* { dg-final { scan-assembler {e64,m8} } } */
/* { dg-final { scan-assembler-not {csrr} } } */
/* { dg-final { scan-tree-dump-times "Maximum lmul = 8" 1 "vect" } } */
/* { dg-final { scan-tree-dump-not "Maximum lmul = 4" "vect" } } */
/* { dg-final { scan-tree-dump-not "Maximum lmul = 2" "vect" } } */
/* { dg-final { scan-tree-dump-not "Maximum lmul = 1" "vect" } } */
