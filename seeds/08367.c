#if __SIZEOF_INT__ == 4
typedef unsigned int V __attribute__((vector_size(16)));
typedef V VI;
#elif __SIZEOF_LONG__ == 4
typedef unsigned long V __attribute__((vector_size(16)));
typedef V VI;
#else
# define UNSUPPORTED
#endif

#define A	0x11121314
#define B	0x21222324
#define C	0x31323334
#define D	0x41424344

#define W	0xc1c2c3c4
#define X	0xd1d2d3d4
#define Y	0xe1e2e3e4
#define Z	0xf1f2f3f4

/* Test fragment for vectors of 4 elements.  */

#ifndef UNSUPPORTED

struct S
{
  V in;
  VI mask;
  V out;
};

struct S tests[] = {
  { { A, B, C, D }, { 0, 1, 2, 3 }, { A, B, C, D }, },
  { { A, B, C, D }, { 0+1*4, 1+2*4, 2+3*4, 3+4*4 }, { A, B, C, D } },
  { { A, B, C, D }, { 3, 2, 1, 0 }, { D, C, B, A } },
  { { A, B, C, D }, { 0, 3, 2, 1 }, { A, D, C, B } },
  { { A, B, C, D }, { 0, 2, 1, 3 }, { A, C, B, D } },
  { { W, X, Y, Z }, { 3, 1, 2, 0 }, { Z, X, Y, W } },
  { { W, X, Y, Z }, { 0, 0, 0, 0 }, { W, W, W, W } },
  { { W, X, Y, Z }, { 1, 2, 1, 2 }, { X, Y, X, Y } },
};

struct S2
{
  V in1, in2;
  VI mask;
  V out;
};

struct S2 tests2[] = {
  { { A, B, C, D }, { W, X, Y, Z }, { 0, 1, 2, 3 }, { A, B, C, D } },
  { { A, B, C, D }, { W, X, Y, Z }, { 4, 5, 6, 7 }, { W, X, Y, Z } },
  { { A, B, C, D }, { W, X, Y, Z }, { 0, 4, 1, 5 }, { A, W, B, X } },
  { { A, B, C, D }, { W, X, Y, Z }, { 0, 7, 4, 3 }, { A, Z, W, D } },
  { { A, B, C, D }, { W, X, Y, Z }, { 0, 0, 0, 0 }, { A, A, A, A } },
  { { A, B, C, D }, { W, X, Y, Z }, { 7, 7, 7, 7 }, { Z, Z, Z, Z } },
};

#endif
/* Driver fragment for __builtin_shuffle of any vector shape.  */

extern void abort(void);

int main()
{
#ifndef UNSUPPORTED
  int i;

  for (i = 0; i < sizeof(tests)/sizeof(tests[0]); ++i)
    {
      V r = __builtin_shuffle(tests[i].in, tests[i].mask);
      if (__builtin_memcmp(&r, &tests[i].out, sizeof(V)) != 0)
	abort();
    }

  for (i = 0; i < sizeof(tests2)/sizeof(tests2[0]); ++i)
    {
      V r = __builtin_shuffle(tests2[i].in1, tests2[i].in2, tests2[i].mask);
      if (__builtin_memcmp(&r, &tests2[i].out, sizeof(V)) != 0)
	abort();
    }
#endif

  return 0;
}
