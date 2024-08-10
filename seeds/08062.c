/* { dg-do run { target aarch64_sve256_hw } } */
/* { dg-options "-O2 -msve-vector-bits=256" } */

/* { dg-do assemble { target aarch64_asm_sve_ok } } */
/* { dg-options "-O -msve-vector-bits=256 --save-temps" } */
/* { dg-final { check-function-bodies "**" "" } } */

/* Case 5.1: All elements.  */ 

#include <stdint.h>

typedef int32_t vnx4si __attribute__((vector_size (32)));

/*
** foo:
**	mov	(z[0-9]+\.s), w7
**	insr	\1, w6
**	insr	\1, w5
**	insr	\1, w4
**	insr	\1, w3
**	insr	\1, w2
**	insr	\1, w1
**	insr	\1, w0
**	...
*/
__attribute__((noipa))
vnx4si foo(int a, int b, int c, int d, int e, int f, int g, int h)
{
  return (vnx4si) { a, b, c, d, e, f, g, h };
}

int main()
{
  int a = 10;
  int b = 11;
  int c = 12;
  int d = 13;
  int e = 14;
  int f = 15;
  int g = 16;
  int h = 17;

  vnx4si v = foo (a, b, c, d, e, f, g, h);
  int expected[] = { a, b, c, d, e, f, g, h };

  for (int i = 0; i < 8; i++)
    if (v[i] != expected[i])
      __builtin_abort ();

  return 0;
}
