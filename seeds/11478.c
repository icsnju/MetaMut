/* { dg-do compile } */
/* { dg-options "-O2 -fstack-clash-protection --param stack-clash-protection-guard-size=16" } */
/* { dg-require-effective-target supports_stack_clash_protection } */
/* { dg-require-effective-target alloca } */
/* { dg-skip-if "" { *-*-* } { "-fstack-check" } { "" } } */

#define SIZE y

/* Avoid inclusion of alloca.h, unavailable on some systems.  */
#define alloca __builtin_alloca

__attribute__((noinline, noipa))
void g (char* ptr, int y)
{
  ptr[y] = '\0';
}

void f_caller (int y)
{
  char* pStr = alloca(SIZE);
  g (pStr, y);
}

/* { dg-final { scan-assembler-times {stp*t*r*\.d\t\$r0,\$r\d{1,2},-8} 1 } } */
/* { dg-final { scan-assembler-times {stx\.d\t\$r0,\$r3,\$r12} 1 } } */

/* Dynamic alloca, expect loop, and 1 probes with top at sp.
   1st probe is inside the loop for the full guard-size allocations, second
   probe is for the case where residual is zero.  */
