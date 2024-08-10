/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param=riscv-autovec-preference=scalable -fno-vect-cost-model" } */

/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param=riscv-autovec-preference=fixed-vlmax -fno-vect-cost-model" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=fixed-vlmax -fno-vect-cost-model -fdump-tree-optimized" } */

#define N 32

/* Simple condition reduction.  */

int __attribute__ ((noinline, noclone))
condition_reduction (int *a, int min_v)
{
  int last = 66; /* High start value.  */

  for (int i = 0; i < N; i++)
    if (a[i] < min_v)
      last = i;

  return last;
}

/* { dg-final { scan-tree-dump "\.LEN_FOLD_EXTRACT_LAST" "optimized" } } */

int __attribute__ ((optimize (1)))
main (void)
{
  int a[N] = {
    11, -12, 13, 14, 15, 16, 17, 18, 19, 20,
    1, 2, -3, 4, 5, 6, 7, -8, 9, 10,
    21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
    31, 32
  };

  int ret = condition_reduction (a, 1);

  if (ret != 17)
    __builtin_abort ();

  return 0;
}
