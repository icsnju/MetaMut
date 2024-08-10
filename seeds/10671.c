/* { dg-do compile } */
/* { dg-options "-O2 -fstack-clash-protection --param stack-clash-protection-guard-size=16" } */
/* { dg-require-effective-target supports_stack_clash_protection } */
/* { dg-skip-if "" { *-*-* } { "-fstack-check" } { "" } } */

#define SIZE 65 * 1024
int f_test (int x)
{
  char arr[SIZE];
  return arr[x];
}

/* { dg-final { scan-assembler-times {stp*t*r*\.d\t\$r0,\$r3,0} 1 } } */

/* SIZE is more than guard-size 1 probe expected.  */
