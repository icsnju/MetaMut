/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize" } */

#include <stdint.h>

void __attribute__ ((noinline, noclone))
pack_int_double_plus_7 (uint32_t *d, double *s, int size)
{
  for (int i = 0; i < size; i++)
    d[i] = s[i] + 7;
}

/* { dg-final { scan-assembler-times {\tfcvtzu\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.d\n} 2 } } */
/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */

#define ARRAY_SIZE 157

#define VAL1 (i * 9584.3432)

int __attribute__ ((optimize (1)))
main (void)
{
  static uint32_t array_dest[ARRAY_SIZE];
  double array_source[ARRAY_SIZE];

  for (int i = 0; i < ARRAY_SIZE; i++)
    {
      array_source[i] = VAL1;
      asm volatile ("" ::: "memory");
    }

  pack_int_double_plus_7 (array_dest, array_source, ARRAY_SIZE);
  for (int i = 0; i < ARRAY_SIZE; i++)
    if (array_dest[i] != (uint32_t) VAL1 + 7)
      __builtin_abort ();

  return 0;
}
