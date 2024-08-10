/* { dg-do run } */
/* { dg-skip-if "Stack alignment is too small" { hppa*-*-hpux* } } */
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

#ifndef ALIGNMENT
#define ALIGNMENT       64
#endif

extern void abort();
typedef struct my_struct
{
	char str[31];
} stype ;

stype g_s;

stype __attribute__((noinline))
foo (char arg1, char arg2, char arg3)
{
	stype __attribute__((aligned(ALIGNMENT))) s;
	s.str[0] = arg1;
	s.str[1] = arg2;
	s.str[30] = arg3;
	check(&s, ALIGNMENT);
	return s;
}

int main()
{
	g_s = foo(1,2,3);

	if (g_s.str[0] != 1 || g_s.str[1] != 2 || g_s.str[30] !=3)
	  abort();
	return 0;
}
