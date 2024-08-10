/* { dg-do run } */
/* { dg-options "-mmmx -mfpmath=387" } */

#include <stdio.h>
#include <stdlib.h>

#include "cpuid.h"

static void mmx_test (void);

static void
__attribute__ ((noinline))
do_test (void)
{
  mmx_test ();
}

int
main ()
{
  unsigned int eax, ebx, ecx, edx;
 
  if (!__get_cpuid (1, &eax, &ebx, &ecx, &edx))
    return 0;

  /* Run MMX test only if host has MMX support.  */
  if (edx & bit_MMX)
    do_test ();

  return 0;
}

#include <mmintrin.h>

typedef float float32x2_t __attribute__ ((vector_size (8)));

float
foo32x2_be (float32x2_t x)
{
  _mm_empty ();
  return x[1];
}

static void
mmx_test (void)
{
  float32x2_t b = { 0.0f, 1.0f };

  if (foo32x2_be (b) != 1.0f)
    abort ();
}
