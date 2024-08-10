/* { dg-skip-if "missing optab for vectorization" { sparc*-*-* } } */
/* { dg-additional-options "-O3 -fdump-tree-dce -w" } */

#include <stdint.h>

__attribute__((noinline, noipa))
void fun1(uint32_t *x, int n)
{
    for (int i = 0; i < (n & -16); i++)
      x[i] = (x[i]&(~255)) == 0;
}

__attribute__((noinline, noipa, optimize("O1")))
void fun2(uint32_t *x, int n)
{
    for (int i = 0; i < (n & -16); i++)
      x[i] = (x[i]&(~255)) == 0;
}

#include <stdio.h>

#ifndef N
#define N 65
#endif

#ifndef TYPE
#define TYPE uint32_t
#endif

#ifndef DEBUG
#define DEBUG 0
#endif

#define BASE ((TYPE) -1 < 0 ? -126 : 4)

int main ()
{
  TYPE a[N];
  TYPE b[N];

  for (int i = 0; i < N; ++i)
    {
      a[i] = BASE + i * 13;
      b[i] = BASE + i * 13;
      if (DEBUG)
        printf ("%d: 0x%x\n", i, a[i]);
    }

  fun1 (a, N);
  fun2 (b, N);

  for (int i = 0; i < N; ++i)
    {
      if (DEBUG)
        printf ("%d = 0x%x == 0x%x\n", i, a[i], b[i]);

      if (a[i] != b[i])
        __builtin_abort ();
    }
  return 0;
}


/* { dg-final { scan-tree-dump-times {<=\s*.+\{ 255,.+\}} 1 dce7 { target vect_int } } } */
/* { dg-final { scan-tree-dump-not {&\s*.+\{ 4294967040,.+\}} dce7 { target vect_int } } } */
/* { dg-final { scan-tree-dump-not {\s+bic\s+} dce7 { target { aarch64*-*-* } } } } */

