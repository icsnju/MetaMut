/* { dg-do run { target { riscv_vector } } } */
/* { dg-options "--param riscv-autovec-preference=fixed-vlmax -O3" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv64gcv -mabi=lp64d" } */

#include <stdint-gcc.h>

typedef double vnx2df __attribute__ ((vector_size (16)));
typedef double vnx4df __attribute__ ((vector_size (32)));
typedef double vnx8df __attribute__ ((vector_size (64)));
typedef double vnx16df __attribute__ ((vector_size (128)));

__attribute__ ((noipa)) void
f_vnx2df (double a, double b, double *out)
{
  vnx2df v = {a, b};
  *(vnx2df *) out = v;
}

__attribute__ ((noipa)) void
f_vnx4df (double a, double b, double c, double d, double *out)
{
  vnx4df v = {a, b, c, d};
  *(vnx4df *) out = v;
}

__attribute__ ((noipa)) void
f_vnx8df (double a, double b, double c, double d, double e, double f, double g, double h, double *out)
{
  vnx8df v = {a, b, c, d, e, f, g, h};
  *(vnx8df *) out = v;
}

__attribute__ ((noipa)) void
f_vnx16df (double a, double b, double c, double d, double e, double f,
	   double g, double h, double a2, double b2, double c2, double d2,
	   double e2, double f2, double g2, double h2, double *out)
{
  vnx16df v = {a, b, c, d, e, f, g, h, a2, b2, c2, d2, e2, f2, g2, h2};
  *(vnx16df *) out = v;
}

/* { dg-final { scan-assembler-times {vfslide1down\.vf\tv[0-9]+,\s*v[0-9]+,\s*[a-x0-9]+} 26 } } */

int
main ()
{
  double in[16] = {-458.615, -1.551,  -55.2, -4586.15, -4581.5, -55.1,  -2,	  -4.585,
		    -13.4,    -9929.4, -7.89, -512.84,  -73.24,  -33.33, -666.6, -1.1};

  double v_vnx2df[sizeof (vnx2df) / sizeof (double)];
  f_vnx2df (in[0], in[1], v_vnx2df);
  for (int i = 0; i < sizeof (vnx2df) / sizeof (double); i++)
    {
      if (v_vnx2df[i] != in[i])
	__builtin_abort ();
    }

  double v_vnx4df[sizeof (vnx4df) / sizeof (double)];
  f_vnx4df (in[0], in[1], in[2], in[3], v_vnx4df);
  for (int i = 0; i < sizeof (vnx2df) / sizeof (double); i++)
    {
      if (v_vnx2df[i] != in[i])
	__builtin_abort ();
    }

  double v_vnx8df[sizeof (vnx8df) / sizeof (double)];
  f_vnx8df (in[0], in[1], in[2], in[3], in[4], in[5], in[6], in[7], v_vnx8df);
  for (int i = 0; i < sizeof (vnx2df) / sizeof (double); i++)
    {
      if (v_vnx2df[i] != in[i])
	__builtin_abort ();
    }

  double v_vnx16df[sizeof (vnx16df) / sizeof (double)];
  f_vnx16df (in[0], in[1], in[2], in[3], in[4], in[5], in[6], in[7], in[8],
	     in[9], in[10], in[11], in[12], in[13], in[14], in[15], v_vnx16df);
  for (int i = 0; i < sizeof (vnx2df) / sizeof (double); i++)
    {
      if (v_vnx2df[i] != in[i])
	__builtin_abort ();
    }

  return 0;
}
