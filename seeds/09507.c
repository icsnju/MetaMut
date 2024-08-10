/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "-std=c99 --param=riscv-autovec-preference=fixed-vlmax -funroll-all-loops -fno-schedule-insns -fno-schedule-insns2" } */

#define TYPE uint16_t
/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "-std=c99 --param=riscv-autovec-preference=fixed-vlmax -funroll-all-loops -fno-schedule-insns -fno-schedule-insns2" } */

/* { dg-do compile } */
/* { dg-additional-options "-std=c99 -march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=fixed-vlmax -funroll-all-loops -fno-schedule-insns -fno-schedule-insns2" } */

#include <stdint-gcc.h>
#ifndef TYPE
#define TYPE uint8_t
#endif

#ifndef NAME
#define NAME(X) X
#endif

#ifndef N
#define N 1024
#endif

void __attribute__ ((noinline, noclone))
NAME(f2) (TYPE *__restrict a, TYPE *__restrict b, TYPE *__restrict c)
{
  for (int i = 0; i < N; ++i)
    {
      a[i] = c[i * 2];
      b[i] = c[i * 2 + 1];
    }
}

void __attribute__ ((noinline, noclone))
NAME(f3) (TYPE *__restrict a, TYPE *__restrict b, TYPE *__restrict c,
	  TYPE *__restrict d)
{
  for (int i = 0; i < N; ++i)
    {
      a[i] = d[i * 3];
      b[i] = d[i * 3 + 1];
      c[i] = d[i * 3 + 2];
    }
}

void __attribute__ ((noinline, noclone))
NAME(f4) (TYPE *__restrict a, TYPE *__restrict b, TYPE *__restrict c,
	  TYPE *__restrict d, TYPE *__restrict e)
{
  for (int i = 0; i < N; ++i)
    {
      a[i] = e[i * 4];
      b[i] = e[i * 4 + 1];
      c[i] = e[i * 4 + 2];
      d[i] = e[i * 4 + 3];
    }
}

void __attribute__ ((noinline, noclone))
NAME(f5) (TYPE *__restrict a, TYPE *__restrict b, TYPE *__restrict c,
	  TYPE *__restrict d, TYPE *__restrict e, TYPE *__restrict f)
{
  for (int i = 0; i < N; ++i)
    {
      a[i] = f[i * 5];
      b[i] = f[i * 5 + 1];
      c[i] = f[i * 5 + 2];
      d[i] = f[i * 5 + 3];
      e[i] = f[i * 5 + 4];
    }
}

void __attribute__ ((noinline, noclone))
NAME(f6) (TYPE *__restrict a, TYPE *__restrict b, TYPE *__restrict c,
	  TYPE *__restrict d, TYPE *__restrict e, TYPE *__restrict f,
	  TYPE *__restrict g)
{
  for (int i = 0; i < N; ++i)
    {
      a[i] = g[i * 6];
      b[i] = g[i * 6 + 1];
      c[i] = g[i * 6 + 2];
      d[i] = g[i * 6 + 3];
      e[i] = g[i * 6 + 4];
      f[i] = g[i * 6 + 5];
    }
}

void __attribute__ ((noinline, noclone))
NAME(f7) (TYPE *__restrict a, TYPE *__restrict b, TYPE *__restrict c,
	  TYPE *__restrict d, TYPE *__restrict e, TYPE *__restrict f,
	  TYPE *__restrict g, TYPE *__restrict h)
{
  for (int i = 0; i < N; ++i)
    {
      a[i] = h[i * 7];
      b[i] = h[i * 7 + 1];
      c[i] = h[i * 7 + 2];
      d[i] = h[i * 7 + 3];
      e[i] = h[i * 7 + 4];
      f[i] = h[i * 7 + 5];
      g[i] = h[i * 7 + 6];
    }
}

void __attribute__ ((noinline, noclone))
NAME(f8) (TYPE *__restrict a, TYPE *__restrict b, TYPE *__restrict c,
	  TYPE *__restrict d, TYPE *__restrict e, TYPE *__restrict f,
	  TYPE *__restrict g, TYPE *__restrict h, TYPE *__restrict j)
{
  for (int i = 0; i < N; ++i)
    {
      a[i] = j[i * 8];
      b[i] = j[i * 8 + 1];
      c[i] = j[i * 8 + 2];
      d[i] = j[i * 8 + 3];
      e[i] = j[i * 8 + 4];
      f[i] = j[i * 8 + 5];
      g[i] = j[i * 8 + 6];
      h[i] = j[i * 8 + 7];
    }
}

