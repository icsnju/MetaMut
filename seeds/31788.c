/* { dg-do run } */
/* { dg-require-effective-target fma } */
/* { dg-options "-O3 -Wno-attributes -mfpmath=sse -mfma" } */

/* Test that the compiler properly optimizes floating point multiply
   and add instructions into FMA3 instructions.  */

#define TYPE float


#ifndef fma_3
#define fma_3

TYPE __attribute__((sseregparm))
test_noneg_add_noneg_add (TYPE a, TYPE b, TYPE c)
{
  return ((a * b) + c) * b + a;
}

TYPE __attribute__((sseregparm))
test_noneg_add_noneg_sub (TYPE a, TYPE b, TYPE c)
{
  return ((a * b) + c) * b - a;
}

TYPE __attribute__((sseregparm))
test_noneg_add_neg_add (TYPE a, TYPE b, TYPE c)
{
  return -((a * b) + c) * b + a;
}

TYPE __attribute__((sseregparm))
test_noneg_add_neg_sub (TYPE a, TYPE b, TYPE c)
{
  return -((a * b) + c) * b - a;
}

TYPE __attribute__((sseregparm))
test_noneg_sub_noneg_add (TYPE a, TYPE b, TYPE c)
{
  return ((a * b) - c) * b + a;
}

TYPE __attribute__((sseregparm))
test_noneg_sub_noneg_sub (TYPE a, TYPE b, TYPE c)
{
  return ((a * b) - c) * b - a;
}

TYPE __attribute__((sseregparm))
test_noneg_sub_neg_add (TYPE a, TYPE b, TYPE c)
{
  return -((a * b) - c) * b + a;
}

TYPE __attribute__((sseregparm))
test_noneg_sub_neg_sub (TYPE a, TYPE b, TYPE c)
{
  return -((a * b) - c) * b - a;
}

TYPE __attribute__((sseregparm))
test_neg_add_noneg_add (TYPE a, TYPE b, TYPE c)
{
  return (-(a * b) + c) * b + a;
}

TYPE __attribute__((sseregparm))
test_neg_add_noneg_sub (TYPE a, TYPE b, TYPE c)
{
  return (-(a * b) + c) * b - a;
}

TYPE __attribute__((sseregparm))
test_neg_add_neg_add (TYPE a, TYPE b, TYPE c)
{
  return -(-(a * b) + c) * b + a;
}

TYPE __attribute__((sseregparm))
test_neg_add_neg_sub (TYPE a, TYPE b, TYPE c)
{
  return -(-(a * b) + c) * b - a;
}

TYPE __attribute__((sseregparm))
test_neg_sub_noneg_add (TYPE a, TYPE b, TYPE c)
{
  return (-(a * b) - c) * b + a;
}

TYPE __attribute__((sseregparm))
test_neg_sub_noneg_sub (TYPE a, TYPE b, TYPE c)
{
  return (-(a * b) - c) * b - a;
}

TYPE __attribute__((sseregparm))
test_neg_sub_neg_add (TYPE a, TYPE b, TYPE c)
{
  return -(-(a * b) - c) * b + a;
}

TYPE __attribute__((sseregparm))
test_neg_sub_neg_sub (TYPE a, TYPE b, TYPE c)
{
  return -(-(a * b) - c) * b - a;
}

#endif


#ifndef fma_run_float_results_3
#define fma_run_float_results_3

