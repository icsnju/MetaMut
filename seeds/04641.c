/* { dg-do run { target aarch64_sve256_hw } } */
/* { dg-options "-O2 -msve-vector-bits=256" } */

/* { dg-do assemble { target aarch64_asm_sve_ok } } */
/* { dg-options "-O -msve-vector-bits=256 --save-temps" } */
/* { dg-final { check-function-bodies "**" "" } } */

/* Case 2.2: Leading constants with stepped sequence.  */

#include <stdint.h>

typedef int32_t vnx4si __attribute__((vector_size (32)));

/*
** foo:
**	...
**	ld1rd	(z[0-9]+)\.d, p[0-9]+/z, \[x[0-9]+\]
**	insr	\1\.s, w1
**	insr	\1\.s, w0
**	rev	\1\.s, \1\.s
**	...
*/
__attribute__((noipa))
vnx4si foo(int a, int b)
{
  return (vnx4si) { 3, 2, 3, 2, 3, 2, b, a };
}

int main()
{
  int a = 10;
  int b = 11;

  vnx4si v = foo (a, b);
  int expected[] = { 3, 2, 3, 2, 3, 2, b, a };

  for (int i = 0; i < 8; i++)
    if (v[i] != expected[i])
      __builtin_abort ();

  return 0;
}
