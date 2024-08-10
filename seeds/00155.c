/* { dg-do compile } */
/* { dg-require-effective-target tls_native } */
/* { dg-options "-O -mtp=cp15" } */

__thread int i;

int
foo (void)
{
  return i;
}


/* { dg-final { scan-assembler-times {mrc\tp15, 0, r3, c13, c0, 3} 1 } } */
