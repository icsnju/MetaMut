/* { dg-options "-O -Wno-psabi" } */

#ifdef __x86_64__
#include <stdlib.h>
#include <complex.h>

struct st
{
  int s1;
  float complex x;
  int s2;
};

typedef struct { float r, i; } _complex;

struct stc
{
  int s1;
  _complex x;
  int s2;
};

void
bar(struct st x)
{
  if (x.s1 != 1 || x.s2 != 2
      || __real__ x.x != 2 || __imag__ x.x != 4)
    abort ();
}

void
foo(struct stc x)
{
  if (x.s1 != 1 || x.s2 != 2 || x.x.r != 2 || x.x.i != 4)
    abort ();
}
#else
int dummy_y;
#endif

