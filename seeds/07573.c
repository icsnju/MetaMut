#if __SIZEOF_INT__ == 4
typedef unsigned int V __attribute__((vector_size(8)));
typedef V VI;
#elif __SIZEOF_LONG__ == 4
typedef unsigned long V __attribute__((vector_size(8)));
typedef V VI;
#else
#define UNSUPPORTED
#endif

#define A	0x11121314
#define B	0x21222324

#define X	0xd1d2d3d4
#define Y	0xe1e2e3e4

/* Test fragment for vectors of 2 elements.  */

#ifndef UNSUPPORTED

struct S
{
  V in;
  VI mask;
  V out;
};

struct S tests[] = {
  { { A, B }, { 0, 1 }, { A, B } },
  { { A, B }, { -16U, -1U }, { A, B } },
  { { A, B }, { 1, 0 }, { B, A } },
  { { A, B }, { 0, 0 }, { A, A } },
  { { X, Y }, { 1, 1 }, { Y, Y } },
  { { X, Y }, { 1, 0 }, { Y, X } },
};

struct S2
{
  V in1, in2;
  VI mask;
  V out;
};

struct S2 tests2[] = {
  { { A, B }, { X, Y }, { 0, 1 }, { A, B } },
  { { A, B }, { X, Y }, { 2, 3 }, { X, Y } },
  { { A, B }, { X, Y }, { 0, 2 }, { A, X } },
  { { A, B }, { X, Y }, { 2, 1 }, { X, B } },
  { { A, B }, { X, Y }, { 3, 0 }, { Y, A } },
  { { A, B }, { X, Y }, { 0, 0 }, { A, A } },
  { { A, B }, { X, Y }, { 3, 3 }, { Y, Y } },
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
