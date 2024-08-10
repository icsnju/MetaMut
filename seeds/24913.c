/* { dg-do compile } */
/* { dg-require-effective-target vect_complex_add_half } */
/* { dg-require-effective-target float16 } */
/* { dg-additional-options "-fno-tree-loop-vectorize" } */
/* { dg-add-options arm_v8_3a_fp16_complex_neon } */

#define TYPE _Float16
#define N 16
#include <complex.h>

void fma0 (_Complex TYPE a[restrict N], _Complex TYPE b[restrict N],
	   _Complex TYPE c[restrict N])
{
  for (int i=0; i < N; i++)
    c[i] += a[i] * b[i];
}

void fma90snd (_Complex TYPE a[restrict N], _Complex TYPE b[restrict N],
	       _Complex TYPE c[restrict N])
{
  for (int i=0; i < N; i++)
    c[i] += a[i] * (b[i] * I);
}

void fma180snd (_Complex TYPE a[restrict N], _Complex TYPE b[restrict N],
	        _Complex TYPE c[restrict N])
{
  for (int i=0; i < N; i++)
    c[i] += a[i] * (b[i] * I * I);
}

void fma270snd (_Complex TYPE a[restrict N], _Complex TYPE b[restrict N],
	        _Complex TYPE c[restrict N])
{
  for (int i=0; i < N; i++)
    c[i] += a[i] * (b[i] * I * I * I);
}

void fma90fst (_Complex TYPE a[restrict N], _Complex TYPE b[restrict N],
	       _Complex TYPE c[restrict N])
{
  for (int i=0; i < N; i++)
    c[i] += (a[i] * I) * b[i];
}

void fma180fst (_Complex TYPE a[restrict N], _Complex TYPE b[restrict N],
	        _Complex TYPE c[restrict N])
{
  for (int i=0; i < N; i++)
    c[i] += (a[i] * I * I) * b[i];
}

void fma270fst (_Complex TYPE a[restrict N], _Complex TYPE b[restrict N],
	        _Complex TYPE c[restrict N])
{
  for (int i=0; i < N; i++)
    c[i] += (a[i] * I * I * I) * b[i];
}

void fmaconjfst (_Complex TYPE a[restrict N], _Complex TYPE b[restrict N],
		 _Complex TYPE c[restrict N])
{
  for (int i=0; i < N; i++)
    c[i] += ~a[i] * b[i];
}

void fmaconjsnd (_Complex TYPE a[restrict N], _Complex TYPE b[restrict N],
		 _Complex TYPE c[restrict N])
{
  for (int i=0; i < N; i++)
    c[i] += a[i] * ~b[i];
}

void fmaconjboth (_Complex TYPE a[restrict N], _Complex TYPE b[restrict N],
		  _Complex TYPE c[restrict N])
{
  for (int i=0; i < N; i++)
    c[i] += ~a[i] * ~b[i];
}

void fma_elem (_Complex TYPE a[restrict N], _Complex TYPE b,
	       _Complex TYPE c[restrict N])
{
  for (int i=0; i < N; i++)
    c[i] += a[i] * b;
}


void fma_elemconjfst (_Complex TYPE a[restrict N], _Complex TYPE b,
		      _Complex TYPE c[restrict N])
{
  for (int i=0; i < N; i++)
    c[i] += ~a[i] * b;
}

void fma_elemconjsnd (_Complex TYPE a[restrict N], _Complex TYPE b,
		      _Complex TYPE c[restrict N])
{
  for (int i=0; i < N; i++)
    c[i] += a[i] * ~b;
}

void fma_elemconjboth (_Complex TYPE a[restrict N], _Complex TYPE b,
		       _Complex TYPE c[restrict N])
{
  for (int i=0; i < N; i++)
    c[i] += ~a[i] * ~b;
}


/* { dg-final { scan-tree-dump "Found COMPLEX_FMA_CONJ" "slp1" { xfail *-*-* } } } */
/* { dg-final { scan-tree-dump "Found COMPLEX_FMA" "slp1"  { xfail *-*-* } } } */
