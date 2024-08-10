/* { dg-do run } */
/* { dg-require-effective-target fma } */
/* { dg-options "-O3 -Wno-attributes -mfpmath=sse -mfma" } */

/* Test that the compiler properly optimizes floating point multiply
   and add instructions into FMA3 instructions.  */

#define TYPE double


#ifndef l_fma_4
#define l_fma_4

void __attribute__((sseregparm))
test_noneg_add_noneg_add (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = ((a[i] * b[i]) + c[i]) * b[i] + c[i];
}

void __attribute__((sseregparm))
test_noneg_add_noneg_sub (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = ((a[i] * b[i]) + c[i]) * b[i] - c[i];
}

void __attribute__((sseregparm))
test_noneg_add_neg_add (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = -((a[i] * b[i]) + c[i]) * b[i] + c[i];
}

void __attribute__((sseregparm))
test_noneg_add_neg_sub (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = -((a[i] * b[i]) + c[i]) * b[i] - c[i];
}

void __attribute__((sseregparm))
test_noneg_sub_noneg_add (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = ((a[i] * b[i]) - c[i]) * b[i] + c[i];
}

void __attribute__((sseregparm))
test_noneg_sub_noneg_sub (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = ((a[i] * b[i]) - c[i]) * b[i] - c[i];
}

void __attribute__((sseregparm))
test_noneg_sub_neg_add (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = -((a[i] * b[i]) - c[i]) * b[i] + c[i];
}

void __attribute__((sseregparm))
test_noneg_sub_neg_sub (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = -((a[i] * b[i]) - c[i]) * b[i] - c[i];
}

void __attribute__((sseregparm))
test_neg_add_noneg_add (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = (-(a[i] * b[i]) + c[i]) * b[i] + c[i];
}

void __attribute__((sseregparm))
test_neg_add_noneg_sub (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = (-(a[i] * b[i]) + c[i]) * b[i] - c[i];
}

void __attribute__((sseregparm))
test_neg_add_neg_add (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = -(-(a[i] * b[i]) + c[i]) * b[i] + c[i];
}

void __attribute__((sseregparm))
test_neg_add_neg_sub (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = -(-(a[i] * b[i]) + c[i]) * b[i] - c[i];
}

void __attribute__((sseregparm))
test_neg_sub_noneg_add (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = (-(a[i] * b[i]) - c[i]) * b[i] + c[i];
}

void __attribute__((sseregparm))
test_neg_sub_noneg_sub (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = (-(a[i] * b[i]) - c[i]) * b[i] - c[i];
}

void __attribute__((sseregparm))
test_neg_sub_neg_add (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = -(-(a[i] * b[i]) - c[i]) * b[i] + c[i];
}

void __attribute__((sseregparm))
test_neg_sub_neg_sub (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = -(-(a[i] * b[i]) - c[i]) * b[i] - c[i];
}

#endif


#ifndef fma_run_double_results_4
#define fma_run_double_results_4

