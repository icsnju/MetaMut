/* Testing return address signing.  */
/* { dg-do run } */
/* { dg-require-effective-target mbranch_protection_ok } */
/* { dg-require-effective-target arm_pacbti_hw } */
/* { dg-options "-march=armv8.1-m.main+pacbti+fp -mbranch-protection=bti+pac-ret+leaf -mthumb -mfloat-abi=hard --save-temps -O2" } */

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

/* { dg-final { scan-assembler-times "pacbti\tip, lr, sp" 2 } } */
/* { dg-final { scan-assembler-times "aut\tip, lr, sp" 2 } } */
/* { dg-final { scan-assembler-not "\tbti" } } */
