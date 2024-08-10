/* { dg-do compile } */
/* { dg-options "-O2 -fstack-clash-protection --param stack-clash-protection-guard-size=16" } */
/* { dg-require-effective-target supports_stack_clash_protection } */
/* { dg-require-effective-target alloca } */
/* { dg-skip-if "" { *-*-* } { "-fstack-check" } { "" } } */

#define SIZE 100

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

/* { dg-final { scan-assembler-times {st\.d\t\$r0,\$r3,104} 1 } } */

/* Alloca is less than guard-size, 1 probe at the top of the new allocation.  */
