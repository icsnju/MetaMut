/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do assemble { target aarch64_asm_sve_ok } } */
/* { dg-options "-O2 -ftree-vectorize -fdump-tree-vect-details --save-temps" } */

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

/* { dg-final { scan-tree-dump "operating on partial vectors." "vect" } } */
/* { dg-final { scan-assembler {\tclastb\ts[0-9]+, p[0-7], s[0-9]+, z[0-9]+\.s} } } */

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
