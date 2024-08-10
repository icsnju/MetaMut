/* { dg-do run } */
/* { dg-require-effective-target tls_native } */
/* { dg-require-effective-target aarch64_tlsle32 } */
/* { dg-options "-O2 -fpic -ftls-model=local-exec -mtls-size=32 --save-temps" } */
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


/* { dg-final { scan-assembler-times "#:tprel_g1" 2 } } */
/* { dg-final { scan-assembler-times "#:tprel_g0_nc" 2 } } */
/* { dg-final { cleanup-saved-temps } } */
