/* { dg-do run { target { riscv_vector } } } */
/* { dg-options "--param riscv-autovec-preference=fixed-vlmax -O3" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv64gcv -mabi=lp64d" } */

#include <stdint-gcc.h>

typedef double vnx8df __attribute__ ((vector_size (64)));
typedef double vnx16df __attribute__ ((vector_size (128)));

__attribute__ ((noipa)) void
f_vnx8df (double a, double b, double *out)
{
  vnx8df v = {a, b, a, b, a, b, a, b};
  *(vnx8df *) out = v;
}

__attribute__ ((noipa)) void
f_vnx16df (double a, double b, double *out)
{
  vnx16df v = {a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b};
  *(vnx16df *) out = v;
}

/* { dg-final { scan-assembler-times {vmv\.s\.x\tv[0-9]+,\s*[a-x0-9]+} 1 } } */
/* { dg-final { scan-assembler-times {vfmerge\.vfm\tv[0-9]+,\s*v[0-9]+,\s*[a-x0-9]+,\s*v0} 1 } } */

int
main ()
{
  double a = -1789089.23423;
  double b = -8916156.45644;

  double v_vnx8df[sizeof (vnx8df) / sizeof (double)];
  f_vnx8df (a, b, v_vnx8df);

  for (int i = 0; i < sizeof (vnx8df) / sizeof (double); i++)
    {
      if (i % 2 == 0)
	{
	  if (v_vnx8df[i] != a)
	    __builtin_abort ();
	}
      else
	{
	  if (v_vnx8df[i] != b)
	    __builtin_abort ();
	}
    }

  double v_vnx16df[sizeof (vnx16df) / sizeof (double)];
  f_vnx16df (a, b, v_vnx16df);
  for (int i = 0; i < sizeof (vnx16df) / sizeof (double); i++)
    {
      if (i % 2 == 0)
	{
	  if (v_vnx16df[i] != a)
	    __builtin_abort ();
	}
      else
	{
	  if (v_vnx16df[i] != b)
	    __builtin_abort ();
	}
    }

  return 0;
}
