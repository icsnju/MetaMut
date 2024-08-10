/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O3 -mtune=thunderx -fno-vect-cost-model" } */
/* { dg-options "-O3 -mtune=thunderx -msve-vector-bits=256 -fno-vect-cost-model" { target aarch64_sve256_hw } } */

/* { dg-do compile } */
/* Pick an arbitrary target for which unaligned accesses are more
   expensive.  */
/* { dg-options "-O3 -msve-vector-bits=256 -mtune=thunderx -fno-vect-cost-model" } */

#define N 32
#define MAX_START 8
#define COUNT 16

int x[MAX_START][N] __attribute__((aligned(32)));

void __attribute__((noinline, noclone))
foo (int start)
{
  for (int i = start; i < start + COUNT; ++i)
    x[start][i] = i;
}

/* We should operate on aligned vectors.  */
/* { dg-final { scan-assembler {\t(adrp|adr)\tx[0-9]+, (x|\.LANCHOR0)\n} } } */
/* { dg-final { scan-assembler {\tubfx\t} } } */

int __attribute__ ((optimize (1)))
main (void)
{
  for (int start = 0; start < MAX_START; ++start)
    {
      foo (start);
      for (int i = 0; i < N; ++i)
	{
	  if (x[start][i] != (i < start || i >= start + COUNT ? 0 : i))
	    __builtin_abort ();
	  asm volatile ("" ::: "memory");
	}
    }
  return 0;
}
