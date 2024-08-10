/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize --save-temps" } */

#define TYPE double
#define ITYPE int64_t
/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize --save-temps" } */

/* { dg-do assemble { target aarch64_asm_sve_ok } } */
/* { dg-options "-O2 -ftree-vectorize --save-temps" } */

#include <stdint.h>

#ifndef TYPE
#define TYPE uint8_t
#define ITYPE int8_t
#endif

void __attribute__ ((noinline, noclone))
f2 (TYPE *__restrict a, TYPE *__restrict b, TYPE *__restrict c, ITYPE n)
{
  for (ITYPE i = 0; i < n; ++i)
    {
      a[i] = c[i * 2];
      b[i] = c[i * 2 + 1];
    }
}

void __attribute__ ((noinline, noclone))
f3 (TYPE *__restrict a, TYPE *__restrict b, TYPE *__restrict c,
    TYPE *__restrict d, ITYPE n)
{
  for (ITYPE i = 0; i < n; ++i)
    {
      a[i] = d[i * 3];
      b[i] = d[i * 3 + 1];
      c[i] = d[i * 3 + 2];
    }
}

void __attribute__ ((noinline, noclone))
f4 (TYPE *__restrict a, TYPE *__restrict b, TYPE *__restrict c,
    TYPE *__restrict d, TYPE *__restrict e, ITYPE n)
{
  for (ITYPE i = 0; i < n; ++i)
    {
      a[i] = e[i * 4];
      b[i] = e[i * 4 + 1];
      c[i] = e[i * 4 + 2];
      d[i] = e[i * 4 + 3];
    }
}

void __attribute__ ((noinline, noclone))
g2 (TYPE *__restrict a, TYPE *__restrict b, TYPE *__restrict c, ITYPE n)
{
  for (ITYPE i = 0; i < n; ++i)
    {
      c[i * 2] = a[i];
      c[i * 2 + 1] = b[i];
    }
}

void __attribute__ ((noinline, noclone))
g3 (TYPE *__restrict a, TYPE *__restrict b, TYPE *__restrict c,
    TYPE *__restrict d, ITYPE n)
{
  for (ITYPE i = 0; i < n; ++i)
    {
      d[i * 3] = a[i];
      d[i * 3 + 1] = b[i];
      d[i * 3 + 2] = c[i];
    }
}

void __attribute__ ((noinline, noclone))
g4 (TYPE *__restrict a, TYPE *__restrict b, TYPE *__restrict c,
    TYPE *__restrict d, TYPE *__restrict e, ITYPE n)
{
  for (ITYPE i = 0; i < n; ++i)
    {
      e[i * 4] = a[i];
      e[i * 4 + 1] = b[i];
      e[i * 4 + 2] = c[i];
      e[i * 4 + 3] = d[i];
    }
}

/* { dg-final { scan-assembler {\tld2b\t{z[0-9]+.b - z[0-9]+.b}, p[0-7]/z, \[x[0-9]+, x[0-9]+\]\n} } } */
/* { dg-final { scan-assembler {\tld3b\t{z[0-9]+.b - z[0-9]+.b}, p[0-7]/z, \[x[0-9]+\]\n} } } */
/* { dg-final { scan-assembler {\tld4b\t{z[0-9]+.b - z[0-9]+.b}, p[0-7]/z, \[x[0-9]+, x[0-9]+\]\n} } } */
/* { dg-final { scan-assembler {\tst2b\t{z[0-9]+.b - z[0-9]+.b}, p[0-7], \[x[0-9]+, x[0-9]+\]\n} } } */
/* { dg-final { scan-assembler {\tst3b\t{z[0-9]+.b - z[0-9]+.b}, p[0-7], \[x[0-9]+\]\n} } } */
/* { dg-final { scan-assembler {\tst4b\t{z[0-9]+.b - z[0-9]+.b}, p[0-7], \[x[0-9]+, x[0-9]+\]\n} } } */

#define N 93

TYPE a[N], b[N], c[N], d[N], e[N * 4];

void __attribute__ ((noinline, noclone))
init_array (TYPE *array, int n, TYPE base, TYPE step)
{
  for (int i = 0; i < n; ++i)
    array[i] = base + step * i;
}

void __attribute__ ((noinline, noclone))
check_array (TYPE *array, int n, TYPE base, TYPE step)
{
  for (int i = 0; i < n; ++i)
    if (array[i] != (TYPE) (base + step * i))
      __builtin_abort ();
}

int __attribute__ ((optimize (1)))
main (void)
{
  init_array (e, 2 * N, 11, 5);
  f2 (a, b, e, N);
  check_array (a, N, 11, 10);
  check_array (b, N, 16, 10);

  init_array (e, 3 * N, 7, 6);
  f3 (a, b, c, e, N);
  check_array (a, N, 7, 18);
  check_array (b, N, 13, 18);
  check_array (c, N, 19, 18);

  init_array (e, 4 * N, 4, 11);
  f4 (a, b, c, d, e, N);
  check_array (a, N, 4, 44);
  check_array (b, N, 15, 44);
  check_array (c, N, 26, 44);
  check_array (d, N, 37, 44);

  init_array (a, N, 2, 8);
  init_array (b, N, 6, 8);
  g2 (a, b, e, N);
  check_array (e, 2 * N, 2, 4);

  init_array (a, N, 4, 15);
  init_array (b, N, 9, 15);
  init_array (c, N, 14, 15);
  g3 (a, b, c, e, N);
  check_array (e, 3 * N, 4, 5);

  init_array (a, N, 14, 36);
  init_array (b, N, 23, 36);
  init_array (c, N, 32, 36);
  init_array (d, N, 41, 36);
  g4 (a, b, c, d, e, N);
  check_array (e, 4 * N, 14, 9);

  return 0;
}
