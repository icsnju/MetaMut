/* PR tree-optimization/89487 */
/* { dg-do compile } */
/* { dg-options "-O2 -ftree-loop-distribution" } */

#if defined (__aarch64__)
# define REG1 "x0"
# define REG2 "x1"
#elif defined (__arm__)
# define REG1 "r0"
# define REG2 "r1"
#elif defined (__i386__)
# define REG1 "%eax"
# define REG2 "%edx"
#elif defined (__powerpc__) || defined (__POWERPC__)
# define REG1 "r3"
# define REG2 "r4"
#elif defined (__s390__)
# define REG1 "0"
# define REG2 "1"
#elif defined (__x86_64__)
# define REG1 "rax"
# define REG2 "rdx"
#endif

void
caml_interprete (void)
{
#if defined(REG1) && defined(REG2)
  register int *pc asm(REG1);
  register int *sp asm(REG2);
  int i;

  for (i = 0; i < 3; ++i)
    *--sp = pc[i];
#endif
}
