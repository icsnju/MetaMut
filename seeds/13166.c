/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param riscv-autovec-preference=fixed-vlmax" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv -mabi=ilp32d --param riscv-autovec-preference=fixed-vlmax" } */

#include <stdint-gcc.h>

void __attribute__ ((noinline, noclone))
f0 (int8_t *__restrict x, int16_t *__restrict y, int n)
{
  for (int i = 0, j = 0; i < n; i += 4, j += 8)
    {
      x[i + 0] += 1;
      x[i + 1] += 2;
      x[i + 2] += 3;
      x[i + 3] += 4;
      y[j + 0] += 1;
      y[j + 1] += 2;
      y[j + 2] += 3;
      y[j + 3] += 4;
      y[j + 4] += 5;
      y[j + 5] += 6;
      y[j + 6] += 7;
      y[j + 7] += 8;
    }
}

void __attribute__ ((optimize (0)))
f0_init (int8_t *__restrict x, int8_t *__restrict x2, int16_t *__restrict y,
	 int16_t *__restrict y2, int n)
{
  for (int i = 0, j = 0; i < n; i += 4, j += 8)
    {
      x[i + 0] = i % 120;
      x[i + 1] = i % 78;
      x[i + 2] = i % 55;
      x[i + 3] = i % 27;
      y[j + 0] = j % 33;
      y[j + 1] = j % 44;
      y[j + 2] = j % 66;
      y[j + 3] = j % 88;
      y[j + 4] = j % 99;
      y[j + 5] = j % 39;
      y[j + 6] = j % 49;
      y[j + 7] = j % 101;

      x2[i + 0] = i % 120;
      x2[i + 1] = i % 78;
      x2[i + 2] = i % 55;
      x2[i + 3] = i % 27;
      y2[j + 0] = j % 33;
      y2[j + 1] = j % 44;
      y2[j + 2] = j % 66;
      y2[j + 3] = j % 88;
      y2[j + 4] = j % 99;
      y2[j + 5] = j % 39;
      y2[j + 6] = j % 49;
      y2[j + 7] = j % 101;
    }
}

void __attribute__ ((optimize (0)))
f0_golden (int8_t *__restrict x, int16_t *__restrict y, int n)
{
  for (int i = 0, j = 0; i < n; i += 4, j += 8)
    {
      x[i + 0] += 1;
      x[i + 1] += 2;
      x[i + 2] += 3;
      x[i + 3] += 4;
      y[j + 0] += 1;
      y[j + 1] += 2;
      y[j + 2] += 3;
      y[j + 3] += 4;
      y[j + 4] += 5;
      y[j + 5] += 6;
      y[j + 6] += 7;
      y[j + 7] += 8;
    }
}

void __attribute__ ((optimize (0)))
f0_check (int8_t *__restrict x, int8_t *__restrict x2, int16_t *__restrict y,
	  int16_t *__restrict y2, int n)
{
  for (int i = 0, j = 0; i < n; i += 4, j += 8)
    {
      if (x[i + 0] != x2[i + 0])
	__builtin_abort ();
      if (x[i + 1] != x2[i + 1])
	__builtin_abort ();
      if (x[i + 2] != x2[i + 2])
	__builtin_abort ();
      if (x[i + 3] != x2[i + 3])
	__builtin_abort ();
      if (y[j + 0] != y2[j + 0])
	__builtin_abort ();
      if (y[j + 1] != y2[j + 1])
	__builtin_abort ();
      if (y[j + 2] != y2[j + 2])
	__builtin_abort ();
      if (y[j + 3] != y2[j + 3])
	__builtin_abort ();
      if (y[j + 4] != y2[j + 4])
	__builtin_abort ();
      if (y[j + 5] != y2[j + 5])
	__builtin_abort ();
      if (y[j + 6] != y2[j + 6])
	__builtin_abort ();
      if (y[j + 7] != y2[j + 7])
	__builtin_abort ();
    }
}

void __attribute__ ((noinline, noclone))
f1 (int16_t *__restrict x, int32_t *__restrict y, int n)
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
f1_init (int16_t *__restrict x, int16_t *__restrict x2, int32_t *__restrict y,
	 int32_t *__restrict y2, int n)
{
  for (int i = 0, j = 0; i < n; i += 2, j += 4)
    {
      x[i + 0] = i % 67;
      x[i + 1] = i % 76;
      y[j + 0] = j % 111;
      y[j + 1] = j % 63;
      y[j + 2] = j % 39;
      y[j + 3] = j % 8;

      x2[i + 0] = i % 67;
      x2[i + 1] = i % 76;
      y2[j + 0] = j % 111;
      y2[j + 1] = j % 63;
      y2[j + 2] = j % 39;
      y2[j + 3] = j % 8;
    }
}

void __attribute__ ((optimize (0)))
f1_golden (int16_t *__restrict x, int32_t *__restrict y, int n)
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
f1_check (int16_t *__restrict x, int16_t *__restrict x2, int32_t *__restrict y,
	  int32_t *__restrict y2, int n)
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

