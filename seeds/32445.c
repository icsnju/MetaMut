/* { dg-do compile } */
/* { dg-options "-O2 -fstack-clash-protection --param stack-clash-protection-guard-size=16" } */
/* { dg-require-effective-target supports_stack_clash_protection } */

#define SIZE 128 * 1024
int f_test (int x)
{
  char arr[SIZE];
  return arr[x];
}

/* { dg-final { scan-assembler-times {str\s+xzr, \[sp, 1024\]} 2 } } */

/* SIZE is more than 2x guard-size and no remainder, unrolled, no loop.  */