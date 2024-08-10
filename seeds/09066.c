/* { dg-do run { target aarch64_sve256_hw } } */
/* { dg-options "-O3 -msve-vector-bits=256 --param vect-partial-vector-usage=1" } */

#define N 0x1100

/* { dg-options "-O3 --param vect-partial-vector-usage=1" } */

int __attribute__((noipa))
add_loop (int *x, int n, int res)
{
  for (int i = 0; i < n; ++i)
    {
      res += x[i * 2];
      res += x[i * 2 + 1];
    }
  return res;
}

/* { dg-final { scan-assembler-times {\tadd\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tadd\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tuaddv\t} 1 } } */

int
main (void)
{
  int x[N];
  for (int i = 0; i < N; ++i)
    x[i] = ((i + 1) * (i + 2)) & 0xfffff;

  if (add_loop (x, 0, 33) != 33
      || add_loop (x, 11, 30) != 4078
      || add_loop (x, 0x100, 45) != 45001773
      || add_loop (x, 0x11f, 300) != 63369900)
    __builtin_abort ();

  return 0;
}
