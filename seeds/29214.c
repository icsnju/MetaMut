/* { dg-do run } */
/* { dg-require-effective-target tls_native } */
/* { dg-options "-O2 -fpic -ftls-model=local-exec -mtls-size=12 --save-temps" } */
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


/* { dg-final { scan-assembler-times "#:tprel_lo12" 2 } } */
/* { dg-final { cleanup-saved-temps } } */