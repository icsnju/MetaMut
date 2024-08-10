/* { dg-do compile } */
/* { dg-options "-march=rv32gcv -mabi=ilp32d -O3 --param riscv-autovec-preference=scalable" } */

void __attribute__((noinline, noclone))
f (int * __restrict dst, int * __restrict op1, int * __restrict op2, int count)
{
  for (int i = 0; i < count; ++i)
    dst[i] = op1[i] + op2[i];
}

/* { dg-final { scan-assembler-not "lw" } } */
/* { dg-final { scan-assembler-not "sw" } } */
