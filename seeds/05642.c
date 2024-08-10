/* { dg-do compile } */
/* { dg-options "-O2 -fstack-clash-protection --param stack-clash-protection-guard-size=16" } */
/* { dg-require-effective-target supports_stack_clash_protection } */
/* { dg-require-effective-target alloca } */
/* { dg-skip-if "" { *-*-* } { "-fstack-check" } { "" } } */

#define SIZE 64 * 1024

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

/* Alloca is exactly one guard-size, 1 probe expected at top.  */
