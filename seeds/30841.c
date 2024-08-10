/* { dg-do run { target { riscv_vector } } } */
/* { dg-options "--param riscv-autovec-preference=fixed-vlmax -O3" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv -mabi=ilp32d" } */

#include <stdint-gcc.h>

typedef int8_t vnx2qi __attribute__ ((vector_size (2)));
typedef int8_t vnx4qi __attribute__ ((vector_size (4)));
typedef int8_t vnx8qi __attribute__ ((vector_size (8)));
typedef int8_t vnx16qi __attribute__ ((vector_size (16)));
typedef int8_t vnx32qi __attribute__ ((vector_size (32)));
typedef int8_t vnx64qi __attribute__ ((vector_size (64)));
typedef int8_t vnx128qi __attribute__ ((vector_size (128)));

__attribute__ ((noipa)) void
f_vnx2qi (int8_t a, int8_t b, int8_t *out)
{
  vnx2qi v = {a, b};
  *(vnx2qi *) out = v;
}

__attribute__ ((noipa)) void
f_vnx4qi (int8_t a, int8_t b, int8_t *out)
{
  vnx4qi v = {a, b, a, b};
  *(vnx4qi *) out = v;
}

__attribute__ ((noipa)) void
f_vnx8qi (int8_t a, int8_t b, int8_t *out)
{
  vnx8qi v = {a, b, a, b, a, b, a, b};
  *(vnx8qi *) out = v;
}

__attribute__ ((noipa)) void
f_vnx16qi (int8_t a, int8_t b, int8_t *out)
{
  vnx16qi v = {a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b};
  *(vnx16qi *) out = v;
}

__attribute__ ((noipa)) void
f_vnx32qi (int8_t a, int8_t b, int8_t *out)
{
  vnx32qi v = {a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b,
	       a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b};
  *(vnx32qi *) out = v;
}

__attribute__ ((noipa)) void
f_vnx64qi (int8_t a, int8_t b, int8_t *out)
{
  vnx64qi v = {a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b,
	       a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b,
	       a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b};
  *(vnx64qi *) out = v;
}

__attribute__ ((noipa)) void
f_vnx128qi (int8_t a, int8_t b, int8_t *out)
{
  vnx128qi v
    = {a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b,
       a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b,
       a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b,
       a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b,
       a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b,
       a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b};
  *(vnx128qi *) out = v;
}

/* { dg-final { scan-assembler-times {vmv.v.x\tv[0-9]+,\s*[a-x0-9]+} 7 } } */
/* { dg-final { scan-assembler-times {slli\t[a-x0-9]+,\s*[a-x0-9]+,\s*8} 6 } } */
/* { dg-final { scan-assembler-times {or\t[a-x0-9]+,\s*[a-x0-9]+,\s*[a-x0-9]+} 6 } } */
/* { dg-final { scan-assembler-times {vslide1down\.vx\tv[0-9]+,\s*v[0-9]+,\s*[a-x0-9]+} 1 } } */

int
main ()
{
  int8_t a = -17;
  int8_t b = 89;

  int8_t v_vnx2qi[sizeof (vnx2qi) / sizeof (int8_t)];
  f_vnx2qi (a, b, v_vnx2qi);
  for (int i = 0; i < sizeof (vnx2qi) / sizeof (int8_t); i++)
    {
      if (i % 2 == 0)
	{
	  if (v_vnx2qi[i] != a)
	    __builtin_abort ();
	}
      else
	{
	  if (v_vnx2qi[i] != b)
	    __builtin_abort ();
	}
    }

  int8_t v_vnx4qi[sizeof (vnx4qi) / sizeof (int8_t)];
  f_vnx4qi (a, b, v_vnx4qi);
  for (int i = 0; i < sizeof (vnx4qi) / sizeof (int8_t); i++)
    {
      if (i % 2 == 0)
	{
	  if (v_vnx4qi[i] != a)
	    __builtin_abort ();
	}
      else
	{
	  if (v_vnx4qi[i] != b)
	    __builtin_abort ();
	}
    }

  int8_t v_vnx8qi[sizeof (vnx8qi) / sizeof (int8_t)];
  f_vnx8qi (a, b, v_vnx8qi);
  for (int i = 0; i < sizeof (vnx8qi) / sizeof (int8_t); i++)
    {
      if (i % 2 == 0)
	{
	  if (v_vnx8qi[i] != a)
	    __builtin_abort ();
	}
      else
	{
	  if (v_vnx8qi[i] != b)
	    __builtin_abort ();
	}
    }

  int8_t v_vnx16qi[sizeof (vnx16qi) / sizeof (int8_t)];
  f_vnx16qi (a, b, v_vnx16qi);
  for (int i = 0; i < sizeof (vnx16qi) / sizeof (int8_t); i++)
    {
      if (i % 2 == 0)
	{
	  if (v_vnx16qi[i] != a)
	    __builtin_abort ();
	}
      else
	{
	  if (v_vnx16qi[i] != b)
	    __builtin_abort ();
	}
    }

  int8_t v_vnx32qi[sizeof (vnx32qi) / sizeof (int8_t)];
  f_vnx32qi (a, b, v_vnx32qi);
  for (int i = 0; i < sizeof (vnx32qi) / sizeof (int8_t); i++)
    {
      if (i % 2 == 0)
	{
	  if (v_vnx32qi[i] != a)
	    __builtin_abort ();
	}
      else
	{
	  if (v_vnx32qi[i] != b)
	    __builtin_abort ();
	}
    }

  int8_t v_vnx64qi[sizeof (vnx64qi) / sizeof (int8_t)];
  f_vnx64qi (a, b, v_vnx64qi);
  for (int i = 0; i < sizeof (vnx64qi) / sizeof (int8_t); i++)
    {
      if (i % 2 == 0)
	{
	  if (v_vnx64qi[i] != a)
	    __builtin_abort ();
	}
      else
	{
	  if (v_vnx64qi[i] != b)
	    __builtin_abort ();
	}
    }

  int8_t v_vnx128qi[sizeof (vnx128qi) / sizeof (int8_t)];
  f_vnx128qi (a, b, v_vnx128qi);
  for (int i = 0; i < sizeof (vnx128qi) / sizeof (int8_t); i++)
    {
      if (i % 2 == 0)
	{
	  if (v_vnx128qi[i] != a)
	    __builtin_abort ();
	}
      else
	{
	  if (v_vnx128qi[i] != b)
	    __builtin_abort ();
	}
    }

  return 0;
}
