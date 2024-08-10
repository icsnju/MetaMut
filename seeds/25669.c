/* { dg-do run { target sse2_runtime } } */
/* { dg-options "-O2 -msse2" } */

/* { dg-do compile } */
/* { dg-options "-O2 -msse -mno-sse4" } */
/* { dg-final { scan-assembler-not "movlhps" } } */
/* { dg-final { scan-assembler-not "unpckhps" } } */
/* { dg-final { scan-assembler-not "unpcklps" } } */
/* { dg-final { scan-assembler-not "shufps" } } */

typedef float __v4sf __attribute__ ((__vector_size__ (16)));
typedef float __m128 __attribute__ ((__vector_size__ (16), __may_alias__));

__m128
foo1 (__m128 x)
{
  __v4sf z = { 11.4, ((__v4sf) x)[1], ((__v4sf) x)[2], ((__v4sf) x) [3] };
  return (__m128) z;
}
/* { dg-do compile } */
/* { dg-options "-O2 -msse -mno-sse4" } */
/* { dg-final { scan-assembler-not "movlhps" } } */
/* { dg-final { scan-assembler-not "unpckhps" } } */
/* { dg-final { scan-assembler-not "unpcklps" } } */
/* { dg-final { scan-assembler-not "shufps" } } */

typedef float __v4sf __attribute__ ((__vector_size__ (16)));
typedef float __m128 __attribute__ ((__vector_size__ (16), __may_alias__));

static __m128
vector_init (float f0,float f1, float f2,float f3)
{
  __v4sf y = { f0, f1, f2, f3 };
   return (__m128) y;
}

__m128
foo2 (__m128 x)
{
  return vector_init (11.4, ((__v4sf) x)[1], ((__v4sf) x)[2],
		      ((__v4sf) x) [3]);
}

extern void abort ();

void
do_check (__v4sf y, float f[4], float z)
{
  int i;

  for (i = 0; i < 4; i++)
    if (i == 0)
      {
	if (y[i] != z)
	  abort ();
      }
    else
      {
	if (y[i] != f[i])
	  abort ();
      }
}

int
main (void)
{
  float f[4] = { -11, 2, 55553, -4 };
  float z = 11.4;
  __m128 x = (__m128) (__v4sf) { f[0], f[1], f[2], f[3] };
  __m128 y;
  int i;

  for (i = 0; i < 4; i++)
    if (x[i] != f[i])
      abort ();

  y = foo1 (x);
  do_check (y, f, z);
  y = foo2 (x);
  do_check (y, f, z);

  return 0;
}
