/* { dg-do run { target { riscv_vector } } } */
/* { dg-options "--param riscv-autovec-preference=fixed-vlmax -O3" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv64gcv -mabi=lp64d" } */

#include <stdint-gcc.h>

typedef int64_t vnx16di __attribute__ ((vector_size (128)));

__attribute__ ((noipa)) void
f_vnx16di (int64_t a, int64_t b, int64_t c, int64_t d, int64_t *out)
{
  vnx16di v = {a, b, c, d, a, b, c, d, a, b, c, d, a, b, c, d,};
  *(vnx16di *) out = v;
}

/* { dg-final { scan-assembler-times {vmv\.v\.x\tv[0-9]+,\s*[a-x0-9]+} 1 } } */
/* { dg-final { scan-assembler-times {vmv\.s\.x\tv[0-9]+,\s*[a-x0-9]+} 0 } } */
/* { dg-final { scan-assembler-times {vmerge\.vxm\tv[0-9]+,\s*v[0-9]+,\s*[a-x0-9]+,\s*v0} 0 } } */

int
main ()
{
  int64_t a = -178908923423;
  int64_t b = -891615645644;
  int64_t c = 78908923423;
  int64_t d = 81615645644;

  int64_t v_vnx16di[sizeof (vnx16di) / sizeof (int64_t)];
  f_vnx16di (a, b, c, d, v_vnx16di);
  for (int i = 0; i < sizeof (vnx16di) / sizeof (int64_t); i++)
    {
      if (i % 4 == 0)
	{
	  if (v_vnx16di[i] != a)
	    __builtin_abort ();
	}
      else if (i % 4 == 1)
	{
	  if (v_vnx16di[i] != b)
	    __builtin_abort ();
	}
      else if (i % 4 == 2)
	{
	  if (v_vnx16di[i] != c)
	    __builtin_abort ();
	}
      else
	{
	  if (v_vnx16di[i] != d)
	    __builtin_abort ();
	}
    }

  return 0;
}