void __attribute__ ((noinline, noclone))
NAME(g2) (TYPE *__restrict a, TYPE *__restrict b, TYPE *__restrict c)
{
  for (int i = 0; i < N; ++i)
    {
      c[i * 2] = a[i];
      c[i * 2 + 1] = b[i];
    }
}

void __attribute__ ((noinline, noclone))
NAME(g3) (TYPE *__restrict a, TYPE *__restrict b, TYPE *__restrict c,
	  TYPE *__restrict d)
{
  for (int i = 0; i < N; ++i)
    {
      d[i * 3] = a[i];
      d[i * 3 + 1] = b[i];
      d[i * 3 + 2] = c[i];
    }
}

void __attribute__ ((noinline, noclone))
NAME(g4) (TYPE *__restrict a, TYPE *__restrict b, TYPE *__restrict c,
	  TYPE *__restrict d, TYPE *__restrict e)
{
  for (int i = 0; i < N; ++i)
    {
      e[i * 4] = a[i];
      e[i * 4 + 1] = b[i];
      e[i * 4 + 2] = c[i];
      e[i * 4 + 3] = d[i];
    }
}

void __attribute__ ((noinline, noclone))
NAME(g5) (TYPE *__restrict a, TYPE *__restrict b, TYPE *__restrict c,
	  TYPE *__restrict d, TYPE *__restrict e, TYPE *__restrict f)
{
  for (int i = 0; i < N; ++i)
    {
      f[i * 5] = a[i];
      f[i * 5 + 1] = b[i];
      f[i * 5 + 2] = c[i];
      f[i * 5 + 3] = d[i];
      f[i * 5 + 4] = e[i];
    }
}

void __attribute__ ((noinline, noclone))
NAME(g6) (TYPE *__restrict a, TYPE *__restrict b, TYPE *__restrict c,
	  TYPE *__restrict d, TYPE *__restrict e, TYPE *__restrict f,
	  TYPE *__restrict g)
{
  for (int i = 0; i < N; ++i)
    {
      g[i * 6] = a[i];
      g[i * 6 + 1] = b[i];
      g[i * 6 + 2] = c[i];
      g[i * 6 + 3] = d[i];
      g[i * 6 + 4] = e[i];
      g[i * 6 + 5] = f[i];
    }
}

void __attribute__ ((noinline, noclone))
NAME(g7) (TYPE *__restrict a, TYPE *__restrict b, TYPE *__restrict c,
	  TYPE *__restrict d, TYPE *__restrict e, TYPE *__restrict f,
	  TYPE *__restrict g, TYPE *__restrict h)
{
  for (int i = 0; i < N; ++i)
    {
      h[i * 7] = a[i];
      h[i * 7 + 1] = b[i];
      h[i * 7 + 2] = c[i];
      h[i * 7 + 3] = d[i];
      h[i * 7 + 4] = e[i];
      h[i * 7 + 5] = f[i];
      h[i * 7 + 6] = g[i];
    }
}

void __attribute__ ((noinline, noclone))
NAME(g8) (TYPE *__restrict a, TYPE *__restrict b, TYPE *__restrict c,
	  TYPE *__restrict d, TYPE *__restrict e, TYPE *__restrict f,
	  TYPE *__restrict g, TYPE *__restrict h, TYPE *__restrict j)
{
  for (int i = 0; i < N; ++i)
    {
      j[i * 8] = a[i];
      j[i * 8 + 1] = b[i];
      j[i * 8 + 2] = c[i];
      j[i * 8 + 3] = d[i];
      j[i * 8 + 4] = e[i];
      j[i * 8 + 5] = f[i];
      j[i * 8 + 6] = g[i];
      j[i * 8 + 7] = h[i];
    }
}

/* { dg-final { scan-assembler-times {vlseg2e8\.v} 8 } } */
/* { dg-final { scan-assembler-times {vlseg3e8\.v} 8 } } */
/* { dg-final { scan-assembler-times {vlseg4e8\.v} 8 } } */
/* { dg-final { scan-assembler-times {vlseg5e8\.v} 8 } } */
/* { dg-final { scan-assembler-times {vlseg6e8\.v} 8 } } */
/* { dg-final { scan-assembler-times {vlseg7e8\.v} 4 } } */
/* { dg-final { scan-assembler-times {vlseg8e8\.v} 4 } } */
/* { dg-final { scan-assembler-times {vsseg2e8\.v} 8 } } */
/* { dg-final { scan-assembler-times {vsseg3e8\.v} 8 } } */
/* { dg-final { scan-assembler-times {vsseg4e8\.v} 4 } } */
/* { dg-final { scan-assembler-times {vsseg5e8\.v} 4 } } */
/* { dg-final { scan-assembler-times {vsseg6e8\.v} 4 } } */
/* { dg-final { scan-assembler-times {vsseg7e8\.v} 4 } } */
/* { dg-final { scan-assembler-times {vsseg8e8\.v} 2 } } */
/* { dg-final { scan-assembler-times {vsetivli\s+zero,\s*16,\s*e8,\s*m1,\s*t[au],\s*m[au]} 14 } } */
/* { dg-final { scan-assembler-not {vsetvli} } } */

