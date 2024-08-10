/* { dg-do run } */
/* { dg-require-effective-target fma } */
/* { dg-options "-O3 -Wno-attributes -mfpmath=sse -mfma" } */

/* Test that the compiler properly optimizes floating point multiply
   and add instructions into FMA3 instructions.  */

#define TYPE float


#ifndef fma_2
#define fma_2

TYPE __attribute__((sseregparm))
test_noneg_add_noneg_add (TYPE a, TYPE b, TYPE c)
{
  return ((a * b) + c) * a + c;
}

TYPE __attribute__((sseregparm))
test_noneg_add_noneg_sub (TYPE a, TYPE b, TYPE c)
{
  return ((a * b) + c) * a - c;
}

TYPE __attribute__((sseregparm))
test_noneg_add_neg_add (TYPE a, TYPE b, TYPE c)
{
  return -((a * b) + c) * a + c;
}

TYPE __attribute__((sseregparm))
test_noneg_add_neg_sub (TYPE a, TYPE b, TYPE c)
{
  return -((a * b) + c) * a - c;
}

TYPE __attribute__((sseregparm))
test_noneg_sub_noneg_add (TYPE a, TYPE b, TYPE c)
{
  return ((a * b) - c) * a + c;
}

TYPE __attribute__((sseregparm))
test_noneg_sub_noneg_sub (TYPE a, TYPE b, TYPE c)
{
  return ((a * b) - c) * a - c;
}

TYPE __attribute__((sseregparm))
test_noneg_sub_neg_add (TYPE a, TYPE b, TYPE c)
{
  return -((a * b) - c) * a + c;
}

TYPE __attribute__((sseregparm))
test_noneg_sub_neg_sub (TYPE a, TYPE b, TYPE c)
{
  return -((a * b) - c) * a - c;
}

TYPE __attribute__((sseregparm))
test_neg_add_noneg_add (TYPE a, TYPE b, TYPE c)
{
  return (-(a * b) + c) * a + c;
}

TYPE __attribute__((sseregparm))
test_neg_add_noneg_sub (TYPE a, TYPE b, TYPE c)
{
  return (-(a * b) + c) * a - c;
}

TYPE __attribute__((sseregparm))
test_neg_add_neg_add (TYPE a, TYPE b, TYPE c)
{
  return -(-(a * b) + c) * a + c;
}

TYPE __attribute__((sseregparm))
test_neg_add_neg_sub (TYPE a, TYPE b, TYPE c)
{
  return -(-(a * b) + c) * a - c;
}

TYPE __attribute__((sseregparm))
test_neg_sub_noneg_add (TYPE a, TYPE b, TYPE c)
{
  return (-(a * b) - c) * a + c;
}

TYPE __attribute__((sseregparm))
test_neg_sub_noneg_sub (TYPE a, TYPE b, TYPE c)
{
  return (-(a * b) - c) * a - c;
}

TYPE __attribute__((sseregparm))
test_neg_sub_neg_add (TYPE a, TYPE b, TYPE c)
{
  return -(-(a * b) - c) * a + c;
}

TYPE __attribute__((sseregparm))
test_neg_sub_neg_sub (TYPE a, TYPE b, TYPE c)
{
  return -(-(a * b) - c) * a - c;
}

#endif


#ifndef fma_run_float_results_2
#define fma_run_float_results_2

