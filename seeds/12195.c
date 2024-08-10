/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=scalable -fno-vect-cost-model -fdump-tree-optimized" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=fixed-vlmax -fno-vect-cost-model -fdump-tree-optimized" } */

#define TYPE uint64_t

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=fixed-vlmax -fno-vect-cost-model -fdump-tree-optimized" } */

#include <stdint-gcc.h>

#if !defined(TYPE)
#define TYPE uint32_t
#endif

#define N 254

/* Non-simple condition reduction.  */

TYPE __attribute__ ((noinline, noclone))
condition_reduction (TYPE *a, TYPE min_v)
{
  TYPE last = 65;

  for (TYPE i = 0; i < N; i++)
    if (a[i] < min_v)
      last = a[i];

  return last;
}

/* { dg-final { scan-tree-dump "\.LEN_FOLD_EXTRACT_LAST" "optimized" } } */

/* { dg-final { scan-tree-dump "\.LEN_FOLD_EXTRACT_LAST" "optimized" } } */

/* { dg-final { scan-tree-dump "\.LEN_FOLD_EXTRACT_LAST" "optimized" } } */
