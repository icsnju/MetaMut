/* { dg-do run } */
/* { dg-require-effective-target fma } */
/* { dg-options "-O3 -Wno-attributes -mfpmath=sse -mfma" } */

/* Test that the compiler properly optimizes floating point multiply
   and add instructions into FMA3 instructions.  */

#define TYPE float


#ifndef fma_5
#define fma_5

TYPE __attribute__((sseregparm))
test_noneg_add_noneg_add (TYPE a, TYPE b, TYPE c)
{
  return ((a * b) + c) * c + a;
}

TYPE __attribute__((sseregparm))
test_noneg_add_noneg_sub (TYPE a, TYPE b, TYPE c)
{
  return ((a * b) + c) * c - a;
}

TYPE __attribute__((sseregparm))
test_noneg_add_neg_add (TYPE a, TYPE b, TYPE c)
{
  return -((a * b) + c) * c + a;
}

TYPE __attribute__((sseregparm))
test_noneg_add_neg_sub (TYPE a, TYPE b, TYPE c)
{
  return -((a * b) + c) * c - a;
}

TYPE __attribute__((sseregparm))
test_noneg_sub_noneg_add (TYPE a, TYPE b, TYPE c)
{
  return ((a * b) - c) * c + a;
}

TYPE __attribute__((sseregparm))
test_noneg_sub_noneg_sub (TYPE a, TYPE b, TYPE c)
{
  return ((a * b) - c) * c - a;
}

TYPE __attribute__((sseregparm))
test_noneg_sub_neg_add (TYPE a, TYPE b, TYPE c)
{
  return -((a * b) - c) * c + a;
}

TYPE __attribute__((sseregparm))
test_noneg_sub_neg_sub (TYPE a, TYPE b, TYPE c)
{
  return -((a * b) - c) * c - a;
}

TYPE __attribute__((sseregparm))
test_neg_add_noneg_add (TYPE a, TYPE b, TYPE c)
{
  return (-(a * b) + c) * c + a;
}

TYPE __attribute__((sseregparm))
test_neg_add_noneg_sub (TYPE a, TYPE b, TYPE c)
{
  return (-(a * b) + c) * c - a;
}

TYPE __attribute__((sseregparm))
test_neg_add_neg_add (TYPE a, TYPE b, TYPE c)
{
  return -(-(a * b) + c) * c + a;
}

TYPE __attribute__((sseregparm))
test_neg_add_neg_sub (TYPE a, TYPE b, TYPE c)
{
  return -(-(a * b) + c) * c - a;
}

TYPE __attribute__((sseregparm))
test_neg_sub_noneg_add (TYPE a, TYPE b, TYPE c)
{
  return (-(a * b) - c) * c + a;
}

TYPE __attribute__((sseregparm))
test_neg_sub_noneg_sub (TYPE a, TYPE b, TYPE c)
{
  return (-(a * b) - c) * c - a;
}

TYPE __attribute__((sseregparm))
test_neg_sub_neg_add (TYPE a, TYPE b, TYPE c)
{
  return -(-(a * b) - c) * c + a;
}

TYPE __attribute__((sseregparm))
test_neg_sub_neg_sub (TYPE a, TYPE b, TYPE c)
{
  return -(-(a * b) - c) * c - a;
}

#endif


#ifndef fma_run_float_results_5
#define fma_run_float_results_5

