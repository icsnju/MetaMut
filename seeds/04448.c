/* { dg-do compile } */
/* { dg-options "-O -march=armv8.5-a+sve2" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

typedef long v2di  __attribute__((vector_size (16)));

/*
** foo:
**	mul	z0.d, z0.d, z1.d
**	ret
*/

v2di
foo (v2di a, v2di b)
{
  return a * b;
}

/*
** foo_imm:
**	mul	z0.d, z0.d, #125
**	ret
*/

v2di
foo_imm (v2di a)
{
  return a * 125;
}

