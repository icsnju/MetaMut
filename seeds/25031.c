/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O3 -mtune=thunderx" } */
/* { dg-options "-O3 -mtune=thunderx -msve-vector-bits=256 -fno-vect-cost-model" { target aarch64_sve256_hw } } */

/* { dg-do compile } */
/* Pick an arbitrary target for which unaligned accesses are more
   expensive.  */
/* { dg-options "-O3 -msve-vector-bits=256 -mtune=thunderx -fno-vect-cost-model" } */

#define N 512
#define START 7
#define END 22

int x[N] __attribute__((aligned(32)));

void __attribute__((noinline, noclone))
foo (void)
{
  for (unsigned int i = START; i < END; ++i)
    x[i] = i;
}

/* We should operate on aligned vectors.  */
/* { dg-final { scan-assembler {\t(adrp|adr)\tx[0-9]+, (x|\.LANCHOR0)\n} } } */
/* We should unroll the loop three times.  */
/* { dg-final { scan-assembler-times "\tst1w\t" 3 } } */
/* { dg-final { scan-assembler {\tptrue\t(p[0-9]+)\.s, vl7\n.*\teor\tp[0-9]+\.b, (p[0-9]+)/z, (\1\.b, \2\.b|\2\.b, \1\.b)\n} } } */

int __attribute__ ((optimize (1)))
main (void)
{
  foo ();
  for (int i = 0; i < N; ++i)
    {
      if (x[i] != (i < START || i >= END ? 0 : i))
	__builtin_abort ();
      asm volatile ("" ::: "memory");
    }
  return 0;
}
