/* { dg-do compile } */
/* { dg-require-effective-target ia32 } */
/* { dg-skip-if "" { *-*-* } { "-march=*" } { "-march=i386" } } */
/* { dg-options "-Os -march=i386 -ffast-math -masm=att" } */

/* { dg-do compile } */
/* { dg-require-effective-target ia32 } */
/* { dg-skip-if "" { *-*-* } { "-march=*" } { "-march=i386" } } */
/* { dg-options "-O2 -march=i386 -ffast-math -masm=att" } */

extern short s;

int test_f_s (short x)
{
  return (float)x > s;
}

int test_d_s (short x)
{
  return (double)x < s;
}

int test_ld_s (short x)
{
  return (long double)x == s;
}

extern int i;

int test_f_i (int x)
{
  return (float)i >= x;
}

int test_d_i (int x)
{
  return (double)i <= x;
}

int test_ld_i (int x)
{
  return (long double)i != x;
}

/* { dg-final { scan-assembler-times "cmpw\[s\t\]" 3 } } */
/* { dg-final { scan-assembler-times "ficompl" 1 } } */
/* { dg-final { scan-assembler-times "cmpl" 2 } } */

/* { dg-final { scan-assembler-times "cmpw\[s\t\]" 3 } } */
/* { dg-final { scan-assembler-times "ficompl" 1 } } */
/* { dg-final { scan-assembler-times "cmpl" 2 } } */