/* { dg-do compile } */
/* { dg-options "-march=armv8-a+nolse -O2 -fno-ipa-icf -mno-outline-atomics" } */

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

/* { dg-final { scan-assembler-times "ldxr\tw\[0-9\]+, \\\[x\[0-9\]+\\\]" 2 } } */
/* { dg-final { scan-assembler-times "stlxr\tw\[0-9\]+, w\[0-9\]+, \\\[x\[0-9\]+\\\]" 2 } } */
/* { dg-final { scan-assembler-times "dmb\tish" 2 } } */