TYPE res_test0000[32] = {
  16, 42, 88, 160, 264, 406, 592, 828, 1120, 1474, 1896, 2392, 2968, 3630, 4384, 5236, 6192, 7258, 8440, 9744, 11176, 12742, 14448, 16300, 18304, 20466, 22792, 25288, 27960, 30814, 33856, 37092
};
TYPE res_test0001[32] = {
  14, 38, 82, 152, 254, 394, 578, 812, 1102, 1454, 1874, 2368, 2942, 3602, 4354, 5204, 6158, 7222, 8402, 9704, 11134, 12698, 14402, 16252, 18254, 20414, 22738, 25232, 27902, 30754, 33794, 37028
};
TYPE res_test0010[32] = {
  -14, -38, -82, -152, -254, -394, -578, -812, -1102, -1454, -1874, -2368, -2942, -3602, -4354, -5204, -6158, -7222, -8402, -9704, -11134, -12698, -14402, -16252, -18254, -20414, -22738, -25232, -27902, -30754, -33794, -37028
};
TYPE res_test0011[32] = {
  -16, -42, -88, -160, -264, -406, -592, -828, -1120, -1474, -1896, -2392, -2968, -3630, -4384, -5236, -6192, -7258, -8440, -9744, -11176, -12742, -14448, -16300, -18304, -20466, -22792, -25288, -27960, -30814, -33856, -37092
};
TYPE res_test0100[32] = {
  -2, 10, 38, 88, 166, 278, 430, 628, 878, 1186, 1558, 2000, 2518, 3118, 3806, 4588, 5470, 6458, 7558, 8776, 10118, 11590, 13198, 14948, 16846, 18898, 21110, 23488, 26038, 28766, 31678, 34780
};
TYPE res_test0101[32] = {
  -4, 6, 32, 80, 156, 266, 416, 612, 860, 1166, 1536, 1976, 2492, 3090, 3776, 4556, 5436, 6422, 7520, 8736, 10076, 11546, 13152, 14900, 16796, 18846, 21056, 23432, 25980, 28706, 31616, 34716
};
TYPE res_test0110[32] = {
  4, -6, -32, -80, -156, -266, -416, -612, -860, -1166, -1536, -1976, -2492, -3090, -3776, -4556, -5436, -6422, -7520, -8736, -10076, -11546, -13152, -14900, -16796, -18846, -21056, -23432, -25980, -28706, -31616, -34716
};
TYPE res_test0111[32] = {
  2, -10, -38, -88, -166, -278, -430, -628, -878, -1186, -1558, -2000, -2518, -3118, -3806, -4588, -5470, -6458, -7558, -8776, -10118, -11590, -13198, -14948, -16846, -18898, -21110, -23488, -26038, -28766, -31678, -34780
};
TYPE res_test1000[32] = {
  4, -6, -32, -80, -156, -266, -416, -612, -860, -1166, -1536, -1976, -2492, -3090, -3776, -4556, -5436, -6422, -7520, -8736, -10076, -11546, -13152, -14900, -16796, -18846, -21056, -23432, -25980, -28706, -31616, -34716
};
TYPE res_test1001[32] = {
  2, -10, -38, -88, -166, -278, -430, -628, -878, -1186, -1558, -2000, -2518, -3118, -3806, -4588, -5470, -6458, -7558, -8776, -10118, -11590, -13198, -14948, -16846, -18898, -21110, -23488, -26038, -28766, -31678, -34780
};
TYPE res_test1010[32] = {
  -2, 10, 38, 88, 166, 278, 430, 628, 878, 1186, 1558, 2000, 2518, 3118, 3806, 4588, 5470, 6458, 7558, 8776, 10118, 11590, 13198, 14948, 16846, 18898, 21110, 23488, 26038, 28766, 31678, 34780
};
TYPE res_test1011[32] = {
  -4, 6, 32, 80, 156, 266, 416, 612, 860, 1166, 1536, 1976, 2492, 3090, 3776, 4556, 5436, 6422, 7520, 8736, 10076, 11546, 13152, 14900, 16796, 18846, 21056, 23432, 25980, 28706, 31616, 34716
};
TYPE res_test1100[32] = {
  -14, -38, -82, -152, -254, -394, -578, -812, -1102, -1454, -1874, -2368, -2942, -3602, -4354, -5204, -6158, -7222, -8402, -9704, -11134, -12698, -14402, -16252, -18254, -20414, -22738, -25232, -27902, -30754, -33794, -37028
};
TYPE res_test1101[32] = {
  -16, -42, -88, -160, -264, -406, -592, -828, -1120, -1474, -1896, -2392, -2968, -3630, -4384, -5236, -6192, -7258, -8440, -9744, -11176, -12742, -14448, -16300, -18304, -20466, -22792, -25288, -27960, -30814, -33856, -37092
};
TYPE res_test1110[32] = {
  16, 42, 88, 160, 264, 406, 592, 828, 1120, 1474, 1896, 2392, 2968, 3630, 4384, 5236, 6192, 7258, 8440, 9744, 11176, 12742, 14448, 16300, 18304, 20466, 22792, 25288, 27960, 30814, 33856, 37092
};
TYPE res_test1111[32] = {
  14, 38, 82, 152, 254, 394, 578, 812, 1102, 1454, 1874, 2368, 2942, 3602, 4354, 5204, 6158, 7222, 8402, 9704, 11134, 12698, 14402, 16252, 18254, 20414, 22738, 25232, 27902, 30754, 33794, 37028
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
