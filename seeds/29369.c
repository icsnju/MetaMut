/* { dg-do compile { target { ! ia32 } } } */
/* { dg-options "-O2 -mbmi2 -dp" } */

/* { dg-do run { target { bmi2 && { ! ia32 } } } } */
/* { dg-options "-mbmi2 -O2" } */

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
unsigned __int128
calc_mul_u64 (unsigned long long volatile a, unsigned long long b)
{
  unsigned __int128 res = 0;
  int i;
  for (i = 0; i < b; ++i)
    res += (unsigned __int128) a;

  return res;
}

static void
bmi2_test ()
{
  unsigned i;
  unsigned long long a = 0xce7ace0ce7ace0;
  unsigned long long b = 0xface;
  unsigned __int128 res, res_ref;

  for (i=0; i<5; ++i) {
    a = a * (i + 1);
    b = b / (i + 1);

    res_ref = calc_mul_u64 (a, b);
    res = (unsigned __int128) a * b;

    if (res != res_ref)
      abort();
  }
}

/* { dg-final { scan-assembler-times "bmi2_umulditi3_1" 1 } } */
