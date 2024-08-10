/* { dg-do run } */
/* { dg-skip-if "Stack alignment is too small" { hppa*-*-hpux* } } */
/* { dg-require-effective-target alloca } */

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
extern int strncmp (const char *, const char *, size_t);

int global;

void
copy (char *p, int size)
{
  __builtin_strncpy (p, "good", size);
}

void
foo (int size)
{
  aligned j;

   __attribute__ ((__noinline__))
  void bar (int size)
    {
      char *p = __builtin_alloca (size + 1);
      aligned i;

      copy (p, size);
      if (strncmp (p, "good", size) != 0)
	{
#ifdef DEBUG
	  p[size] = '\0';
	  printf ("Failed: %s != good\n", p);
#endif
	  abort ();
	}

      if (check_int (&i,  __alignof__(i)) != i)
	abort ();

      if (check_int (&j,  __alignof__(j)) != j)
	abort ();

      j = -20;
    }
  bar (size);

  if (j != -20)
    abort ();

  if (check_int (&j,  __alignof__(j)) != j)
    abort ();
}

int
main()
{
  foo (5);
  return 0;
}
