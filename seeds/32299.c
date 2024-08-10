/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-Ofast -mtune=thunderx" } */
/* { dg-options "-Ofast -mtune=thunderx -mtune=thunderx" { target aarch64_sve256_hw } } */

/* { dg-do compile } */
/* Pick an arbitrary target for which unaligned accesses are more
   expensive.  */
/* { dg-options "-Ofast -msve-vector-bits=256 -mtune=thunderx -fno-vect-cost-model" } */

#define START 1
#define END 505

void __attribute__((noinline, noclone))
foo (double *x)
{
  double v = 10.0;
  for (unsigned int i = START; i < END; ++i)
    {
      x[i] = v;
      v += 5.0;
    }
}

/* We should operate on aligned vectors.  */
/* { dg-final { scan-assembler {\tubfx\t} } } */

int __attribute__ ((optimize (1)))
main (void)
{
  double x[END + 1];
  for (int i = 0; i < END + 1; ++i)
    {
      x[i] = i;
      asm volatile ("" ::: "memory");
    }
  foo (x);
  for (int i = 0; i < END + 1; ++i)
    {
      double expected;
      if (i < START || i >= END)
	expected = i;
      else
	expected = 10 + (i - START) * 5;
      if (x[i] != expected)
	__builtin_abort ();
      asm volatile ("" ::: "memory");
    }
  return 0;
}
