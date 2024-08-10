/* { dg-do compile } */
/* { dg-require-effective-target arm_arch_v8a_ok } */
/* { dg-options "-O2" } */
/* { dg-add-options arm_arch_v8a } */

int v;

void
sync_lock_release (void)
{
  __sync_lock_release (&v);
}

/* { dg-final { scan-assembler-times "stl" 1 } } */
