/* { dg-do compile } */
/* { dg-require-effective-target tls_native } */
/* { dg-options "-O -mtp=tpidr_el0" } */

__thread int i;

int
foo (void)
{
  return i;
}


/* { dg-final { scan-assembler-times {mrs\tx[0-9]+, tpidr_el0} 1 } } */