TYPE res_test0000[32] = {
  11, 32, 71, 134, 227, 356, 527, 746, 1019, 1352, 1751, 2222, 2771, 3404, 4127, 4946, 5867, 6896, 8039, 9302, 10691, 12212, 13871, 15674, 17627, 19736, 22007, 24446, 27059, 29852, 32831, 36002
};
TYPE res_test0001[32] = {
  9, 28, 65, 126, 217, 344, 513, 730, 1001, 1332, 1729, 2198, 2745, 3376, 4097, 4914, 5833, 6860, 8001, 9262, 10649, 12168, 13825, 15626, 17577, 19684, 21953, 24390, 27001, 29792, 32769, 35938
};
TYPE res_test0010[32] = {
  -9, -28, -65, -126, -217, -344, -513, -730, -1001, -1332, -1729, -2198, -2745, -3376, -4097, -4914, -5833, -6860, -8001, -9262, -10649, -12168, -13825, -15626, -17577, -19684, -21953, -24390, -27001, -29792, -32769, -35938
};
TYPE res_test0011[32] = {
  -11, -32, -71, -134, -227, -356, -527, -746, -1019, -1352, -1751, -2222, -2771, -3404, -4127, -4946, -5867, -6896, -8039, -9302, -10691, -12212, -13871, -15674, -17627, -19736, -22007, -24446, -27059, -29852, -32831, -36002
};
TYPE res_test0100[32] = {
  -1, 8, 31, 74, 143, 244, 383, 566, 799, 1088, 1439, 1858, 2351, 2924, 3583, 4334, 5183, 6136, 7199, 8378, 9679, 11108, 12671, 14374, 16223, 18224, 20383, 22706, 25199, 27868, 30719, 33758
};
TYPE res_test0101[32] = {
  -3, 4, 25, 66, 133, 232, 369, 550, 781, 1068, 1417, 1834, 2325, 2896, 3553, 4302, 5149, 6100, 7161, 8338, 9637, 11064, 12625, 14326, 16173, 18172, 20329, 22650, 25141, 27808, 30657, 33694
};
TYPE res_test0110[32] = {
  3, -4, -25, -66, -133, -232, -369, -550, -781, -1068, -1417, -1834, -2325, -2896, -3553, -4302, -5149, -6100, -7161, -8338, -9637, -11064, -12625, -14326, -16173, -18172, -20329, -22650, -25141, -27808, -30657, -33694
};
TYPE res_test0111[32] = {
  1, -8, -31, -74, -143, -244, -383, -566, -799, -1088, -1439, -1858, -2351, -2924, -3583, -4334, -5183, -6136, -7199, -8378, -9679, -11108, -12671, -14374, -16223, -18224, -20383, -22706, -25199, -27868, -30719, -33758
};
TYPE res_test1000[32] = {
  3, -4, -25, -66, -133, -232, -369, -550, -781, -1068, -1417, -1834, -2325, -2896, -3553, -4302, -5149, -6100, -7161, -8338, -9637, -11064, -12625, -14326, -16173, -18172, -20329, -22650, -25141, -27808, -30657, -33694
};
TYPE res_test1001[32] = {
  1, -8, -31, -74, -143, -244, -383, -566, -799, -1088, -1439, -1858, -2351, -2924, -3583, -4334, -5183, -6136, -7199, -8378, -9679, -11108, -12671, -14374, -16223, -18224, -20383, -22706, -25199, -27868, -30719, -33758
};
TYPE res_test1010[32] = {
  -1, 8, 31, 74, 143, 244, 383, 566, 799, 1088, 1439, 1858, 2351, 2924, 3583, 4334, 5183, 6136, 7199, 8378, 9679, 11108, 12671, 14374, 16223, 18224, 20383, 22706, 25199, 27868, 30719, 33758
};
TYPE res_test1011[32] = {
  -3, 4, 25, 66, 133, 232, 369, 550, 781, 1068, 1417, 1834, 2325, 2896, 3553, 4302, 5149, 6100, 7161, 8338, 9637, 11064, 12625, 14326, 16173, 18172, 20329, 22650, 25141, 27808, 30657, 33694
};
TYPE res_test1100[32] = {
  -9, -28, -65, -126, -217, -344, -513, -730, -1001, -1332, -1729, -2198, -2745, -3376, -4097, -4914, -5833, -6860, -8001, -9262, -10649, -12168, -13825, -15626, -17577, -19684, -21953, -24390, -27001, -29792, -32769, -35938
};
TYPE res_test1101[32] = {
  -11, -32, -71, -134, -227, -356, -527, -746, -1019, -1352, -1751, -2222, -2771, -3404, -4127, -4946, -5867, -6896, -8039, -9302, -10691, -12212, -13871, -15674, -17627, -19736, -22007, -24446, -27059, -29852, -32831, -36002
};
TYPE res_test1110[32] = {
  11, 32, 71, 134, 227, 356, 527, 746, 1019, 1352, 1751, 2222, 2771, 3404, 4127, 4946, 5867, 6896, 8039, 9302, 10691, 12212, 13871, 15674, 17627, 19736, 22007, 24446, 27059, 29852, 32831, 36002
};
TYPE res_test1111[32] = {
  9, 28, 65, 126, 217, 344, 513, 730, 1001, 1332, 1729, 2198, 2745, 3376, 4097, 4914, 5833, 6860, 8001, 9262, 10649, 12168, 13825, 15626, 17577, 19684, 21953, 24390, 27001, 29792, 32769, 35938
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

#ifndef fma_main
#define fma_main

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
compare_result(char * title, TYPE *res)
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
  int i;
  for (i=0; i <32; i++)
    m4[i] = test_noneg_add_noneg_add (m1[i], m2[i], m3[i]);
  compare_result ("test0000", res_test0000);

  for (i=0; i <32; i++)
    m4[i] = test_noneg_add_noneg_sub (m1[i], m2[i], m3[i]);
  compare_result ("test0001", res_test0001);

  for (i=0; i <32; i++)
    m4[i] = test_noneg_add_neg_add (m1[i], m2[i], m3[i]);
  compare_result ("test0010", res_test0010);

  for (i=0; i <32; i++)
    m4[i] = test_noneg_add_neg_sub (m1[i], m2[i], m3[i]);
  compare_result ("test0011", res_test0011);

  for (i=0; i <32; i++)
    m4[i] = test_noneg_sub_noneg_add (m1[i], m2[i], m3[i]);
  compare_result ("test0100", res_test0100);

  for (i=0; i <32; i++)
    m4[i] = test_noneg_sub_noneg_sub (m1[i], m2[i], m3[i]);
  compare_result ("test0101", res_test0101);

  for (i=0; i <32; i++)
    m4[i] = test_noneg_sub_neg_add (m1[i], m2[i], m3[i]);
  compare_result ("test0110", res_test0110);

  for (i=0; i <32; i++)
    m4[i] = test_noneg_sub_neg_sub (m1[i], m2[i], m3[i]);
  compare_result ("test0111", res_test0111);

  for (i=0; i <32; i++)
    m4[i] = test_neg_add_noneg_add (m1[i], m2[i], m3[i]);
  compare_result ("test1000", res_test1000);

  for (i=0; i <32; i++)
    m4[i] = test_neg_add_noneg_sub (m1[i], m2[i], m3[i]);
  compare_result ("test1001", res_test1001);

  for (i=0; i <32; i++)
    m4[i] = test_neg_add_neg_add (m1[i], m2[i], m3[i]);
  compare_result ("test1010", res_test1010);

  for (i=0; i <32; i++)
    m4[i] = test_neg_add_neg_sub (m1[i], m2[i], m3[i]);
  compare_result ("test1011", res_test1011);

  for (i=0; i <32; i++)
    m4[i] = test_neg_sub_noneg_add (m1[i], m2[i], m3[i]);
  compare_result ("test1100", res_test1100);

  for (i=0; i <32; i++)
    m4[i] = test_neg_sub_noneg_sub (m1[i], m2[i], m3[i]);
  compare_result ("test1101", res_test1101);

  for (i=0; i <32; i++)
    m4[i] = test_neg_sub_neg_add (m1[i], m2[i], m3[i]);
  compare_result ("test1110", res_test1110);

  for (i=0; i <32; i++)
    m4[i] = test_neg_sub_neg_sub (m1[i], m2[i], m3[i]);
  compare_result ("test1111", res_test1111);

  if (test_fails) abort ();
}

#endif
