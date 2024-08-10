/* { dg-do run } */
/* { dg-require-effective-target fma } */
/* { dg-options "-O3 -Wno-attributes -mfpmath=sse -mfma" } */

/* Test that the compiler properly optimizes floating point multiply
   and add instructions into FMA3 instructions.  */

#define TYPE float


#ifndef l_fma_1
#define l_fma_1

void __attribute__((sseregparm))
test_noneg_add_noneg_add (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = ((a[i] * b[i]) + c[i]) * a[i] + b[i];
}

void __attribute__((sseregparm))
test_noneg_add_noneg_sub (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = ((a[i] * b[i]) + c[i]) * a[i] - b[i];
}

void __attribute__((sseregparm))
test_noneg_add_neg_add (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = -((a[i] * b[i]) + c[i]) * a[i] + b[i];
}

void __attribute__((sseregparm))
test_noneg_add_neg_sub (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = -((a[i] * b[i]) + c[i]) * a[i] - b[i];
}

void __attribute__((sseregparm))
test_noneg_sub_noneg_add (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = ((a[i] * b[i]) - c[i]) * a[i] + b[i];
}

void __attribute__((sseregparm))
test_noneg_sub_noneg_sub (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = ((a[i] * b[i]) - c[i]) * a[i] - b[i];
}

void __attribute__((sseregparm))
test_noneg_sub_neg_add (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = -((a[i] * b[i]) - c[i]) * a[i] + b[i];
}

void __attribute__((sseregparm))
test_noneg_sub_neg_sub (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = -((a[i] * b[i]) - c[i]) * a[i] - b[i];
}

void __attribute__((sseregparm))
test_neg_add_noneg_add (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = (-(a[i] * b[i]) + c[i]) * a[i] + b[i];
}

void __attribute__((sseregparm))
test_neg_add_noneg_sub (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = (-(a[i] * b[i]) + c[i]) * a[i] - b[i];
}

void __attribute__((sseregparm))
test_neg_add_neg_add (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = -(-(a[i] * b[i]) + c[i]) * a[i] + b[i];
}

void __attribute__((sseregparm))
test_neg_add_neg_sub (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = -(-(a[i] * b[i]) + c[i]) * a[i] - b[i];
}

void __attribute__((sseregparm))
test_neg_sub_noneg_add (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = (-(a[i] * b[i]) - c[i]) * a[i] + b[i];
}

void __attribute__((sseregparm))
test_neg_sub_noneg_sub (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = (-(a[i] * b[i]) - c[i]) * a[i] - b[i];
}

void __attribute__((sseregparm))
test_neg_sub_neg_add (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = -(-(a[i] * b[i]) - c[i]) * a[i] + b[i];
}

void __attribute__((sseregparm))
test_neg_sub_neg_sub (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = -(-(a[i] * b[i]) - c[i]) * a[i] - b[i];
}

#endif


#ifndef fma_run_float_results_1
#define fma_run_float_results_1

