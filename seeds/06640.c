#if __SIZEOF_INT__ == 4
typedef unsigned int V __attribute__((vector_size(32)));
typedef V VI;
#elif __SIZEOF_LONG__ == 4
typedef unsigned long V __attribute__((vector_size(32)));
typedef V VI;
#else
# define UNSUPPORTED
#endif

#define A1	0x11121314
#define B1	0x21222324
#define C1	0x31323334
#define D1	0x41424344
#define E1	0x51525354
#define F1	0x61626364
#define G1	0x71727374
#define H1	0x81828384

#define A2	0x91929394
#define B2	0xa1a2a3a4
#define C2	0xb1b2b3b4
#define D2	0xc1c2c3c4
#define E2	0xd1d2d3d4
#define F2	0xe1e2e3e4
#define G2	0xf1f2f3f4
#define H2	0x01020304

/* Test fragment for vectors of 8 elements.  */

#ifndef UNSUPPORTED

struct S
{
  V in;
  VI mask;
  V out;
};

struct S tests[] = {
  {
    { A1, B1, C1, D1, E1, F1, G1, H1 },
    {  0,  1,  2,  3,  4,  5,  6,  7 },
    { A1, B1, C1, D1, E1, F1, G1, H1 },
  },
  {
    { A1, B1, C1, D1, E1, F1, G1, H1 },
    { 0x10, 0x21, 0x32, 0x43, 0x54, 0x65, 0x76, 0x87 },
    { A1, B1, C1, D1, E1, F1, G1, H1 },
  },
  {
    { A1, B1, C1, D1, E1, F1, G1, H1 },
    {  7,  6,  5,  4,  3,  2,  1,  0 },
    { H1, G1, F1, E1, D1, C1, B1, A1 },
  },
  {
    { A1, B1, C1, D1, E1, F1, G1, H1 },
    {  7,  0,  5,  3,  2,  4,  1,  6 },
    { H1, A1, F1, D1, C1, E1, B1, G1 },
  },
  {
    { A1, B1, C1, D1, E1, F1, G1, H1 },
    {  0,  2,  1,  3,  4,  6,  5,  7 },
    { A1, C1, B1, D1, E1, G1, F1, H1 },
  },
  {
    { A2, B2, C2, D2, E2, F2, G2, H2 },
    {  3,  1,  2,  0,  7,  5,  6,  4 },
    { D2, B2, C2, A2, H2, F2, G2, E2 },
  },
  {
    { A2, B2, C2, D2, E2, F2, G2, H2 },
    { 0, 0, 0, 0 },
    { A2, A2, A2, A2, A2, A2, A2, A2 },
  },
  {
    { A2, B2, C2, D2, E2, F2, G2, H2 },
    {  1,  6,  1,  6,  1,  6,  1,  6 }, 
    { B2, G2, B2, G2, B2, G2, B2, G2 },
  }
};

struct S2
{
  V in1, in2;
  VI mask;
  V out;
};

struct S2 tests2[] = {
  {
    { A1, B1, C1, D1, E1, F1, G1, H1 },
    { A2, B2, C2, D2, E2, F2, G2, H2 },
    { 0, 1, 2, 3, 4, 5, 6, 7 },
    { A1, B1, C1, D1, E1, F1, G1, H1 },
  },
  {
    { A1, B1, C1, D1, E1, F1, G1, H1 },
    { A2, B2, C2, D2, E2, F2, G2, H2 },
    {  8,  9, 10, 11, 12, 13, 14, 15 },
    { A2, B2, C2, D2, E2, F2, G2, H2 },
  },
  {
    { A1, B1, C1, D1, E1, F1, G1, H1 },
    { A2, B2, C2, D2, E2, F2, G2, H2 },
    {  0,  8,  1,  9,  2, 10,  3, 11 },
    { A1, A2, B1, B2, C1, C2, D1, D2 },
  },
  {
    { A1, B1, C1, D1, E1, F1, G1, H1 },
    { A2, B2, C2, D2, E2, F2, G2, H2 },
    {  0, 15,  4, 11, 12,  3,  7,  8 },
    { A1, H2, E1, D2, E2, D1, H1, A2 },
  },
  {
    { A1, B1, C1, D1, E1, F1, G1, H1 },
    { A2, B2, C2, D2, E2, F2, G2, H2 },
    {  0,  0,  0,  0,  0,  0,  0,  0 },
    { A1, A1, A1, A1, A1, A1, A1, A1 }, 
  },
  {
    { A1, B1, C1, D1, E1, F1, G1, H1 },
    { A2, B2, C2, D2, E2, F2, G2, H2 },
    { 0x1e, 0x2e, 0x3e, 0x4e, 0x5e, 0x6e, 0x7e, 0x8e },
    { G2, G2, G2, G2, G2, G2, G2, G2 },
  },
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
