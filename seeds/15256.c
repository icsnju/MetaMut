/* { dg-do run { target { riscv_vector } } } */
/* { dg-options "--param riscv-autovec-preference=fixed-vlmax -O3" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv -mabi=ilp32d" } */

#include <stdint-gcc.h>

typedef int8_t vnx8qi __attribute__ ((vector_size (8)));
typedef int8_t vnx16qi __attribute__ ((vector_size (16)));
typedef int8_t vnx32qi __attribute__ ((vector_size (32)));
typedef int8_t vnx64qi __attribute__ ((vector_size (64)));
typedef int8_t vnx128qi __attribute__ ((vector_size (128)));

__attribute__ ((noipa)) void
f_vnx8qi (int8_t a, int8_t b, int8_t c, int8_t d, int8_t *out)
{
  vnx8qi v = {a, b, c, d, a, b, c, d};
  *(vnx8qi *) out = v;
}

__attribute__ ((noipa)) void
f_vnx16qi (int8_t a, int8_t b, int8_t c, int8_t d, int8_t *out)
{
  vnx16qi v = {a, b, c, d, a, b, c, d, a, b, c, d, a, b, c, d};
  *(vnx16qi *) out = v;
}

__attribute__ ((noipa)) void
f_vnx32qi (int8_t a, int8_t b, int8_t c, int8_t d, int8_t *out)
{
  vnx32qi v = {a, b, c, d, a, b, c, d, a, b, c, d, a, b, c, d,
	       a, b, c, d, a, b, c, d, a, b, c, d, a, b, c, d};
  *(vnx32qi *) out = v;
}

__attribute__ ((noipa)) void
f_vnx64qi (int8_t a, int8_t b, int8_t c, int8_t d, int8_t *out)
{
  vnx64qi v = {a, b, c, d, a, b, c, d, a, b, c, d, a, b, c, d,
	       a, b, c, d, a, b, c, d, a, b, c, d, a, b, c, d,
	       a, b, c, d, a, b, c, d, a, b, c, d, a, b, c, d,
	       a, b, c, d, a, b, c, d, a, b, c, d, a, b, c, d};
  *(vnx64qi *) out = v;
}

__attribute__ ((noipa)) void
f_vnx128qi (int8_t a, int8_t b, int8_t c, int8_t d, int8_t *out)
{
  vnx128qi v
    = {a, b, c, d, a, b, c, d, a, b, c, d, a, b, c, d,
       a, b, c, d, a, b, c, d, a, b, c, d, a, b, c, d,
       a, b, c, d, a, b, c, d, a, b, c, d, a, b, c, d,
       a, b, c, d, a, b, c, d, a, b, c, d, a, b, c, d,
       a, b, c, d, a, b, c, d, a, b, c, d, a, b, c, d,
       a, b, c, d, a, b, c, d, a, b, c, d, a, b, c, d,
       a, b, c, d, a, b, c, d, a, b, c, d, a, b, c, d,
       a, b, c, d, a, b, c, d, a, b, c, d, a, b, c, d};
  *(vnx128qi *) out = v;
}

/* { dg-final { scan-assembler-times {vmv.v.x\tv[0-9]+,\s*[a-x0-9]+} 5 } } */
/* { dg-final { scan-assembler-times {slli} 15 } } */
/* { dg-final { scan-assembler-times {or} 15 } } */

int
main ()
{
  int8_t a = -17;
  int8_t b = -120;
  int8_t c = 111;
  int8_t d = -11;

  int8_t v_vnx8qi[sizeof (vnx8qi) / sizeof (int8_t)];
  f_vnx8qi (a, b, c, d, v_vnx8qi);
  for (int i = 0; i < sizeof (vnx8qi) / sizeof (int8_t); i++)
    {
      if (i % 4 == 0)
	{
	  if (v_vnx8qi[i] != a)
	    __builtin_abort ();
	}
      else if (i % 4 == 1)
	{
	  if (v_vnx8qi[i] != b)
	    __builtin_abort ();
	}
      else if (i % 4 == 2)
	{
	  if (v_vnx8qi[i] != c)
	    __builtin_abort ();
	}
      else
	{
	  if (v_vnx8qi[i] != d)
	    __builtin_abort ();
	}
    }

  int8_t v_vnx16qi[sizeof (vnx16qi) / sizeof (int8_t)];
  f_vnx16qi (a, b, c, d, v_vnx16qi);
  for (int i = 0; i < sizeof (vnx16qi) / sizeof (int8_t); i++)
    {
      if (i % 4 == 0)
	{
	  if (v_vnx16qi[i] != a)
	    __builtin_abort ();
	}
      else if (i % 4 == 1)
	{
	  if (v_vnx16qi[i] != b)
	    __builtin_abort ();
	}
      else if (i % 4 == 2)
	{
	  if (v_vnx16qi[i] != c)
	    __builtin_abort ();
	}
      else
	{
	  if (v_vnx16qi[i] != d)
	    __builtin_abort ();
	}
    }

  int8_t v_vnx32qi[sizeof (vnx32qi) / sizeof (int8_t)];
  f_vnx32qi (a, b, c, d, v_vnx32qi);
  for (int i = 0; i < sizeof (vnx32qi) / sizeof (int8_t); i++)
    {
      if (i % 4 == 0)
	{
	  if (v_vnx32qi[i] != a)
	    __builtin_abort ();
	}
      else if (i % 4 == 1)
	{
	  if (v_vnx32qi[i] != b)
	    __builtin_abort ();
	}
      else if (i % 4 == 2)
	{
	  if (v_vnx32qi[i] != c)
	    __builtin_abort ();
	}
      else
	{
	  if (v_vnx32qi[i] != d)
	    __builtin_abort ();
	}
    }

  int8_t v_vnx64qi[sizeof (vnx64qi) / sizeof (int8_t)];
  f_vnx64qi (a, b, c, d, v_vnx64qi);
  for (int i = 0; i < sizeof (vnx64qi) / sizeof (int8_t); i++)
    {
      if (i % 4 == 0)
	{
	  if (v_vnx64qi[i] != a)
	    __builtin_abort ();
	}
      else if (i % 4 == 1)
	{
	  if (v_vnx64qi[i] != b)
	    __builtin_abort ();
	}
      else if (i % 4 == 2)
	{
	  if (v_vnx64qi[i] != c)
	    __builtin_abort ();
	}
      else
	{
	  if (v_vnx64qi[i] != d)
	    __builtin_abort ();
	}
    }

  int8_t v_vnx128qi[sizeof (vnx128qi) / sizeof (int8_t)];
  f_vnx128qi (a, b, c, d, v_vnx128qi);
  for (int i = 0; i < sizeof (vnx128qi) / sizeof (int8_t); i++)
    {
      if (i % 4 == 0)
	{
	  if (v_vnx128qi[i] != a)
	    __builtin_abort ();
	}
      else if (i % 4 == 1)
	{
	  if (v_vnx128qi[i] != b)
	    __builtin_abort ();
	}
      else if (i % 4 == 2)
	{
	  if (v_vnx128qi[i] != c)
	    __builtin_abort ();
	}
      else
	{
	  if (v_vnx128qi[i] != d)
	    __builtin_abort ();
	}
    }

  return 0;
}