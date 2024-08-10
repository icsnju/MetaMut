/* { dg-options "-march=sm_30" } */

/* { dg-do link } */

int
main (void)
{
  return 0;
}

/* { dg-final { scan-assembler-times "\\.target\tsm_30" 1 } } */
