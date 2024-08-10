/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline" } */

#include <stdint.h>

void __attribute__ ((noinline, noclone))
unpack_double_int_plus8 (double *d, int32_t *s, int size)
{
  for (int i = 0; i < size; i++)
    d[i] = s[i] + 8;
}

/* { dg-final { scan-assembler-times {\tzip1\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tzip2\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tscvtf\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.s\n} 2 } } */

#define ARRAY_SIZE 89

#define VAL1 ((i * 88654) - (33 * 88654))

int __attribute__ ((optimize (1)))
main (void)
{
  double array_dest[ARRAY_SIZE];
  int32_t array_source[ARRAY_SIZE];

  for (int i = 0; i < ARRAY_SIZE; i++)
    {
      array_source[i] = VAL1;
      asm volatile ("" ::: "memory");
    }

  unpack_double_int_plus8 (array_dest, array_source, ARRAY_SIZE);
  for (int i = 0; i < ARRAY_SIZE; i++)
    if (array_dest[i] != (double) (VAL1 + 8))
      __builtin_abort ();

  return 0;
}
