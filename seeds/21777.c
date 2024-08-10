/* { dg-do run { target sse2_runtime } } */
/* { dg-options "-O2 -msse2" } */

/* { dg-do compile } */
/* { dg-options "-O2 -msse -mno-sse4" } */
/* { dg-final { scan-assembler "movss" } } */
/* { dg-final { scan-assembler-not "movaps" } } */
/* { dg-final { scan-assembler-not "movlhps" } } */
/* { dg-final { scan-assembler-not "unpcklps" } } */
/* { dg-final { scan-assembler-not "shufps" } } */

typedef float __v4sf __attribute__ ((__vector_size__ (16)));

__attribute__((noinline, noclone))
__v4sf
foo1 (__v4sf x, float f)
{
  __v4sf y = { f, x[1], x[2], x[3] };
  return y;
}
/* { dg-do compile } */
/* { dg-options "-O2 -msse -mno-sse4" } */
/* { dg-final { scan-assembler "movss" } } */
/* { dg-final { scan-assembler-not "movaps" } } */
/* { dg-final { scan-assembler-not "movlhps" } } */
/* { dg-final { scan-assembler-not "unpcklps" } } */
/* { dg-final { scan-assembler-not "shufps" } } */

typedef float __v4sf __attribute__ ((__vector_size__ (16)));

static __v4sf
vector_init (float f0,float f1, float f2,float f3)
{
  __v4sf y = { f0, f1, f2, f3 };
   return y;
}

__attribute__((noinline, noclone))
__v4sf
foo2 (__v4sf x, float f)
{
  return vector_init (f, x[1], x[2], x[3]) ;
}
/* { dg-do compile } */
/* { dg-options "-O2 -msse -mno-sse4" } */
/* { dg-final { scan-assembler "movss" } } */
/* { dg-final { scan-assembler-not "movaps" } } */
/* { dg-final { scan-assembler-not "movlhps" } } */
/* { dg-final { scan-assembler-not "unpcklps" } } */
/* { dg-final { scan-assembler-not "shufps" } } */

typedef float __v4sf __attribute__ ((__vector_size__ (16)));

__attribute__((noinline, noclone))
__v4sf
foo3 (__v4sf x, float f)
{
  __v4sf y = x;
  y[0] = f;
  return y;
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
  float z = 134567;
  __v4sf x = { f[0], f[1], f[2], f[3] };
  __v4sf y;
  int i;

  for (i = 0; i < 4; i++)
    if (x[i] != f[i])
      abort ();

  y = foo1 (x, z);
  do_check (y, f, z);
  y = foo2 (x, z);
  do_check (y, f, z);
  y = foo3 (x, z);
  do_check (y, f, z);

  return 0;
}
