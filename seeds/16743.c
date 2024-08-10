/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O3" } */

/* { dg-do compile } */
/* { dg-options "-O3" } */

void __attribute__((noinline, noclone))
vadd (int *dst, int *op1, int *op2, int count)
{
  for (int i = 0; i < count; ++i)
    dst[i] = op1[i] + op2[i];
}

/* { dg-final { scan-assembler-times {\tld1w\tz[0-9]+\.s, p[0-7]/z,} 2 } } */
/* { dg-final { scan-assembler-times {\tst1w\tz[0-9]+\.s, p[0-7],} 1 } } */
/* { dg-final { scan-assembler-times {\tadd\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */

#define ELEMS 10

int __attribute__ ((optimize (1)))
main (void)
{
  int in1[ELEMS] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  int in2[ELEMS] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
  int out[ELEMS];
  int check[ELEMS] = { 3, 5, 7, 9, 11, 13, 15, 17, 19, 21 };

  vadd (out, in1, in2, ELEMS);

  for (int i = 0; i < ELEMS; ++i)
    if (out[i] != check[i])
      __builtin_abort ();

  return 0;
}
