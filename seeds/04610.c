/* { dg-do compile } */
/* { dg-options "-march=armv8-a+nolse -O2 -moutline-atomics" } */

int v;

int
sync_lock_test_and_set (int a)
{
  return __sync_lock_test_and_set (&v, a);
}

/* { dg-final { scan-assembler-times "bl.*__aarch64_swp4_sync" 1 } } */
