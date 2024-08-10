/* { dg-do run { target aarch64_sve256_hw } } */
/* { dg-options "-O3 -msve-vector-bits=256 --param vect-partial-vector-usage=1" } */

#define N 0x1100

/* { dg-options "-O3 -msve-vector-bits=256 --param vect-partial-vector-usage=1" } */

unsigned short __attribute__((noipa))
add_loop (unsigned short *x)
{
  unsigned short res = 0;
  for (int i = 0; i < 0xfff; ++i)
    res += x[i];
  return res;
}

unsigned short __attribute__((noipa))
min_loop (unsigned short *x)
{
  unsigned short res = ~0;
  for (int i = 0; i < 0xfff; ++i)
    res = res < x[i] ? res : x[i];
  return res;
}

unsigned short __attribute__((noipa))
max_loop (unsigned short *x)
{
  unsigned short res = 0;
  for (int i = 0; i < 0xfff; ++i)
    res = res > x[i] ? res : x[i];
  return res;
}

unsigned short __attribute__((noipa))
and_loop (unsigned short *x)
{
  unsigned short res = ~0;
  for (int i = 0; i < 0xfff; ++i)
    res &= x[i];
  return res;
}

unsigned short __attribute__((noipa))
or_loop (unsigned short *x)
{
  unsigned short res = 0;
  for (int i = 0; i < 0xfff; ++i)
    res |= x[i];
  return res;
}

unsigned short __attribute__((noipa))
eor_loop (unsigned short *x)
{
  unsigned short res = 0;
  for (int i = 0; i < 0xfff; ++i)
    res ^= x[i];
  return res;
}

/* { dg-final { scan-assembler-times {\tadd\tz[0-9]+\.h, z[0-9]+\.h, z[0-9]+\.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\tadd\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h, z[0-9]+\.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\tuaddv\t} 1 } } */

/* { dg-final { scan-assembler-times {\tumin\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h, z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\tuminv\t} 1 } } */

/* { dg-final { scan-assembler-times {\tumax\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h, z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\tumaxv\t} 1 } } */

/* { dg-final { scan-assembler-times {\tand\tz[0-9]+\.d, z[0-9]+\.d, z[0-9]+\.d\n} 1 } } */
/* { dg-final { scan-assembler-times {\tand\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h, z[0-9]+\.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\tandv\t} 1 } } */

/* { dg-final { scan-assembler-times {\torr\tz[0-9]+\.d, z[0-9]+\.d, z[0-9]+\.d\n} 1 } } */
/* { dg-final { scan-assembler-times {\torr\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h, z[0-9]+\.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\torv\t} 1 } } */

/* { dg-final { scan-assembler-times {\teor\tz[0-9]+\.d, z[0-9]+\.d, z[0-9]+\.d\n} 1 } } */
/* { dg-final { scan-assembler-times {\teor\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h, z[0-9]+\.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\teorv\t} 1 } } */

int
main (void)
{
  unsigned short x[N];
  for (int i = 0; i < N; ++i)
    x[i] = (i + 1) * (i + 2);

  if (add_loop (x) != 20480
      || max_loop (x) != 65504
      || or_loop (x) != 0xfffe
      || eor_loop (x) != 0xa000)
    __builtin_abort ();

  for (int i = 0; i < N; ++i)
    x[i] = ~x[i];

  if (min_loop (x) != 31
      || and_loop (x) != 1)
    __builtin_abort ();

  return 0;
}
