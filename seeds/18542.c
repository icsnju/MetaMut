/* { dg-do compile } */
/* { dg-options "-O2 -mbmi2 -dp" } */

/* { dg-do run { target bmi2 } } */
/* { dg-options "-mbmi2 -O2 -dp" } */

#include <stdio.h>
#include <stdlib.h>
#include "cpuid.h"

static void bmi2_test (void);

static void
__attribute__ ((noinline))
do_test (void)
{
  bmi2_test ();
}

int
main ()
{
  unsigned int eax, ebx, ecx, edx;

  if (!__get_cpuid_count (7, 0, &eax, &ebx, &ecx, &edx))
    return 0;

  /* Run BMI2 test only if host has BMI2 support.  */
  if (ebx & bit_BMI2)
    {
      do_test ();
#ifdef DEBUG
      printf ("PASSED\n");
#endif
      return 0;
    }

#ifdef DEBUG
  printf ("SKIPPED\n");
#endif
  return 0;
}

__attribute__((noinline))
int
calc_sarx_u32 (int a, int l)
{
  int volatile res = a;
  int i;
  for (i = 0; i < l; ++i)
    res >>= 1;

  return res;
}

static void
bmi2_test ()
{
  unsigned i;
  int src = 0xfce7ace0;
  int res, res_ref;

  for (i = 0; i < 5; ++i) {
    src = src * (i + 1);

    res_ref = calc_sarx_u32 (src, i + 1);
    res = src >> (i + 1);

    if (res != res_ref)
      abort();
  }
}

/* { dg-final { scan-assembler-times "bmi2_ashrsi3" 1 } } */
