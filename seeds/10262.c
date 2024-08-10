/* { dg-do run { target aarch64_sve256_hw } } */
/* { dg-options "-O2 -msve-vector-bits=256" } */

/* { dg-do assemble { target aarch64_asm_sve_ok } } */
/* { dg-options "-O -msve-vector-bits=256 --save-temps" } */
/* { dg-final { check-function-bodies "**" "" } } */

/* Case 5.5: Interleaved repeating elements and trailing same elements.  */

#include <stdint.h>

typedef int32_t vnx4si __attribute__((vector_size (32)));

/*
** foo:
**	fmov	(s[0-9]+), w1
**	mov	(z[0-9]+\.s), w2
**	mov	(z[0-9]+\.s), w0
**	insr	\3, \1
**	insr	\3, \1
**	insr	\3, \1
**	zip1	\3, \3, \2
**	...
*/
__attribute__((noipa))
vnx4si foo(int a, int b, int f) 
{
  return (vnx4si) { b, f, b, f, b, f, a, f };
}

int main()
{
  int a = 10;
  int b = 11;
  int f = 12;

  vnx4si v = foo (a, b, f);
  int expected[] = { b, f, b, f, b, f, a, f };

  for (int i = 0; i < 8; i++)
    if (v[i] != expected[i])
      __builtin_abort ();

  return 0;
}
