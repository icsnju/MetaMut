/* { dg-do compile } */
/* { dg-skip-if "-mpure-code supports M-profile only" { *-*-* } { "-mpure-code" } } */
/* { dg-require-effective-target arm32 } */
/* { dg-options "-O2 -mcpu=cortex-a57 -save-temps" } */

int
f (int x)
{
  return x % 256;
}

/* { dg-final { scan-assembler "rsbpl\tr\[0-9\]*" } } */

