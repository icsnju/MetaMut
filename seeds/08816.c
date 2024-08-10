/* { dg-do run } */
/* { dg-options "-O2 -fzero-call-used-regs=used-gpr" } */

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
