/* { dg-do compile } */
/* { dg-require-effective-target arm_arch_v8a_ok } */
/* { dg-options "-O2" } */
/* { dg-add-options arm_arch_v8a } */

int v = 0;

int
sync_bool_compare_swap (int a, int b)
{
  return __sync_bool_compare_and_swap (&v, &a, &b);
}

int
sync_val_compare_swap (int a, int b)
{
  return __sync_val_compare_and_swap (&v, &a, &b);
}

/* { dg-final { scan-assembler-times "ldrex" 2 } } */
/* { dg-final { scan-assembler-times "stlex" 2 } } */
/* { dg-final { scan-assembler-times "dmb" 2 } } */
