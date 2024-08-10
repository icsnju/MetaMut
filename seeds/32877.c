/* { dg-do run { target { riscv_vector } } } */
/* { dg-options "--param riscv-autovec-preference=fixed-vlmax -O3" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv64gcv -mabi=lp64d" } */

#include <stdint-gcc.h>

typedef int64_t vnx8di __attribute__ ((vector_size (64)));
typedef int64_t vnx16di __attribute__ ((vector_size (128)));

__attribute__ ((noipa)) void
f_vnx8di (int64_t a, int64_t b, int64_t *out)
{
  vnx8di v = {a, b, a, b, a, b, a, b};
  *(vnx8di *) out = v;
}

__attribute__ ((noipa)) void
f_vnx16di (int64_t a, int64_t b, int64_t *out)
{
  vnx16di v = {a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b};
  *(vnx16di *) out = v;
}


/* { dg-final { scan-assembler-times {vmv\.s\.x\tv[0-9]+,\s*[a-x0-9]+} 1 } } */
/* { dg-final { scan-assembler-times {vmerge\.vxm\tv[0-9]+,\s*v[0-9]+,\s*[a-x0-9]+,\s*v0} 1 } } */

int
main ()
{
  int64_t a = -178908923423;
  int64_t b = -891615645644;

  int64_t v_vnx8di[sizeof (vnx8di) / sizeof (int64_t)];
  f_vnx8di (a, b, v_vnx8di);
  for (int i = 0; i < sizeof (vnx8di) / sizeof (int64_t); i++)
    {
      if (i % 2 == 0)
	{
	  if (v_vnx8di[i] != a)
	    __builtin_abort ();
	}
      else
	{
	  if (v_vnx8di[i] != b)
	    __builtin_abort ();
	}
    }

  int64_t v_vnx16di[sizeof (vnx16di) / sizeof (int64_t)];
  f_vnx16di (a, b, v_vnx16di);

  for (int i = 0; i < sizeof (vnx16di) / sizeof (int64_t); i++)
    {
      if (i % 2 == 0)
	{
	  if (v_vnx16di[i] != a)
	    __builtin_abort ();
	}
      else
	{
	  if (v_vnx16di[i] != b)
	    __builtin_abort ();
	}
    }

  return 0;
}
