/* PR rtl-optimization/88416 */
/* { dg-do compile } */
/* { dg-options "-O1 -fvar-tracking-assignments -fno-forward-propagate --param max-cse-insns=1" } */

/* { dg-do run } */
/* { dg-options "-O0" } */

#include <x86intrin.h>

extern void abort (void);

#ifdef __x86_64__
#define EFLAGS_TYPE unsigned long long int
#else
#define EFLAGS_TYPE unsigned int
#endif

int
main ()
{
  EFLAGS_TYPE flags = 0xD7; /* 111010111b  */

  __writeeflags (flags);

  flags = __readeflags ();

  if ((flags & 0xFF) != 0xD7)
    abort ();

#ifdef DEBUG
    printf ("PASSED\n");
#endif

  return 0;
}

