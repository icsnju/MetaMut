/* Testing return address signing.  */
/* { dg-do compile } */
/* { dg-require-effective-target mbranch_protection_ok } */
/* { dg-options "-march=armv8.1-m.main+pacbti+fp -mbranch-protection=pac-ret -mthumb -mfloat-abi=hard --save-temps -O0" } */

#include <stdlib.h>

int
__attribute__((noinline))
foo1 (int a, int b)
{
  return a + b;
}

int
main (void)
{
  if (foo1 (1, 2) != 3)
    abort ();

  return 0;
}

/* { dg-final { scan-assembler "pac\tip, lr, sp" } } */
/* { dg-final { scan-assembler "aut\tip, lr, sp" } } */
/* { dg-final { scan-assembler-not "\tbti" } } */
