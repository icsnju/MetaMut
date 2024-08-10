#if __SIZEOF_DOUBLE__ == 8 && __SIZEOF_LONG_LONG__ == 8
typedef double V __attribute__((vector_size(32)));
typedef unsigned long long VI __attribute__((vector_size(32)));
#else
#define UNSUPPORTED
#endif

#define A	0.69314718055994530942
#define B	2.7182818284590452354
#define C	2.30258509299404568402
#define D	1.4426950408889634074

#define W	0.31830988618379067154
#define X	3.14159265358979323846
#define Y	1.41421356237309504880
#define Z	0.70710678118654752440

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
