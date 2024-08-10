/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize" } */

#include <stdint.h>

void __attribute__ ((noinline, noclone))
pack_int_double_plus_3 (int32_t *d, double *s, int size)
{
  for (int i = 0; i < size; i++)
    d[i] = s[i] + 3;
}

/* { dg-final { scan-assembler-times {\tfcvtzs\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.d\n} 2 } } */
/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */

#define ARRAY_SIZE 107

#define VAL1 ((i * 345.434) - (19 * 345.434))

int __attribute__ ((optimize (1)))
main (void)
{
  static int32_t array_dest[ARRAY_SIZE];
  double array_source[ARRAY_SIZE];

  for (int i = 0; i < ARRAY_SIZE; i++)
    {
      array_source[i] = VAL1;
      asm volatile ("" ::: "memory");
    }

  pack_int_double_plus_3 (array_dest, array_source, ARRAY_SIZE);
  for (int i = 0; i < ARRAY_SIZE; i++)
    if (array_dest[i] != (int32_t) VAL1 + 3)
      __builtin_abort ();

  return 0;
}
