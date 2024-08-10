/* PR middle-end/45234 */
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
__attribute__ ((noinline))
bar (__float128 f)
{
  check (&f, __alignof__(f));
}

int
main (void)
{
  char *p = __builtin_alloca (6);

  bar (0);

  __builtin_strncpy (p, "good", 5);
  if (__builtin_strncmp (p, "good", 5) != 0)
    {
#ifdef DEBUG
      p[5] = '\0';
      printf ("Failed: %s != good\n", p);
#endif
     abort ();
    }

  return 0;
}
