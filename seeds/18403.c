/* PR middle-end/37009 */
/* { dg-do run { target { { i?86-*-* x86_64-*-* } && ia32 } } } */
/* { dg-additional-options "-mincoming-stack-boundary=2 -mpreferred-stack-boundary=2" } */

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

void
bar (char *p, int size)
{
  __builtin_strncpy (p, "good", size);
}

void
__attribute__ ((noinline))
foo (double x, double y ,double z ,double a, int size)
{
  char *p = __builtin_alloca (size + 1);
  double i;

  bar (p, size);
  if (__builtin_strncmp (p, "good", size) != 0)
    {
#ifdef DEBUG
      p[size] = '\0';
      printf ("Failed: %s != good\n", p);
#endif
     abort ();
    }

  check (&i, __alignof__(i));
}

int
main (void)
{
  double x =  1.0 ;
 
  foo (x, x, x, x, 5);

  return 0;
}
