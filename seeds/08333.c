/* { dg-do run } */
/* { dg-options "-O2 -mlzcnt" } */
/* { dg-require-effective-target lzcnt } */

#include <stdio.h>
#include <stdlib.h>

#include "cpuid.h"

static void lzcnt_test (void);

static void
__attribute__ ((noinline))
do_test (void)
{
  lzcnt_test ();
}

int
main ()
{
  unsigned int eax, ebx, ecx, edx;

  if (!__get_cpuid (0x80000001, &eax, &ebx, &ecx, &edx))
    return 0;

  /* Run LZCNT test only if host has LZCNT support.  */
  if (ecx & bit_LZCNT)
    {
      do_test ();
#ifdef DEBUG
      printf ("PASSED\n");
#endif
    }
#ifdef DEBUG
  else
    printf ("SKIPPED\n");
#endif

  return 0;
}

static int
foo (unsigned int v)
{
  return v ? __builtin_clz (v) : 32;
}

/* returns -1 if x == 0 */
int
__attribute__ ((noinline, noclone))
bar (unsigned int x)
{
  return 31 - foo (x);
}

static void
lzcnt_test ()
{
  int r = bar (0);

  if (r != -1)
    abort ();
}
