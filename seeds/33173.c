/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O3" } */

/* { dg-options "-O3" } */

#include <stdint.h>

#define N 55

void __attribute__ ((noipa))
f (double *restrict a, double *restrict b, double *restrict c,
   double *restrict d, double *restrict e, int64_t *restrict cond)
{
  for (int i = 0; i < N; ++i)
    {
      a[i] = cond[i] ? __builtin_fma (c[i], d[i], e[i]) : e[i];
      b[i] = cond[i] ? __builtin_fma (c[i], e[i], d[i]) : d[i];
    }
}

/* { dg-final { scan-assembler-times {\tfmla\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, z[0-9]+\.d\n} 2 } } */
/* { dg-final { scan-assembler-not {\tfmad\t} } } */
/* { dg-final { scan-assembler-times {\tst1d} 2 } } */

int __attribute__ ((optimize (1)))
main (void)
{
  double a[N], b[N], c[N], d[N], e[N];
  int64_t cond[N];

  for (int i = 0; i < N; ++i)
    {
      c[i] = i + i % 5;
      d[i] = i + i % 7;
      e[i] = i + i % 9;
      cond[i] = i % 3;
    }

  f (a, b, c, d, e, cond);

  for (int i = 0; i < N; ++i)
    if (a[i] != (cond[i] ? __builtin_fma (c[i], d[i], e[i]) : e[i])
	|| b[i] != (cond[i] ? __builtin_fma (c[i], e[i], d[i]) : d[i]))
      __builtin_abort ();

  return 0;
}
