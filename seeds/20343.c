/* { dg-do run { target { riscv_vector } } } */
/* { dg-options "--param riscv-autovec-preference=fixed-vlmax -O3" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv64gcv -mabi=lp64d" } */

#include <stdint-gcc.h>

typedef float vnx4sf __attribute__ ((vector_size (16)));
typedef float vnx8sf __attribute__ ((vector_size (32)));
typedef float vnx16sf __attribute__ ((vector_size (64)));
typedef float vnx32sf __attribute__ ((vector_size (128)));

__attribute__ ((noipa)) void
f_vnx4sf (float a, float b, float *out)
{
  vnx4sf v = {a, b, a, b};
  *(vnx4sf *) out = v;
}

__attribute__ ((noipa)) void
f_vnx8sf (float a, float b, float *out)
{
  vnx8sf v = {a, b, a, b, a, b, a, b};
  *(vnx8sf *) out = v;
}

__attribute__ ((noipa)) void
f_vnx16sf (float a, float b, float *out)
{
  vnx16sf v = {a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b};
  *(vnx16sf *) out = v;
}

__attribute__ ((noipa)) void
f_vnx32sf (float a, float b, float *out)
{
  vnx32sf v = {a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b};
  *(vnx32sf *) out = v;
}

/* { dg-final { scan-assembler-times {vmv\.v\.x\tv[0-9]+,\s*[a-x0-9]+} 4 } } */

int
main ()
{
  float a = -9523.33;
  float b = 8156.55;

  float v_vnx4sf[sizeof (vnx4sf) / sizeof (float)];
  f_vnx4sf (a, b, v_vnx4sf);
  for (int i = 0; i < sizeof (vnx4sf) / sizeof (float); i++)
    {
      if (i % 2 == 0)
	{
	  if (v_vnx4sf[i] != a)
	    __builtin_abort ();
	}
      else
	{
	  if (v_vnx4sf[i] != b)
	    __builtin_abort ();
	}
    }

  float v_vnx8sf[sizeof (vnx8sf) / sizeof (float)];
  f_vnx8sf (a, b, v_vnx8sf);
  for (int i = 0; i < sizeof (vnx8sf) / sizeof (float); i++)
    {
      if (i % 2 == 0)
	{
	  if (v_vnx8sf[i] != a)
	    __builtin_abort ();
	}
      else
	{
	  if (v_vnx8sf[i] != b)
	    __builtin_abort ();
	}
    }

  float v_vnx16sf[sizeof (vnx16sf) / sizeof (float)];
  f_vnx16sf (a, b, v_vnx16sf);
  for (int i = 0; i < sizeof (vnx16sf) / sizeof (float); i++)
    {
      if (i % 2 == 0)
	{
	  if (v_vnx16sf[i] != a)
	    __builtin_abort ();
	}
      else
	{
	  if (v_vnx16sf[i] != b)
	    __builtin_abort ();
	}
    }

  float v_vnx32sf[sizeof (vnx32sf) / sizeof (float)];
  f_vnx32sf (a, b, v_vnx32sf);
  for (int i = 0; i < sizeof (vnx32sf) / sizeof (float); i++)
    {
      if (i % 2 == 0)
	{
	  if (v_vnx32sf[i] != a)
	    __builtin_abort ();
	}
      else
	{
	  if (v_vnx32sf[i] != b)
	    __builtin_abort ();
	}
    }

  return 0;
}