TYPE res_test0000[32] = {
  7, 23, 55, 109, 191, 307, 463, 665, 919, 1231, 1607, 2053, 2575, 3179, 3871, 4657, 5543, 6535, 7639, 8861, 10207, 11683, 13295, 15049, 16951, 19007, 21223, 23605, 26159, 28891, 31807, 34913
};
TYPE res_test0001[32] = {
  3, 17, 47, 99, 179, 293, 447, 647, 899, 1209, 1583, 2027, 2547, 3149, 3839, 4623, 5507, 6497, 7599, 8819, 10163, 11637, 13247, 14999, 16899, 18953, 21167, 23547, 26099, 28829, 31743, 34847
};
TYPE res_test0010[32] = {
  -3, -17, -47, -99, -179, -293, -447, -647, -899, -1209, -1583, -2027, -2547, -3149, -3839, -4623, -5507, -6497, -7599, -8819, -10163, -11637, -13247, -14999, -16899, -18953, -21167, -23547, -26099, -28829, -31743, -34847
};
TYPE res_test0011[32] = {
  -7, -23, -55, -109, -191, -307, -463, -665, -919, -1231, -1607, -2053, -2575, -3179, -3871, -4657, -5543, -6535, -7639, -8861, -10207, -11683, -13295, -15049, -16951, -19007, -21223, -23605, -26159, -28891, -31807, -34913
};
TYPE res_test0100[32] = {
  1, 7, 25, 61, 121, 211, 337, 505, 721, 991, 1321, 1717, 2185, 2731, 3361, 4081, 4897, 5815, 6841, 7981, 9241, 10627, 12145, 13801, 15601, 17551, 19657, 21925, 24361, 26971, 29761, 32737
};
TYPE res_test0101[32] = {
  -3, 1, 17, 51, 109, 197, 321, 487, 701, 969, 1297, 1691, 2157, 2701, 3329, 4047, 4861, 5777, 6801, 7939, 9197, 10581, 12097, 13751, 15549, 17497, 19601, 21867, 24301, 26909, 29697, 32671
};
TYPE res_test0110[32] = {
  3, -1, -17, -51, -109, -197, -321, -487, -701, -969, -1297, -1691, -2157, -2701, -3329, -4047, -4861, -5777, -6801, -7939, -9197, -10581, -12097, -13751, -15549, -17497, -19601, -21867, -24301, -26909, -29697, -32671
};
TYPE res_test0111[32] = {
  -1, -7, -25, -61, -121, -211, -337, -505, -721, -991, -1321, -1717, -2185, -2731, -3361, -4081, -4897, -5815, -6841, -7981, -9241, -10627, -12145, -13801, -15601, -17551, -19657, -21925, -24361, -26971, -29761, -32737
};
TYPE res_test1000[32] = {
  3, -1, -17, -51, -109, -197, -321, -487, -701, -969, -1297, -1691, -2157, -2701, -3329, -4047, -4861, -5777, -6801, -7939, -9197, -10581, -12097, -13751, -15549, -17497, -19601, -21867, -24301, -26909, -29697, -32671
};
TYPE res_test1001[32] = {
  -1, -7, -25, -61, -121, -211, -337, -505, -721, -991, -1321, -1717, -2185, -2731, -3361, -4081, -4897, -5815, -6841, -7981, -9241, -10627, -12145, -13801, -15601, -17551, -19657, -21925, -24361, -26971, -29761, -32737
};
TYPE res_test1010[32] = {
  1, 7, 25, 61, 121, 211, 337, 505, 721, 991, 1321, 1717, 2185, 2731, 3361, 4081, 4897, 5815, 6841, 7981, 9241, 10627, 12145, 13801, 15601, 17551, 19657, 21925, 24361, 26971, 29761, 32737
};
TYPE res_test1011[32] = {
  -3, 1, 17, 51, 109, 197, 321, 487, 701, 969, 1297, 1691, 2157, 2701, 3329, 4047, 4861, 5777, 6801, 7939, 9197, 10581, 12097, 13751, 15549, 17497, 19601, 21867, 24301, 26909, 29697, 32671
};
TYPE res_test1100[32] = {
  -3, -17, -47, -99, -179, -293, -447, -647, -899, -1209, -1583, -2027, -2547, -3149, -3839, -4623, -5507, -6497, -7599, -8819, -10163, -11637, -13247, -14999, -16899, -18953, -21167, -23547, -26099, -28829, -31743, -34847
};
TYPE res_test1101[32] = {
  -7, -23, -55, -109, -191, -307, -463, -665, -919, -1231, -1607, -2053, -2575, -3179, -3871, -4657, -5543, -6535, -7639, -8861, -10207, -11683, -13295, -15049, -16951, -19007, -21223, -23605, -26159, -28891, -31807, -34913
};
TYPE res_test1110[32] = {
  7, 23, 55, 109, 191, 307, 463, 665, 919, 1231, 1607, 2053, 2575, 3179, 3871, 4657, 5543, 6535, 7639, 8861, 10207, 11683, 13295, 15049, 16951, 19007, 21223, 23605, 26159, 28891, 31807, 34913
};
TYPE res_test1111[32] = {
  3, 17, 47, 99, 179, 293, 447, 647, 899, 1209, 1583, 2027, 2547, 3149, 3839, 4623, 5507, 6497, 7599, 8819, 10163, 11637, 13247, 14999, 16899, 18953, 21167, 23547, 26099, 28829, 31743, 34847
};

