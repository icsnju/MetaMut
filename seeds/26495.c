/* { dg-options "-march-map=sm_50" } */

/* { dg-do link } */

int
main (void)
{
  return 0;
}

/* { dg-final { scan-assembler-times "\\.target\tsm_35" 1 } } */
