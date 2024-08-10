/* { dg-do run { target { { i?86-*-* x86_64-*-* } && ia32 } } } */

#include <stddef.h>
#ifdef DEBUG
#include <stdio.h>
#endif

#ifdef  __cplusplus
extern "C" void abort (void);
#else
extern void abort (void);
#endif

int
check_int (int *i, int align)
{
  *i = 20;
  if ((((ptrdiff_t) i) & (align - 1)) != 0)
    {
#ifdef DEBUG
      printf ("\nUnalign address (%d): %p!\n", align, i);
#endif
      abort ();
    }
  return *i;
}

void
check (void *p, int align)
{
  if ((((ptrdiff_t) p) & (align - 1)) != 0)
    {
#ifdef DEBUG
      printf ("\nUnalign address (%d): %p!\n", align, p);
#endif
      abort ();
    }
}

#ifndef ALIGNMENT
#define ALIGNMENT	64
#endif

typedef int aligned __attribute__((aligned(ALIGNMENT)));

int global;

__attribute__ ((fastcall))
void
foo (int j, int k, int m, int n, int o)
{
  aligned i;

  if (check_int (&i,  __alignof__(i)) != i)
    abort ();

  if (i != 20 || j != 1 || k != 2 || m != 3 || n != 4 || o != 5)
    abort ();
}

int
main()
{
  foo (1, 2, 3, 4, 5);
  return 0;
}