#endif

#include <stdlib.h>

#include "cpuid.h"

static void fma_test (void);

static void __attribute__ ((noinline)) do_test (void)
{
  fma_test ();
}

int
main ()
{
  unsigned int eax, ebx, ecx, edx;

  if (!__get_cpuid (1, &eax, &ebx, &ecx, &edx))
    return 0;

  /* Run FMA test only if host has FMA support.  */
  if (ecx & bit_FMA)
    do_test ();

  return 0;
}

#ifndef l_fma_main
#define l_fma_main

#if DEBUG
#include <stdio.h>
#endif

TYPE m1[32] = {
		1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16,
	       17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
	      };
TYPE m2[32] = {
		2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17,
	       18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33
	      };
TYPE m3[32] = {
		3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 
	       19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34
	      };
TYPE m4[32];
int test_fails = 0;

void
compare_result(char *title, TYPE *res)
{
  int i;
  int good = 1;
  for (i =0; i < 32; i++)
    if (m4[i] != res[i])
      {
        if (good)
          {
#if DEBUG
             printf ("!!!! %s miscompare\n", title);
#endif
             good = 0;
          }
#if DEBUG
        printf ("res[%d] = %d, must be %d\n", i, (int)res[i], (int) m4[i]);
#endif
      }
  if (!good)
    test_fails = 1;
}

static void fma_test ()
{
  test_noneg_add_noneg_add (m1, m2, m3, m4, 32);
  compare_result ("test0000", res_test0000);

  test_noneg_add_noneg_sub (m1, m2, m3, m4, 32);
  compare_result ("test0001", res_test0001);

  test_noneg_add_neg_add (m1, m2, m3, m4, 32);
  compare_result ("test0010", res_test0010);

  test_noneg_add_neg_sub (m1, m2, m3, m4, 32);
  compare_result ("test0011", res_test0011);

  test_noneg_sub_noneg_add (m1, m2, m3, m4, 32);
  compare_result ("test0100", res_test0100);

  test_noneg_sub_noneg_sub (m1, m2, m3, m4, 32);
  compare_result ("test0101", res_test0101);

  test_noneg_sub_neg_add (m1, m2, m3, m4, 32);
  compare_result ("test0110", res_test0110);

  test_noneg_sub_neg_sub (m1, m2, m3, m4, 32);
  compare_result ("test0111", res_test0111);

  test_neg_add_noneg_add (m1, m2, m3, m4, 32);
  compare_result ("test1000", res_test1000);

  test_neg_add_noneg_sub (m1, m2, m3, m4, 32);
  compare_result ("test1001", res_test1001);

  test_neg_add_neg_add (m1, m2, m3, m4, 32);
  compare_result ("test1010", res_test1010);

  test_neg_add_neg_sub (m1, m2, m3, m4, 32);
  compare_result ("test1011", res_test1011);

  test_neg_sub_noneg_add (m1, m2, m3, m4, 32);
  compare_result ("test1100", res_test1100);

  test_neg_sub_noneg_sub (m1, m2, m3, m4, 32);
  compare_result ("test1101", res_test1101);

  test_neg_sub_neg_add (m1, m2, m3, m4, 32);
  compare_result ("test1110", res_test1110);

  test_neg_sub_neg_sub (m1, m2, m3, m4, 32);
  compare_result ("test1111", res_test1111);

  if (test_fails) abort ();
}

#endif
