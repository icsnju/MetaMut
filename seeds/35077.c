/* { dg-do run { target { riscv_vector } } } */
/* { dg-options "--param riscv-autovec-preference=fixed-vlmax -O3" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv64gcv -mabi=lp64d" } */

#include <stdint-gcc.h>

typedef int64_t vnx2di __attribute__ ((vector_size (16)));
typedef int64_t vnx4di __attribute__ ((vector_size (32)));
typedef int64_t vnx8di __attribute__ ((vector_size (64)));
typedef int64_t vnx16di __attribute__ ((vector_size (128)));

__attribute__ ((noipa)) void
f_vnx2di (int64_t a, int64_t b, int64_t *out)
{
  vnx2di v = {a, b};
  *(vnx2di *) out = v;
}

__attribute__ ((noipa)) void
f_vnx4di (int64_t a, int64_t b, int64_t c, int64_t d, int64_t *out)
{
  vnx4di v = {a, b, c, d};
  *(vnx4di *) out = v;
}

__attribute__ ((noipa)) void
f_vnx8di (int64_t a, int64_t b, int64_t c, int64_t d, int64_t e, int64_t f, int64_t g, int64_t h, int64_t *out)
{
  vnx8di v = {a, b, c, d, e, f, g, h};
  *(vnx8di *) out = v;
}

__attribute__ ((noipa)) void
f_vnx16di (int64_t a, int64_t b, int64_t c, int64_t d, int64_t e, int64_t f,
	   int64_t g, int64_t h, int64_t a2, int64_t b2, int64_t c2, int64_t d2,
	   int64_t e2, int64_t f2, int64_t g2, int64_t h2, int64_t *out)
{
  vnx16di v = {a, b, c, d, e, f, g, h, a2, b2, c2, d2, e2, f2, g2, h2};
  *(vnx16di *) out = v;
}

/* { dg-final { scan-assembler-times {vslide1down\.vx\tv[0-9]+,\s*v[0-9]+,\s*[a-x0-9]+} 26 } } */

int
main ()
{
  int64_t in[16] = {-458615, -1551,  -552, -458615, -45815, -551,  -2,	  -4585,
		    -134,    -99294, -789, -51284,  -7324,  -3333, -6666, -11};

  int64_t v_vnx2di[sizeof (vnx2di) / sizeof (int64_t)];
  f_vnx2di (in[0], in[1], v_vnx2di);
  for (int i = 0; i < sizeof (vnx2di) / sizeof (int64_t); i++)
    {
      if (v_vnx2di[i] != in[i])
	__builtin_abort ();
    }

  int64_t v_vnx4di[sizeof (vnx4di) / sizeof (int64_t)];
  f_vnx4di (in[0], in[1], in[2], in[3], v_vnx4di);
  for (int i = 0; i < sizeof (vnx2di) / sizeof (int64_t); i++)
    {
      if (v_vnx2di[i] != in[i])
	__builtin_abort ();
    }

  int64_t v_vnx8di[sizeof (vnx8di) / sizeof (int64_t)];
  f_vnx8di (in[0], in[1], in[2], in[3], in[4], in[5], in[6], in[7], v_vnx8di);
  for (int i = 0; i < sizeof (vnx2di) / sizeof (int64_t); i++)
    {
      if (v_vnx2di[i] != in[i])
	__builtin_abort ();
    }

  int64_t v_vnx16di[sizeof (vnx16di) / sizeof (int64_t)];
  f_vnx16di (in[0], in[1], in[2], in[3], in[4], in[5], in[6], in[7], in[8],
	     in[9], in[10], in[11], in[12], in[13], in[14], in[15], v_vnx16di);
  for (int i = 0; i < sizeof (vnx2di) / sizeof (int64_t); i++)
    {
      if (v_vnx2di[i] != in[i])
	__builtin_abort ();
    }

  return 0;
}
