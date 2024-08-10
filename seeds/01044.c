/* { dg-do run } */
/* { dg-skip-if "not implemented" { ! { i?86*-*-* x86_64*-*-* sparc*-*-* aarch64*-*-* arm*-*-* nvptx*-*-* s390*-*-* loongarch64*-*-* } } } */
/* { dg-options "-O2 -fzero-call-used-regs=all-arg" } */

/* { dg-do run } */
/* { dg-options "-O2 -fzero-call-used-regs=skip" } */

volatile int result = 0;
int 
__attribute__((noipa))
foo (int x)
{
  return x;
}
int main()
{
  result = foo (2);
  return 0;
}
