/* { dg-do compile } */
/* { dg-options "-O2 -fstack-clash-protection --param stack-clash-protection-guard-size=16" } */
/* { dg-require-effective-target supports_stack_clash_protection } */

#define SIZE 63 * 1024
int f_test (int x)
{
  char arr[SIZE];
  return arr[x];
}

/* { dg-final { scan-assembler-times {str\s+xzr,} 1 } } */

/* SIZE is exactly guard-size - 1Kb, boundary condition so 1 probe expected.
*/
