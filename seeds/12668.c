/* { dg-do run } */
/* { dg-skip-if "Stack alignment is too small" { hppa*-*-hpux* } } */
/* { dg-require-effective-target alloca } */

#include <stdarg.h>
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

void
bar (char *p, int size)
{
  __builtin_strncpy (p, "good", size);
}

void
test (va_list arg)
{
  char *p;
  aligned i;
  int size;
  double x;

  size = va_arg (arg, int);
  if (size != 5)
    abort ();

  p = __builtin_alloca (size + 1);

  x = va_arg (arg, double);
  if (x != 5.0)
    abort ();

  bar (p, size);
  if (__builtin_strncmp (p, "good", size) != 0)
    {
#ifdef DEBUG
      p[size] = '\0';
      printf ("Failed: %s != good\n", p);
#endif
      abort ();
    }

  if (check_int (&i,  __alignof__(i)) != i)
    abort ();
}

void
foo (const char *fmt, ...)
{
  va_list arg;
  va_start (arg, fmt);
  test (arg);
  va_end (arg);
}
int
main()
{
  foo ("foo", 5, 5.0);
  return 0;
}
