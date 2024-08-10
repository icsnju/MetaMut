/* { dg-do compile } */
/* { dg-require-effective-target vect_complex_add_half } */
/* { dg-require-effective-target float16 } */
/* { dg-additional-options "-fno-tree-loop-vectorize" } */
/* { dg-add-options arm_v8_3a_fp16_complex_neon } */

#define TYPE _Float16
#define N 16
#include <complex.h>

void mul0 (_Complex TYPE a[restrict N], _Complex TYPE b[restrict N],
	   _Complex TYPE c[restrict N])
{
  for (int i=0; i < N; i++)
    c[i] = a[i] * b[i];
}

void mul90snd (_Complex TYPE a[restrict N], _Complex TYPE b[restrict N],
	       _Complex TYPE c[restrict N])
{
  for (int i=0; i < N; i++)
    c[i] = a[i] * (b[i] * I);
}

void mul180snd (_Complex TYPE a[restrict N], _Complex TYPE b[restrict N],
	        _Complex TYPE c[restrict N])
{
  for (int i=0; i < N; i++)
    c[i] = a[i] * (b[i] * I * I);
}

void mul270snd (_Complex TYPE a[restrict N], _Complex TYPE b[restrict N],
	        _Complex TYPE c[restrict N])
{
  for (int i=0; i < N; i++)
    c[i] = a[i] * (b[i] * I * I * I);
}

void mul90fst (_Complex TYPE a[restrict N], _Complex TYPE b[restrict N],
	       _Complex TYPE c[restrict N])
{
  for (int i=0; i < N; i++)
    c[i] = (a[i] * I) * b[i];
}

void mul180fst (_Complex TYPE a[restrict N], _Complex TYPE b[restrict N],
	        _Complex TYPE c[restrict N])
{
  for (int i=0; i < N; i++)
    c[i] = (a[i] * I * I) * b[i];
}

void mul270fst (_Complex TYPE a[restrict N], _Complex TYPE b[restrict N],
	        _Complex TYPE c[restrict N])
{
  for (int i=0; i < N; i++)
    c[i] = (a[i] * I * I * I) * b[i];
}

void mulconjfst (_Complex TYPE a[restrict N], _Complex TYPE b[restrict N],
		 _Complex TYPE c[restrict N])
{
  for (int i=0; i < N; i++)
    c[i] = ~a[i] * b[i];
}

void mulconjsnd (_Complex TYPE a[restrict N], _Complex TYPE b[restrict N],
		 _Complex TYPE c[restrict N])
{
  for (int i=0; i < N; i++)
    c[i] = a[i] * ~b[i];
}

void mulconjboth (_Complex TYPE a[restrict N], _Complex TYPE b[restrict N],
		  _Complex TYPE c[restrict N])
{
  for (int i=0; i < N; i++)
    c[i] = ~a[i] * ~b[i];
}

/* { dg-final { scan-tree-dump "Found COMPLEX_MUL_CONJ" "slp1"  { xfail *-*-* } } } */
/* { dg-final { scan-tree-dump "Found COMPLEX_MUL" "slp1"  { xfail *-*-* } } } */
