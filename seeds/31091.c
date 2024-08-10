/* { dg-do compile } */
/* { dg-options "-O -Wall" } */
/* { dg-final { scan-assembler "fs:16" } } */
/* { dg-final { scan-assembler "gs:16" } } */

int test(void)
{
  /* Also verify the accesses don't trigger warnings.  */
  int __seg_fs *f = (int __seg_fs *)16;
  int __seg_gs *g = (int __seg_gs *)16;
  return *f + *g;
}
