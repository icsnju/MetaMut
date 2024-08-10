/* { dg-do compile } */
/* { dg-require-effective-target tls_native } */
/* { dg-options "-O2 -fpic -ftls-model=initial-exec -mcmodel=tiny" } */
/* { dg-require-effective-target fpic } */

void abort (void);

__thread int t0 = 0x10;
__thread int t1 = 0x10;

int
main (int argc, char **argv)
{
  if (t0 != t1)
    abort ();

  return  0;
}


/* { dg-final { scan-assembler-times ":gottprel:" 2 } } */
