/* { dg-do run { target aarch64_sve256_hw } } */
/* { dg-options "-O2 -msve-vector-bits=256" } */

/* { dg-do assemble { target aarch64_asm_sve_ok } } */
/* { dg-options "-O -msve-vector-bits=256 --save-temps" } */
/* { dg-final { check-function-bodies "**" "" } } */

/* Case 3: Trailing same element.  */ 

#include <stdint.h>

typedef int32_t vnx4si __attribute__((vector_size (32)));

/*
** foo:
**	mov	(z[0-9]+\.s), w2
**	insr	\1, w1
**	insr	\1, w0
**	...
*/
__attribute__((noipa))
vnx4si foo(int a, int b, int c)
{
  return (vnx4si) { a, b, c, c, c, c, c, c };
}

int main()
{
  int a = 10;
  int b = 11;
  int c = 12;

  vnx4si v = foo (a, b, c);
  int expected[] = { a, b, c, c, c, c, c, c };

  for (int i = 0; i < 8; i++)
    if (v[i] != expected[i])
      __builtin_abort ();

  return 0;
}
