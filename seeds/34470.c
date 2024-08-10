/* { dg-do compile } */
/* { dg-options "-O2 -mdejagnu-cpu=power9" } */
/* { dg-require-effective-target int128 } */
/* { dg-require-effective-target powerpc_p9modulo_ok } */
/* { dg-require-effective-target has_arch_ppc64 } */
/* { dg-final { scan-assembler-times {\mmaddld\M} 1 } } */
/* { dg-final { scan-assembler-times {\mmaddhd\M} 0 } } */
/* { dg-final { scan-assembler-times {\mmaddhdu\M} 1 } } */

__attribute__((noinline))
__int128 multiply_add (long a, long b, long c)
{
  return (__int128) a * b + c;
}

__attribute__((noinline))
unsigned __int128 multiply_addu (unsigned long a, unsigned long b,
				 unsigned long c)
{
  return (unsigned __int128) a * b + c;
}
