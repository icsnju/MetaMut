/* { dg-do compile } */
/* { dg-require-effective-target arm_arch_v8a_ok } */
/* { dg-options "-O2" } */
/* { dg-add-options arm_arch_v8a } */

int v;

int
sync_lock_test_and_set (int a)
{
  return __sync_lock_test_and_set (&v, a);
}

/* { dg-final { scan-assembler-times "ldrex" 1 } } */
/* { dg-final { scan-assembler-times "strex" 1 } } */
/* { dg-final { scan-assembler-times "dmb" 1 } } */
