/* { dg-do compile } */
/* { dg-options "-std=c11 -O" } */
/* { dg-require-effective-target arm_arch_v7ve_ok } */
/* { dg-add-options arm_arch_v7ve } */

#include <stdatomic.h>

atomic_ullong foo;
int glob;

int
main (void)
{
  atomic_load_explicit (&foo, memory_order_acquire);
  return glob;
}

/* { dg-final { scan-assembler-times "ldrd\tr\[0-9\]+, r\[0-9\]+, \\\[r\[0-9\]+\\\]" 1 } } */
/* { dg-final { scan-assembler-times "dmb\tish" 1 } } */