void __attribute__ ((noinline, noclone))
f2 (int32_t *__restrict x, int64_t *__restrict y, int n)
{
  for (int i = 0, j = 0; i < n; i += 1, j += 2)
    {
      x[i + 0] += 1;
      y[j + 0] += 1;
      y[j + 1] += 2;
    }
}

void __attribute__ ((optimize (0)))
f2_init (int32_t *__restrict x, int32_t *__restrict x2, int64_t *__restrict y,
	 int64_t *__restrict y2, int n)
{
  for (int i = 0, j = 0; i < n; i += 1, j += 2)
    {
      x[i + 0] = i % 79;
      y[j + 0] = j % 83;
      y[j + 1] = j % 100;

      x2[i + 0] = i % 79;
      y2[j + 0] = j % 83;
      y2[j + 1] = j % 100;
    }
}

void __attribute__ ((optimize (0)))
f2_golden (int32_t *__restrict x, int64_t *__restrict y, int n)
{
  for (int i = 0, j = 0; i < n; i += 1, j += 2)
    {
      x[i + 0] += 1;
      y[j + 0] += 1;
      y[j + 1] += 2;
    }
}

void __attribute__ ((noinline, noclone))
f2_check (int32_t *__restrict x, int32_t *__restrict x2, int64_t *__restrict y,
	  int64_t *__restrict y2, int n)
{
  for (int i = 0, j = 0; i < n; i += 1, j += 2)
    {
      if (x[i + 0] != x2[i + 0])
	__builtin_abort ();
      if (y[j + 0] != y2[j + 0])
	__builtin_abort ();
      if (y[j + 1] != y2[j + 1])
	__builtin_abort ();
    }
}

void __attribute__ ((noinline, noclone))
f3 (int8_t *__restrict x, int64_t *__restrict y, int n)
{
  for (int i = 0, j = 0; i < n; i += 1, j += 2)
    {
      x[i + 0] += 1;
      y[j + 0] += 1;
      y[j + 1] += 2;
    }
}

void __attribute__ ((noinline, noclone))
f3_init (int8_t *__restrict x, int8_t *__restrict x2, int64_t *__restrict y,
    int64_t *__restrict y2, int n)
{
  for (int i = 0, j = 0; i < n; i += 1, j += 2)
    {
      x[i + 0] = i % 22;
      y[j + 0] = i % 12;
      y[j + 1] = i % 21;

      x2[i + 0] = i % 22;
      y2[j + 0] = i % 12;
      y2[j + 1] = i % 21;
    }
}

void __attribute__ ((optimize (0)))
f3_golden (int8_t *__restrict x, int64_t *__restrict y, int n)
{
  for (int i = 0, j = 0; i < n; i += 1, j += 2)
    {
      x[i + 0] += 1;
      y[j + 0] += 1;
      y[j + 1] += 2;
    }
}

void __attribute__ ((noinline, noclone))
f3_check (int8_t *__restrict x, int8_t *__restrict x2, int64_t *__restrict y,
	  int64_t *__restrict y2, int n)
{
  for (int i = 0, j = 0; i < n; i += 1, j += 2)
    {
      if (x[i + 0] != x2[i + 0])
	__builtin_abort ();
      if (y[j + 0] != y2[j + 0])
	__builtin_abort ();
      if (y[j + 1] != y2[j + 1])
	__builtin_abort ();
    }
}

int __attribute__ ((optimize (0))) main (void)
{
  int8_t f0_x[3108], f0_x2[3108];
  int16_t f0_y[6216], f0_y2[6216];
  f0_init (f0_x, f0_x2, f0_y, f0_y2, 3108);
  f0 (f0_x, f0_y, 3108);
  f0_golden (f0_x2, f0_y2, 3108);
  f0_check (f0_x, f0_x2, f0_y, f0_y2, 3108);

  int16_t f1_x[1998], f1_x2[1998];
  int32_t f1_y[3996], f1_y2[3996];
  f1_init (f1_x, f1_x2, f1_y, f1_y2, 1998);
  f1 (f1_x, f1_y, 1998);
  f1_golden (f1_x2, f1_y2, 1998);
  f1_check (f1_x, f1_x2, f1_y, f1_y2, 1998);

  int32_t f2_x[2023], f2_x2[2023];
  int64_t f2_y[4046], f2_y2[4046];
  f2_init (f2_x, f2_x2, f2_y, f2_y2, 2023);
  f2 (f2_x, f2_y, 2023);
  f2_golden (f2_x2, f2_y2, 2023);
  f2_check (f2_x, f2_x2, f2_y, f2_y2, 2023);

  int8_t f3_x[3203], f3_x2[3203];
  int64_t f3_y[6406], f3_y2[6406];
  f3_init (f3_x, f3_x2, f3_y, f3_y2, 3203);
  f3 (f3_x, f3_y, 3203);
  f3_golden (f3_x2, f3_y2, 3203);
  f3_check (f3_x, f3_x2, f3_y, f3_y2, 3203);
  return 0;
}
