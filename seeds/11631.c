/* { dg-do run { target ia32 } } */
/* { dg-require-effective-target sse2_runtime } */
/* { dg-options "-O2 -msse2 -mstv -W" } */

#include <stdlib.h>
/* { dg-do compile { target ia32 } } */
/* { dg-options "-O2 -msse2 -mstv -mpreferred-stack-boundary=4 -mno-stackrealign -W" } */
/* { dg-final { scan-assembler "movq\[ \t\]%xmm\[0-9\]+, \\(%esp\\)" } } */
/* { dg-final { scan-assembler-not "psrlq" } } */

#include <setjmp.h>

extern jmp_buf buf;

extern long long *target_p;
extern long long *c;

extern void foo (long long);

__attribute__ ((noclone, noinline))
void
bar (void)
{
  if (setjmp (buf))
    {
      long long target = *target_p;
      *c = target;
      foo (target);
    }
  else
    foo (0);
}

jmp_buf buf;

long long *target_p;
long long *c;

int count;

__attribute__ ((noclone, noinline))
void
foo (long long x)
{
  if (x != *c)
    abort ();
  if (!count)
    {
      count++;
      longjmp (buf, 1);
    }
}

int
main ()
{
  long long val = 30;
  long long local = 0;
  target_p = &val;
  c = &local;
  bar ();
  if (val != local)
    abort ();
  return 0;
}
