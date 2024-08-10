/* { dg-do compile } */
/* { dg-options "-march=rv32gc_zve32f -mabi=ilp32d -fno-vect-cost-model --param riscv-autovec-preference=fixed-vlmax -fdump-tree-vect-details" } */

#include <stddef.h>
#include <stdint-gcc.h>

void
foo0 (int8_t *__restrict f, int16_t *__restrict d, int n)
{
  for (int i = 0; i < n; ++i)
    {
      f[i * 2 + 0] = 1;
      f[i * 2 + 1] = 2;
      d[i] = 3;
    }
} 

void
foo1 (int16_t *__restrict f, int32_t *__restrict d, int n)
{
  for (int i = 0; i < n; ++i)
    {
      f[i * 2 + 0] = 1;
      f[i * 2 + 1] = 2;
      d[i] = 3;
    }
} 

void
foo2 (int32_t *__restrict f, int64_t *__restrict d, int n)
{
  for (int i = 0; i < n; ++i)
    {
      f[i * 2 + 0] = 1;
      f[i * 2 + 1] = 2;
      d[i] = 3;
    }
}

void
foo3 (int16_t *__restrict f, float *__restrict d, int n)
{
  for (int i = 0; i < n; ++i)
    {
      f[i * 2 + 0] = 1;
      f[i * 2 + 1] = 2;
      d[i] = 3;
    }
} 

void
foo4 (int32_t *__restrict f, float *__restrict d, int n)
{
  for (int i = 0; i < n; ++i)
    {
      f[i * 2 + 0] = 1;
      f[i * 2 + 1] = 2;
      d[i] = 3;
    }
} 

void
foo5 (float *__restrict f, double *__restrict d, int n)
{
  for (int i = 0; i < n; ++i)
    {
      f[i * 2 + 0] = 1;
      f[i * 2 + 1] = 2;
      d[i] = 3;
    }
} 

/* { dg-final { scan-tree-dump-times "vectorized 1 loops in function" 3 "vect" } } */