TYPE a[N], b[N], c[N], d[N], a2[N], b2[N], c2[N], d2[N], e[N * 8];

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
  f2 (a, b, e);
  check_array (a, N, 11, 10);
  check_array (b, N, 16, 10);

  init_array (e, 3 * N, 7, 6);
  f3 (a, b, c, e);
  check_array (a, N, 7, 18);
  check_array (b, N, 13, 18);
  check_array (c, N, 19, 18);

  init_array (e, 4 * N, 4, 11);
  f4 (a, b, c, d, e);
  check_array (a, N, 4, 44);
  check_array (b, N, 15, 44);
  check_array (c, N, 26, 44);
  check_array (d, N, 37, 44);

  init_array (e, 5 * N, 3, 9);
  f5 (a, b, c, d, a2, e);
  check_array (a, N, 3, 45);
  check_array (b, N, 12, 45);
  check_array (c, N, 21, 45);
  check_array (d, N, 30, 45);
  check_array (a2, N, 39, 45);

  init_array (e, 6 * N, 5, 5);
  f6 (a, b, c, d, a2, b2, e);
  check_array (a, N, 5, 30);
  check_array (b, N, 10, 30);
  check_array (c, N, 15, 30);
  check_array (d, N, 20, 30);
  check_array (a2, N, 25, 30);
  check_array (b2, N, 30, 30);

  init_array (e, 7 * N, 7, 3);
  f7 (a, b, c, d, a2, b2, c2, e);
  check_array (a, N, 7, 21);
  check_array (b, N, 10, 21);
  check_array (c, N, 13, 21);
  check_array (d, N, 16, 21);
  check_array (a2, N, 19, 21);
  check_array (b2, N, 22, 21);
  check_array (c2, N, 25, 21);

  init_array (e, 8 * N, 5, 8);
  f8 (a, b, c, d, a2, b2, c2, d2, e);
  check_array (a, N, 5, 64);
  check_array (b, N, 13, 64);
  check_array (c, N, 21, 64);
  check_array (d, N, 29, 64);
  check_array (a2, N, 37, 64);
  check_array (b2, N, 45, 64);
  check_array (c2, N, 53, 64);
  check_array (d2, N, 61, 64);

  init_array (a, N, 2, 8);
  init_array (b, N, 6, 8);
  g2 (a, b, e);
  check_array (e, 2 * N, 2, 4);

  init_array (a, N, 4, 15);
  init_array (b, N, 9, 15);
  init_array (c, N, 14, 15);
  g3 (a, b, c, e);
  check_array (e, 3 * N, 4, 5);

  init_array (a, N, 14, 36);
  init_array (b, N, 23, 36);
  init_array (c, N, 32, 36);
  init_array (d, N, 41, 36);
  g4 (a, b, c, d, e);
  check_array (e, 4 * N, 14, 9);

  init_array (a, N, 3, 45);
  init_array (b, N, 12, 45);
  init_array (c, N, 21, 45);
  init_array (d, N, 30, 45);
  init_array (a2, N, 39, 45);
  g5 (a, b, c, d, a2, e);
  check_array (e, 5 * N, 3, 9);

  init_array (a, N, 5, 30);
  init_array (b, N, 10, 30);
  init_array (c, N, 15, 30);
  init_array (d, N, 20, 30);
  init_array (a2, N, 25, 30);
  init_array (b2, N, 30, 30);
  g6 (a, b, c, d, a2, b2, e);
  check_array (e, 6 * N, 5, 5);

  init_array (a, N, 7, 21);
  init_array (b, N, 10, 21);
  init_array (c, N, 13, 21);
  init_array (d, N, 16, 21);
  init_array (a2, N, 19, 21);
  init_array (b2, N, 22, 21);
  init_array (c2, N, 25, 21);
  g7 (a, b, c, d, a2, b2, c2, e);
  check_array (e, 7 * N, 7, 3);

  init_array (a, N, 5, 64);
  init_array (b, N, 13, 64);
  init_array (c, N, 21, 64);
  init_array (d, N, 29, 64);
  init_array (a2, N, 37, 64);
  init_array (b2, N, 45, 64);
  init_array (c2, N, 53, 64);
  init_array (d2, N, 61, 64);
  g8 (a, b, c, d, a2, b2, c2, d2, e);
  check_array (e, 8 * N, 5, 8);

  return 0;
}
