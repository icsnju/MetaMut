/* { dg-do compile } */
/* { dg-options "-O2 -mcpu=cortex-a57 -save-temps" } */

int
f (int x)
{
  return x % 256;
}

/* { dg-final { scan-assembler "csneg\t\[wx\]\[0-9\]*" } } */