TYPE res_test0000[32] = {
  8, 24, 56, 110, 192, 308, 464, 666, 920, 1232, 1608, 2054, 2576, 3180, 3872, 4658, 5544, 6536, 7640, 8862, 10208, 11684, 13296, 15050, 16952, 19008, 21224, 23606, 26160, 28892, 31808, 34914
};
TYPE res_test0001[32] = {
  2, 16, 46, 98, 178, 292, 446, 646, 898, 1208, 1582, 2026, 2546, 3148, 3838, 4622, 5506, 6496, 7598, 8818, 10162, 11636, 13246, 14998, 16898, 18952, 21166, 23546, 26098, 28828, 31742, 34846
};
TYPE res_test0010[32] = {
  -2, -16, -46, -98, -178, -292, -446, -646, -898, -1208, -1582, -2026, -2546, -3148, -3838, -4622, -5506, -6496, -7598, -8818, -10162, -11636, -13246, -14998, -16898, -18952, -21166, -23546, -26098, -28828, -31742, -34846
};
TYPE res_test0011[32] = {
  -8, -24, -56, -110, -192, -308, -464, -666, -920, -1232, -1608, -2054, -2576, -3180, -3872, -4658, -5544, -6536, -7640, -8862, -10208, -11684, -13296, -15050, -16952, -19008, -21224, -23606, -26160, -28892, -31808, -34914
};
TYPE res_test0100[32] = {
  2, 8, 26, 62, 122, 212, 338, 506, 722, 992, 1322, 1718, 2186, 2732, 3362, 4082, 4898, 5816, 6842, 7982, 9242, 10628, 12146, 13802, 15602, 17552, 19658, 21926, 24362, 26972, 29762, 32738
};
TYPE res_test0101[32] = {
  -4, 0, 16, 50, 108, 196, 320, 486, 700, 968, 1296, 1690, 2156, 2700, 3328, 4046, 4860, 5776, 6800, 7938, 9196, 10580, 12096, 13750, 15548, 17496, 19600, 21866, 24300, 26908, 29696, 32670
};
TYPE res_test0110[32] = {
  4, 0, -16, -50, -108, -196, -320, -486, -700, -968, -1296, -1690, -2156, -2700, -3328, -4046, -4860, -5776, -6800, -7938, -9196, -10580, -12096, -13750, -15548, -17496, -19600, -21866, -24300, -26908, -29696, -32670
};
TYPE res_test0111[32] = {
  -2, -8, -26, -62, -122, -212, -338, -506, -722, -992, -1322, -1718, -2186, -2732, -3362, -4082, -4898, -5816, -6842, -7982, -9242, -10628, -12146, -13802, -15602, -17552, -19658, -21926, -24362, -26972, -29762, -32738
};
TYPE res_test1000[32] = {
  4, 0, -16, -50, -108, -196, -320, -486, -700, -968, -1296, -1690, -2156, -2700, -3328, -4046, -4860, -5776, -6800, -7938, -9196, -10580, -12096, -13750, -15548, -17496, -19600, -21866, -24300, -26908, -29696, -32670
};
TYPE res_test1001[32] = {
  -2, -8, -26, -62, -122, -212, -338, -506, -722, -992, -1322, -1718, -2186, -2732, -3362, -4082, -4898, -5816, -6842, -7982, -9242, -10628, -12146, -13802, -15602, -17552, -19658, -21926, -24362, -26972, -29762, -32738
};
TYPE res_test1010[32] = {
  2, 8, 26, 62, 122, 212, 338, 506, 722, 992, 1322, 1718, 2186, 2732, 3362, 4082, 4898, 5816, 6842, 7982, 9242, 10628, 12146, 13802, 15602, 17552, 19658, 21926, 24362, 26972, 29762, 32738
};
TYPE res_test1011[32] = {
  -4, 0, 16, 50, 108, 196, 320, 486, 700, 968, 1296, 1690, 2156, 2700, 3328, 4046, 4860, 5776, 6800, 7938, 9196, 10580, 12096, 13750, 15548, 17496, 19600, 21866, 24300, 26908, 29696, 32670
};
TYPE res_test1100[32] = {
  -2, -16, -46, -98, -178, -292, -446, -646, -898, -1208, -1582, -2026, -2546, -3148, -3838, -4622, -5506, -6496, -7598, -8818, -10162, -11636, -13246, -14998, -16898, -18952, -21166, -23546, -26098, -28828, -31742, -34846
};
TYPE res_test1101[32] = {
  -8, -24, -56, -110, -192, -308, -464, -666, -920, -1232, -1608, -2054, -2576, -3180, -3872, -4658, -5544, -6536, -7640, -8862, -10208, -11684, -13296, -15050, -16952, -19008, -21224, -23606, -26160, -28892, -31808, -34914
};
TYPE res_test1110[32] = {
  8, 24, 56, 110, 192, 308, 464, 666, 920, 1232, 1608, 2054, 2576, 3180, 3872, 4658, 5544, 6536, 7640, 8862, 10208, 11684, 13296, 15050, 16952, 19008, 21224, 23606, 26160, 28892, 31808, 34914
};
TYPE res_test1111[32] = {
  2, 16, 46, 98, 178, 292, 446, 646, 898, 1208, 1582, 2026, 2546, 3148, 3838, 4622, 5506, 6496, 7598, 8818, 10162, 11636, 13246, 14998, 16898, 18952, 21166, 23546, 26098, 28828, 31742, 34846
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
