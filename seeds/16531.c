/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param riscv-autovec-preference=fixed-vlmax" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv -mabi=ilp32d --param riscv-autovec-preference=fixed-vlmax" } */

#include <stdint-gcc.h>

void __attribute__ ((noinline, noclone))
f (uint64_t *__restrict x, uint16_t *__restrict y, int n)
{
  for (int i = 0, j = 0; i < n; i += 2, j += 4)
    {
      x[i + 0] += 1;
      x[i + 1] += 2;
      y[j + 0] += 1;
      y[j + 1] += 2;
      y[j + 2] += 3;
      y[j + 3] += 4;
    }
}

void __attribute__ ((optimize (0)))
f_init (uint64_t *__restrict x, uint64_t *__restrict x2, uint16_t *__restrict y,
	uint16_t *__restrict y2, int n)
{
  for (int i = 0, j = 0; i < n; i += 2, j += 4)
    {
      x[i + 0] = i * 897 + 189;
      x[i + 1] = i * 79 + 55963;
      y[j + 0] = j * 18 + 78642;
      y[j + 1] = j * 9 + 8634;
      y[j + 2] = j * 78 + 2588;
      y[j + 3] = j * 38 + 8932;
  
      x2[i + 0] = i * 897 + 189;
      x2[i + 1] = i * 79 + 55963;
      y2[j + 0] = j * 18 + 78642;
      y2[j + 1] = j * 9 + 8634;
      y2[j + 2] = j * 78 + 2588;
      y2[j + 3] = j * 38 + 8932;
    }
}

void __attribute__ ((optimize (0)))
f_golden (uint64_t *__restrict x, uint16_t *__restrict y, int n)
{
  for (int i = 0, j = 0; i < n; i += 2, j += 4)
    {
      x[i + 0] += 1;
      x[i + 1] += 2;
      y[j + 0] += 1;
      y[j + 1] += 2;
      y[j + 2] += 3;
      y[j + 3] += 4;
    }
}

void __attribute__ ((optimize (0)))
f_check (uint64_t *__restrict x, uint64_t *__restrict x2,
	 uint16_t *__restrict y, uint16_t *__restrict y2, int n)
{
  for (int i = 0, j = 0; i < n; i += 2, j += 4)
    {
      if (x[i + 0] != x2[i + 0])
	__builtin_abort ();
      if (x[i + 1] != x2[i + 1])
	__builtin_abort ();
      if (y[j + 0] != y2[j + 0])
	__builtin_abort ();
      if (y[j + 1] != y2[j + 1])
	__builtin_abort ();
      if (y[j + 2] != y2[j + 2])
	__builtin_abort ();
      if (y[j + 3] != y2[j + 3])
	__builtin_abort ();
    }
}

int __attribute__ ((optimize (0))) main (void)
{
  uint64_t f_x[3108], f_x2[3108];
  uint16_t f_y[6216], f_y2[6216];
  f_init (f_x, f_x2, f_y, f_y2, 3108);
  f (f_x, f_y, 3108);
  f_golden (f_x2, f_y2, 3108);
  f_check (f_x, f_x2, f_y, f_y2, 3108);
  return 0;
}
