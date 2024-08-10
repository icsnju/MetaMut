/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize" } */

#include <stdint.h>

void __attribute__ ((noinline, noclone))
unpack_double_int_plus9 (double *d, uint32_t *s, int size)
{
  for (int i = 0; i < size; i++)
    d[i] = (double) (s[i] + 9);
}

/* { dg-final { scan-assembler-times {\tzip1\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tzip2\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tucvtf\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.s\n} 2 } } */

#define ARRAY_SIZE 153

#define VAL1 ((unsigned int) ((i * 345435) - (21 * 345435)))

int __attribute__ ((optimize (1)))
main (void)
{
  double array_dest[ARRAY_SIZE];
  uint32_t array_source[ARRAY_SIZE];

  for (int i = 0; i < ARRAY_SIZE; i++)
    {
      array_source[i] = VAL1;
      asm volatile ("" ::: "memory");
    }

  unpack_double_int_plus9 (array_dest, array_source, ARRAY_SIZE);
  for (int i = 0; i < ARRAY_SIZE; i++)
    if (array_dest[i] != (double) (VAL1 + 9))
      __builtin_abort ();

  return 0;
}
