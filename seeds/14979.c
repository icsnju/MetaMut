/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=fixed-vlmax -fno-vect-cost-model -ffast-math" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=scalable -fno-vect-cost-model" } */

double foo (double *a, double *b, double *c)
{
  double result = 0.0;
  for (int i = 0; i < 1024; ++i)
    result += i & 1 ? __builtin_fma (a[i], b[i], c[i]) : 0.0;
  return result;
}

/* { dg-final { scan-assembler-times {vfmacc\.vv\s+v[0-9]+,v[0-9]+,v[0-9]+,v0.t} 1 } } */

/* { dg-final { scan-assembler {vfmacc\.vv\s+v[0-9]+,v[0-9]+,v[0-9]+,v0.t} } } */
