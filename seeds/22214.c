/* { dg-options "-O -Wno-psabi" } */


#ifdef __x86_64__
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

struct st st1;
struct stc st2;

extern void foo ();
extern void bar ();

int
struct_complex_1_x ()
{
  st1.s1 = 1;
  st1.s2 = 2;
  __real__ st1.x = 2;
  __imag__ st1.x = 4;
  st2.s1 = 1;
  st2.s2 = 2;
  st2.x.r = 2;
  st2.x.i = 4;
  foo (st1);
  foo (st2);
  bar (st1);
  bar (st2);
  return 0;
}
#else
int dummy_x;
#endif

