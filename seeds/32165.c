/* { dg-do run { target aarch64_sve256_hw } } */
/* { dg-options "-O2 -msve-vector-bits=256" } */

/* { dg-do assemble { target aarch64_asm_sve_ok } } */
/* { dg-options "-O -msve-vector-bits=256 --save-temps" } */
/* { dg-final { check-function-bodies "**" "" } } */

/* Case 5.2: Interleaved elements and constants.  */ 

#include <stdint.h>

typedef int32_t vnx4si __attribute__((vector_size (32)));

/*
** foo:
**	...
**	ld1w	(z[0-9]+\.s), p[0-9]+/z, \[x[0-9]+\]
**	mov	(z[0-9]+\.s), w3
**	insr	\2, w2
**	insr	\2, w1
**	insr	\2, w0
**	zip1	\2, \2, \1
**	...
*/
__attribute__((noipa))
vnx4si foo(int a, int b, int c, int d)
{
  return (vnx4si) { a, 1, b, 2, c, 3, d, 4 }; 
}

int main()
{
  int a = 10;
  int b = 11;
  int c = 12;
  int d = 13;

  vnx4si v = foo (a, b, c, d);
  int expected[] = { a, 1, b, 2, c, 3, d, 4 };

  for (int i = 0; i < 8; i++)
    if (v[i] != expected[i])
      __builtin_abort ();

  return 0;
}
