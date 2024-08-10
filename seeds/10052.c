/* { dg-do compile } */
/* { dg-options "-march=rv64gc_zve32x_zvl128b -mabi=lp64d -Wno-int-conversion -Wno-implicit-function -Wno-incompatible-pointer-types -Wno-implicit-function-declaration -Ofast -ftree-vectorize" } */

/* { dg-do compile } */
/* { dg-options "-march=rv64gcv -mabi=lp64d -Wno-int-conversion -Wno-implicit-function -Wno-incompatible-pointer-types -Wno-implicit-function-declaration -Ofast -ftree-vectorize" } */

int d ();
typedef struct
{
  int b;
} c;
int
e (char *f, long g)
{
  f += g;
  while (g--)
    *--f = d;
}

int
d (c * f)
{
  while (h ())
    switch (f->b)
      case 'Q':
      {
	long a;
	e (&a, sizeof (a));
	i (a);
      }
}

/* { dg-final { scan-assembler-times {vsetivli\s+zero,\s*2,\s*e32,\s*mf2,\s*t[au],\s*m[au]} 1 } }
/* { dg-final { scan-assembler-times {vmv\.x\.s} 2 } } */
/* { dg-final { scan-assembler-times {vslidedown.vi\s+v[0-9]+,\s*v[0-9]+,\s*1} 1 } } */
/* { dg-final { scan-assembler-times {slli\s+[a-x0-9]+,[a-x0-9]+,32} 1 } } */
/* { dg-final { scan-assembler-times {or\s+[a-x0-9]+,[a-x0-9]+,[a-x0-9]+} 1 } } */
