/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize" } */

void __attribute__ ((noinline, noclone))
unpack_float_plus_7point9 (double *d, float *s, int size)
{
  for (int i = 0; i < size; i++)
    d[i] = s[i] + 7.9;
}

/* { dg-final { scan-assembler-times {\tzip1\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tzip2\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfcvt\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.s\n} 2 } } */

#define ARRAY_SIZE 199

#define VAL1 ((float) ((i * 645.56665) - (645.56665)))

int __attribute__ ((optimize (1)))
main (void)
{
  double array_dest[ARRAY_SIZE];
  float array_source[ARRAY_SIZE];

  for (int i = 0; i < ARRAY_SIZE; i++)
    {
      array_source[i] = VAL1;
      asm volatile ("" ::: "memory");
    }

  unpack_float_plus_7point9 (array_dest, array_source, ARRAY_SIZE);
  for (int i = 0; i < ARRAY_SIZE; i++)
    if (array_dest[i] != (double) (VAL1 + 7.9))
      __builtin_abort ();

  return 0;
}
