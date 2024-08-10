/* { dg-do compile } */
/* { dg-options "-O2 -mcpu=cortex-a57 -save-temps" } */

int
f (int x)
{
  return x % 2;
}

/* { dg-final { scan-assembler "csneg\t\[wx\]\[0-9\]*" } } */
/* { dg-final { scan-assembler-times "and\t\[wx\]\[0-9\]*" 1 } } */
