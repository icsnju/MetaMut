/* Testing PACBTI multilibs matches without mve.  */
/* { dg-do run } */
/* { dg-require-effective-target arm_pacbti_hw } */
/* { dg-skip-if "need fp instructions" { *-*-* } { "" } { "-mfloat-abi=hard" } } */
/* { dg-options "-march=armv8.1-m.main+dsp+fp.dp+pacbti -mbranch-protection=standard -mthumb -mfloat-abi=hard" } */

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
