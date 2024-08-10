/* __builtin_apply_args () and __builtin_return () built-in functions does
   not function properly when -mmmx is used in compile flags.
   __builtin_apply_args () saves all registers that pass arguments to a
   function, including %mm0-%mm3, to a memory block, and __builtin_return ()
   restores %mm0, from a return memory block, as it can be used as a
   function return register.  Unfortunatelly, when MMX registers are touched,
   i387 FPU switches to MMX mode, and no FP operation is possible until emms
   instruction is issued.  */

/* This test case is adapted from gcc.dg/builtin-apply4.c.  */
 
/* { dg-do run { xfail { ! *-*-darwin* } } } */
/* { dg-options "-O2 -mmmx -fgnu89-inline" } */
/* { dg-require-effective-target ia32 } */

#include <stdio.h>
#include <stdlib.h>

#include "cpuid.h"

static void mmx_test (void);

static void
__attribute__ ((noinline))
do_test (void)
{
  mmx_test ();
}

int
main ()
{
  unsigned int eax, ebx, ecx, edx;
 
  if (!__get_cpuid (1, &eax, &ebx, &ecx, &edx))
    return 0;

  /* Run MMX test only if host has MMX support.  */
  if (edx & bit_MMX)
    do_test ();

  return 0;
}

extern void abort (void);

double
foo (double arg)
{
  if (arg != 116.0)
    abort ();

  return arg + 1.0;
}

inline double
bar (double arg)
{
  foo (arg);
  __builtin_return (__builtin_apply ((void (*)()) foo,
				     __builtin_apply_args (), 16));
}

static void
mmx_test (void)
{
  if (bar (116.0) != 117.0)
    abort ();
}
