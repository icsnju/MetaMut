/* { dg-do run } */
/* { dg-skip-if "Stack alignment causes use of alloca" { nvptx-*-* } } */

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

typedef __SIZE_TYPE__ size_t;
#define ALIGNMENT 256
int main(void)
{
  int a[ALIGNMENT/sizeof(int)] __attribute__((aligned(ALIGNMENT)));
  check (&a, ALIGNMENT);
  return 0;
}
