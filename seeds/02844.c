/* { dg-do compile } */
/* { dg-options "-O2 -fstack-clash-protection --param stack-clash-protection-guard-size=16" } */
/* { dg-require-effective-target supports_stack_clash_protection } */
/* { dg-require-effective-target alloca } */

#define SIZE 65 * 1024

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

/* { dg-final { scan-assembler-times {str\s+xzr, \[sp, 1024\]} 1 } } */
/* { dg-final { scan-assembler-times {str\s+xzr, \[sp, 8\]} 1 } } */

/* Alloca is more than one guard-page, and residual is exactly 1Kb. 2 probes
   expected.  One at 1kB offset for the guard-size allocation and one at word
   offset for the residual.  */
