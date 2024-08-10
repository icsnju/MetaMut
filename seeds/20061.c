/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param=riscv-autovec-preference=scalable -ffast-math -fno-vect-cost-model" } */

#define N 0x1100

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv -mabi=ilp32d --param=riscv-autovec-preference=scalable -ffast-math -fno-vect-cost-model -fdump-tree-optimized-details" } */

int __attribute__((noipa))
add_loop (int *x, int n, int res)
{
  for (int i = 0; i < n; ++i)
    {
      res += x[i * 2];
      res += x[i * 2 + 1];
    }
  return res;
}

/* { dg-final { scan-tree-dump-times "VEC_SHL_INSERT" 1 "optimized" } } */
/* { dg-final { scan-assembler-times {vslide1up\.vx\s+v[0-9]+,\s*v[0-9]+,\s*[a-x0-9]+} 1 } } */

int
main (void)
{
  int x[N];
  for (int i = 0; i < N; ++i)
    x[i] = ((i + 1) * (i + 2)) & 0xfffff;

  if (add_loop (x, 0, 33) != 33
      || add_loop (x, 11, 30) != 4078
      || add_loop (x, 0x100, 45) != 45001773
      || add_loop (x, 0x11f, 300) != 63369900)
    __builtin_abort ();

  return 0;
}
