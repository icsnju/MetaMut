/* { dg-do compile } */
/* { dg-options "-std=c11 -O" } */
/* { dg-require-effective-target arm_arch_v8a_ok } */
/* { dg-add-options arm_arch_v8a } */

#include <stdatomic.h>

atomic_ullong foo;
int glob;

int
main (void)
{
  atomic_load_explicit (&foo, memory_order_acquire);
  return glob;
}

/* { dg-final { scan-assembler-times "ldaexd\tr\[0-9\]+, r\[0-9\]+, \\\[r\[0-9\]+\\\]" 1 } } */
/* { dg-final { scan-assembler-not "dmb\tish" } } */