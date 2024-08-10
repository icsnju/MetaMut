/* { dg-do run } */
/* { dg-require-effective-target fma } */
/* { dg-options "-O3 -Wno-attributes -mfpmath=sse -mfma" } */

/* Test that the compiler properly optimizes floating point multiply
   and add instructions into FMA3 instructions.  */

#define TYPE float


#ifndef fma_6
#define fma_6

TYPE __attribute__((sseregparm))
test_noneg_add_noneg_add (TYPE a, TYPE b, TYPE c)
{
  return ((a * b) + c) * c + b;
}

TYPE __attribute__((sseregparm))
test_noneg_add_noneg_sub (TYPE a, TYPE b, TYPE c)
{
  return ((a * b) + c) * c - b;
}

TYPE __attribute__((sseregparm))
test_noneg_add_neg_add (TYPE a, TYPE b, TYPE c)
{
  return -((a * b) + c) * c + b;
}

TYPE __attribute__((sseregparm))
test_noneg_add_neg_sub (TYPE a, TYPE b, TYPE c)
{
  return -((a * b) + c) * c - b;
}

TYPE __attribute__((sseregparm))
test_noneg_sub_noneg_add (TYPE a, TYPE b, TYPE c)
{
  return ((a * b) - c) * c + b;
}

TYPE __attribute__((sseregparm))
test_noneg_sub_noneg_sub (TYPE a, TYPE b, TYPE c)
{
  return ((a * b) - c) * c - b;
}

TYPE __attribute__((sseregparm))
test_noneg_sub_neg_add (TYPE a, TYPE b, TYPE c)
{
  return -((a * b) - c) * c + b;
}

TYPE __attribute__((sseregparm))
test_noneg_sub_neg_sub (TYPE a, TYPE b, TYPE c)
{
  return -((a * b) - c) * c - b;
}

TYPE __attribute__((sseregparm))
test_neg_add_noneg_add (TYPE a, TYPE b, TYPE c)
{
  return (-(a * b) + c) * c + b;
}

TYPE __attribute__((sseregparm))
test_neg_add_noneg_sub (TYPE a, TYPE b, TYPE c)
{
  return (-(a * b) + c) * c - b;
}

TYPE __attribute__((sseregparm))
test_neg_add_neg_add (TYPE a, TYPE b, TYPE c)
{
  return -(-(a * b) + c) * c + b;
}

TYPE __attribute__((sseregparm))
test_neg_add_neg_sub (TYPE a, TYPE b, TYPE c)
{
  return -(-(a * b) + c) * c - b;
}

TYPE __attribute__((sseregparm))
test_neg_sub_noneg_add (TYPE a, TYPE b, TYPE c)
{
  return (-(a * b) - c) * c + b;
}

TYPE __attribute__((sseregparm))
test_neg_sub_noneg_sub (TYPE a, TYPE b, TYPE c)
{
  return (-(a * b) - c) * c - b;
}

TYPE __attribute__((sseregparm))
test_neg_sub_neg_add (TYPE a, TYPE b, TYPE c)
{
  return -(-(a * b) - c) * c + b;
}

TYPE __attribute__((sseregparm))
test_neg_sub_neg_sub (TYPE a, TYPE b, TYPE c)
{
  return -(-(a * b) - c) * c - b;
}

#endif


#ifndef fma_run_float_results_6
#define fma_run_float_results_6

