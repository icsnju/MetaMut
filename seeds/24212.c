/* { dg-do compile } */
/* { dg-options "-O3 -march=z14 -mzarch" } */
/* { dg-do run { target { s390_z14_hw } } } */
/* { dg-do compile } */
/* { dg-options "-O3 -march=z14 -mzarch -fno-unroll-loops" } */
#include <stdarg.h>

__attribute__ ((noipa, used)) long double
long_double_callee (long double x, int n, ...)
{
  long double sum = x;
  va_list vl;
  int i;

  va_start (vl, n);
  for (i = 0; i < n; i++)
    sum += va_arg (vl, long double);
  va_end (vl);

  return sum;
}

/* { dg-final { scan-assembler-times {\n\tvl\t} 3 } } */
/* { dg-do compile } */
/* { dg-options "-O3 -march=z14 -mzarch" } */
#include <assert.h>

long double long_double_callee (long double x, int n, ...);

int
main ()
{
  assert (long_double_callee (1.L, 2, 2.L, 3.L) == 6.L);
}

/* { dg-final { scan-assembler-times {\n\tvst\t} 3 } } */
