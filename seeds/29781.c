/* { dg-do compile } */
/* { dg-options "-O2 -fstack-clash-protection --param stack-clash-protection-guard-size=16" } */
/* { dg-require-effective-target supports_stack_clash_protection } */
/* { dg-require-effective-target alloca } */

#define SIZE 127.5 * 64 * 1024

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

/* { dg-final { scan-assembler-times {str\s+xzr, \[sp, 1024\]} 2 } } */

/* Large alloca of an amount which isn't a multiple of a guard-size, and
   residiual is more than 1kB.  Loop expected with one 1Kb probe offset and
   one residual probe at offset 1kB.  */