TYPE res_test0000[32] = {
  17, 43, 89, 161, 265, 407, 593, 829, 1121, 1475, 1897, 2393, 2969, 3631, 4385, 5237, 6193, 7259, 8441, 9745, 11177, 12743, 14449, 16301, 18305, 20467, 22793, 25289, 27961, 30815, 33857, 37093
};
TYPE res_test0001[32] = {
  13, 37, 81, 151, 253, 393, 577, 811, 1101, 1453, 1873, 2367, 2941, 3601, 4353, 5203, 6157, 7221, 8401, 9703, 11133, 12697, 14401, 16251, 18253, 20413, 22737, 25231, 27901, 30753, 33793, 37027
};
TYPE res_test0010[32] = {
  -13, -37, -81, -151, -253, -393, -577, -811, -1101, -1453, -1873, -2367, -2941, -3601, -4353, -5203, -6157, -7221, -8401, -9703, -11133, -12697, -14401, -16251, -18253, -20413, -22737, -25231, -27901, -30753, -33793, -37027
};
TYPE res_test0011[32] = {
  -17, -43, -89, -161, -265, -407, -593, -829, -1121, -1475, -1897, -2393, -2969, -3631, -4385, -5237, -6193, -7259, -8441, -9745, -11177, -12743, -14449, -16301, -18305, -20467, -22793, -25289, -27961, -30815, -33857, -37093
};
TYPE res_test0100[32] = {
  -1, 11, 39, 89, 167, 279, 431, 629, 879, 1187, 1559, 2001, 2519, 3119, 3807, 4589, 5471, 6459, 7559, 8777, 10119, 11591, 13199, 14949, 16847, 18899, 21111, 23489, 26039, 28767, 31679, 34781
};
TYPE res_test0101[32] = {
  -5, 5, 31, 79, 155, 265, 415, 611, 859, 1165, 1535, 1975, 2491, 3089, 3775, 4555, 5435, 6421, 7519, 8735, 10075, 11545, 13151, 14899, 16795, 18845, 21055, 23431, 25979, 28705, 31615, 34715
};
TYPE res_test0110[32] = {
  5, -5, -31, -79, -155, -265, -415, -611, -859, -1165, -1535, -1975, -2491, -3089, -3775, -4555, -5435, -6421, -7519, -8735, -10075, -11545, -13151, -14899, -16795, -18845, -21055, -23431, -25979, -28705, -31615, -34715
};
TYPE res_test0111[32] = {
  1, -11, -39, -89, -167, -279, -431, -629, -879, -1187, -1559, -2001, -2519, -3119, -3807, -4589, -5471, -6459, -7559, -8777, -10119, -11591, -13199, -14949, -16847, -18899, -21111, -23489, -26039, -28767, -31679, -34781
};
TYPE res_test1000[32] = {
  5, -5, -31, -79, -155, -265, -415, -611, -859, -1165, -1535, -1975, -2491, -3089, -3775, -4555, -5435, -6421, -7519, -8735, -10075, -11545, -13151, -14899, -16795, -18845, -21055, -23431, -25979, -28705, -31615, -34715
};
TYPE res_test1001[32] = {
  1, -11, -39, -89, -167, -279, -431, -629, -879, -1187, -1559, -2001, -2519, -3119, -3807, -4589, -5471, -6459, -7559, -8777, -10119, -11591, -13199, -14949, -16847, -18899, -21111, -23489, -26039, -28767, -31679, -34781
};
TYPE res_test1010[32] = {
  -1, 11, 39, 89, 167, 279, 431, 629, 879, 1187, 1559, 2001, 2519, 3119, 3807, 4589, 5471, 6459, 7559, 8777, 10119, 11591, 13199, 14949, 16847, 18899, 21111, 23489, 26039, 28767, 31679, 34781
};
TYPE res_test1011[32] = {
  -5, 5, 31, 79, 155, 265, 415, 611, 859, 1165, 1535, 1975, 2491, 3089, 3775, 4555, 5435, 6421, 7519, 8735, 10075, 11545, 13151, 14899, 16795, 18845, 21055, 23431, 25979, 28705, 31615, 34715
};
TYPE res_test1100[32] = {
  -13, -37, -81, -151, -253, -393, -577, -811, -1101, -1453, -1873, -2367, -2941, -3601, -4353, -5203, -6157, -7221, -8401, -9703, -11133, -12697, -14401, -16251, -18253, -20413, -22737, -25231, -27901, -30753, -33793, -37027
};
TYPE res_test1101[32] = {
  -17, -43, -89, -161, -265, -407, -593, -829, -1121, -1475, -1897, -2393, -2969, -3631, -4385, -5237, -6193, -7259, -8441, -9745, -11177, -12743, -14449, -16301, -18305, -20467, -22793, -25289, -27961, -30815, -33857, -37093
};
TYPE res_test1110[32] = {
  17, 43, 89, 161, 265, 407, 593, 829, 1121, 1475, 1897, 2393, 2969, 3631, 4385, 5237, 6193, 7259, 8441, 9745, 11177, 12743, 14449, 16301, 18305, 20467, 22793, 25289, 27961, 30815, 33857, 37093
};
TYPE res_test1111[32] = {
  13, 37, 81, 151, 253, 393, 577, 811, 1101, 1453, 1873, 2367, 2941, 3601, 4353, 5203, 6157, 7221, 8401, 9703, 11133, 12697, 14401, 16251, 18253, 20413, 22737, 25231, 27901, 30753, 33793, 37027
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
