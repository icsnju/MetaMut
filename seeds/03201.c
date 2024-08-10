/* { dg-do compile } */
/* { dg-options "-O2 -fstack-clash-protection --param stack-clash-protection-guard-size=16" } */
/* { dg-require-effective-target supports_stack_clash_protection } */
/* { dg-skip-if "" { *-*-* } { "-fstack-check" } { "" } } */

#define SIZE 128
int f_test (int x)
{
  char arr[SIZE];
  return arr[x];
}

/* { dg-final { scan-assembler-times {stp*t*r*\.d\t\$r0,\$r3,0} 0 } } */

/* SIZE is smaller than guard-size so no probe expected.  */
