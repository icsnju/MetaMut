/* Check that GCC does not generate Armv8.1-M low over head loop instructions
   if a non-inlineable function call takes place inside the loop.  */
/* { dg-do compile } */
/* { dg-skip-if "avoid conflicting multilib options" { *-*-* } { "-marm" "-mcpu=*" } } */
/* { dg-options "-march=armv8.1-m.main+fp -mthumb -O3 --save-temps" } */
#include <stdlib.h>
#include <string.h>

/* Common code for lob tests.  */

#define NO_LOB asm volatile ("@ clobber lr" : : : "lr" )

#define N 10000

static void
reset_data (int *a, int *b, int *c)
{
  memset (a, -1, N * sizeof (*a));
  memset (b, -1, N * sizeof (*b));
  memset (c, -1, N * sizeof (*c));
}

int a[N];
int b[N];
int c[N];

int __attribute__ ((noinline))
foo (int a, int b)
{
  return a + b;
}

int
main (void)
{
  for (int i = 0; i < N; i++)
    {
      a[i] = i;
      b[i] = i * 2;
      c[i] = foo (a[i], b[i]);
    }

  return 0;
}
/* { dg-final { scan-assembler-not {dls\s\S*,\s\S*} } } */
/* { dg-final { scan-assembler-not {le\slr,\s\S*} } } */
