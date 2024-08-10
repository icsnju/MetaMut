/* PR tree-optimization/103208 */
/* { dg-do compile } */
/* { dg-additional-options "-O2 -fwrapv" } */

void
foo (void)
{
  int a[1024];
  int *p, *q;
  #pragma omp parallel for collapse(2)
  for (p = &a[0]; p < &a[512]; p++)
    for (q = p + 64; q < p + 128; q++)
      ;
}
