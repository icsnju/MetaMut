/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize" } */

void __attribute__ ((noinline, noclone))
pack_float_plus_1point1 (float *d, double *s, int size)
{
  for (int i = 0; i < size; i++)
    d[i] = s[i] + 1.1;
}

/* { dg-final { scan-assembler-times {\tfcvt\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.d\n} 2 } } */
/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */

#define ARRAY_SIZE 107

#define VAL1 ((i * 886.556) - (43 * 886.556))

int __attribute__ ((optimize (1)))
main (void)
{
  float array_dest[ARRAY_SIZE];
  double array_source[ARRAY_SIZE];

  for (int i = 0; i < ARRAY_SIZE; i++)
    {
      array_source[i] = VAL1;
      asm volatile ("" ::: "memory");
    }

  pack_float_plus_1point1 (array_dest, array_source, ARRAY_SIZE);
  for (int i = 0; i < ARRAY_SIZE; i++)
    if (array_dest[i] != (float) (VAL1 + 1.1))
      __builtin_abort ();

  return 0;
}