TYPE res_test0000[32] = {
  13, 34, 73, 136, 229, 358, 529, 748, 1021, 1354, 1753, 2224, 2773, 3406, 4129, 4948, 5869, 6898, 8041, 9304, 10693, 12214, 13873, 15676, 17629, 19738, 22009, 24448, 27061, 29854, 32833, 36004
};
TYPE res_test0001[32] = {
  7, 26, 63, 124, 215, 342, 511, 728, 999, 1330, 1727, 2196, 2743, 3374, 4095, 4912, 5831, 6858, 7999, 9260, 10647, 12166, 13823, 15624, 17575, 19682, 21951, 24388, 26999, 29790, 32767, 35936
};
TYPE res_test0010[32] = {
  -7, -26, -63, -124, -215, -342, -511, -728, -999, -1330, -1727, -2196, -2743, -3374, -4095, -4912, -5831, -6858, -7999, -9260, -10647, -12166, -13823, -15624, -17575, -19682, -21951, -24388, -26999, -29790, -32767, -35936
};
TYPE res_test0011[32] = {
  -13, -34, -73, -136, -229, -358, -529, -748, -1021, -1354, -1753, -2224, -2773, -3406, -4129, -4948, -5869, -6898, -8041, -9304, -10693, -12214, -13873, -15676, -17629, -19738, -22009, -24448, -27061, -29854, -32833, -36004
};
TYPE res_test0100[32] = {
  1, 10, 33, 76, 145, 246, 385, 568, 801, 1090, 1441, 1860, 2353, 2926, 3585, 4336, 5185, 6138, 7201, 8380, 9681, 11110, 12673, 14376, 16225, 18226, 20385, 22708, 25201, 27870, 30721, 33760
};
TYPE res_test0101[32] = {
  -5, 2, 23, 64, 131, 230, 367, 548, 779, 1066, 1415, 1832, 2323, 2894, 3551, 4300, 5147, 6098, 7159, 8336, 9635, 11062, 12623, 14324, 16171, 18170, 20327, 22648, 25139, 27806, 30655, 33692
};
TYPE res_test0110[32] = {
  5, -2, -23, -64, -131, -230, -367, -548, -779, -1066, -1415, -1832, -2323, -2894, -3551, -4300, -5147, -6098, -7159, -8336, -9635, -11062, -12623, -14324, -16171, -18170, -20327, -22648, -25139, -27806, -30655, -33692
};
TYPE res_test0111[32] = {
  -1, -10, -33, -76, -145, -246, -385, -568, -801, -1090, -1441, -1860, -2353, -2926, -3585, -4336, -5185, -6138, -7201, -8380, -9681, -11110, -12673, -14376, -16225, -18226, -20385, -22708, -25201, -27870, -30721, -33760
};
TYPE res_test1000[32] = {
  5, -2, -23, -64, -131, -230, -367, -548, -779, -1066, -1415, -1832, -2323, -2894, -3551, -4300, -5147, -6098, -7159, -8336, -9635, -11062, -12623, -14324, -16171, -18170, -20327, -22648, -25139, -27806, -30655, -33692
};
TYPE res_test1001[32] = {
  -1, -10, -33, -76, -145, -246, -385, -568, -801, -1090, -1441, -1860, -2353, -2926, -3585, -4336, -5185, -6138, -7201, -8380, -9681, -11110, -12673, -14376, -16225, -18226, -20385, -22708, -25201, -27870, -30721, -33760
};
TYPE res_test1010[32] = {
  1, 10, 33, 76, 145, 246, 385, 568, 801, 1090, 1441, 1860, 2353, 2926, 3585, 4336, 5185, 6138, 7201, 8380, 9681, 11110, 12673, 14376, 16225, 18226, 20385, 22708, 25201, 27870, 30721, 33760
};
TYPE res_test1011[32] = {
  -5, 2, 23, 64, 131, 230, 367, 548, 779, 1066, 1415, 1832, 2323, 2894, 3551, 4300, 5147, 6098, 7159, 8336, 9635, 11062, 12623, 14324, 16171, 18170, 20327, 22648, 25139, 27806, 30655, 33692
};
TYPE res_test1100[32] = {
  -7, -26, -63, -124, -215, -342, -511, -728, -999, -1330, -1727, -2196, -2743, -3374, -4095, -4912, -5831, -6858, -7999, -9260, -10647, -12166, -13823, -15624, -17575, -19682, -21951, -24388, -26999, -29790, -32767, -35936
};
TYPE res_test1101[32] = {
  -13, -34, -73, -136, -229, -358, -529, -748, -1021, -1354, -1753, -2224, -2773, -3406, -4129, -4948, -5869, -6898, -8041, -9304, -10693, -12214, -13873, -15676, -17629, -19738, -22009, -24448, -27061, -29854, -32833, -36004
};
TYPE res_test1110[32] = {
  13, 34, 73, 136, 229, 358, 529, 748, 1021, 1354, 1753, 2224, 2773, 3406, 4129, 4948, 5869, 6898, 8041, 9304, 10693, 12214, 13873, 15676, 17629, 19738, 22009, 24448, 27061, 29854, 32833, 36004
};
TYPE res_test1111[32] = {
  7, 26, 63, 124, 215, 342, 511, 728, 999, 1330, 1727, 2196, 2743, 3374, 4095, 4912, 5831, 6858, 7999, 9260, 10647, 12166, 13823, 15624, 17575, 19682, 21951, 24388, 26999, 29790, 32767, 35936
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
