/* { dg-require-effective-target size32plus } */
/* { dg-additional-options "-fopenmp-simd" } */
/* { dg-additional-options "-mavx" { target avx_runtime } } */
/* { dg-final { scan-tree-dump-times "vectorized \[1-3] loops" 2 "vect" { target i?86-*-* x86_64-*-* } } } */

#ifndef main
/* Check if system supports SIMD */
#include <signal.h>

#if defined(__i386__) || defined(__x86_64__)
# include "cpuid.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern void abort (void);
extern void exit (int);
#ifdef __cplusplus
}
#endif

static void
sig_ill_handler (int sig)
{
  exit(0);
}

static void __attribute__((noinline))
check_vect (void)
{
  signal(SIGILL, sig_ill_handler);
#if defined(__PAIRED__)
  /* 750CL paired-single instruction, 'ps_mul %v0,%v0,%v0'.  */
 asm volatile (".long 0x10000032");
#elif defined(__ppc__) || defined(__ppc64__) || defined(__powerpc__) || defined(powerpc)
  /* Altivec instruction, 'vor %v0,%v0,%v0'.  */
  asm volatile (".long 0x10000484");
#elif defined(__i386__) || defined(__x86_64__)
  {
    unsigned int a, b, c, d,
      want_level, want_b = 0, want_c = 0, want_d = 0;

    /* Determine what instruction set we've been compiled for, and detect
       that we're running with it.  This allows us to at least do a compile
       check for, e.g. SSE4.1 when the machine only supports SSE2.  */
# if defined(__AVX2__)
    want_level = 7, want_b = bit_AVX2;
# elif defined(__AVX__)
    want_level = 1, want_c = bit_AVX;
# elif defined(__SSE4_2__)
    want_level = 1, want_c = bit_SSE4_2;
# elif defined(__SSE4_1__)
    want_level = 1, want_c = bit_SSE4_1;
# elif defined(__SSSE3__)
    want_level = 1, want_c = bit_SSSE3;
# else
    want_level = 1, want_d = bit_SSE2;
# endif

    if (!__get_cpuid_count (want_level, 0, &a, &b, &c, &d)
	|| ((b & want_b) | (c & want_c) | (d & want_d)) == 0)
      exit (0);
  }
#elif defined(__sparc__)
  asm volatile (".word\t0x81b007c0");
#elif defined(__arm__)
  {
    /* On some processors without NEON support, this instruction may
       be a no-op, on others it may trap, so check that it executes
       correctly.  */
    long long a = 0, b = 1;
    asm ("vorr %P0, %P1, %P2"
	 : "=w" (a)
	 : "0" (a), "w" (b));
    if (a != 1)
      exit (0);
  }
#elif defined(__mips_msa)
  asm volatile ("or.v $w0,$w0,$w0");
#endif
  signal (SIGILL, SIG_DFL);
}

#if defined (__ARM_FEATURE_SVE)
#  if __ARM_FEATURE_SVE_BITS == 0
#    define VECTOR_BITS 1024
#  else
#    define VECTOR_BITS __ARM_FEATURE_SVE_BITS
#  endif
#else
#  define VECTOR_BITS 128
#endif

/* Which most of our tests are going to #define for internal use, and
   which might be exposed by system headers related to signal.h on some
   targets, notably VxWorks.  */
#undef OFFSET
   
#endif

#ifdef __FAST_MATH__
#define FLT_MIN_VALUE (-__FLT_MAX__)
#else
#define FLT_MIN_VALUE (-__builtin_inff ())
#endif

float r = 1.0f, a[1024], b[1024];

__attribute__((noipa)) void
foo (float *a, float *b)
{
  #pragma omp simd reduction (inscan, *:r)
  for (int i = 0; i < 1024; i++)
    {
      r *= a[i];
      #pragma omp scan inclusive(r)
      b[i] = r;
    }
}

__attribute__((noipa)) float
bar (void)
{
  float s = FLT_MIN_VALUE;
  #pragma omp simd reduction (inscan, max:s)
  for (int i = 0; i < 1024; i++)
    {
      s = s > a[i] ? s : a[i];
      #pragma omp scan inclusive(s)
      b[i] = s;
    }
  return s;
}

int
main ()
{
  float s = 1.0f;
#ifndef main
  check_vect ();
#endif
  for (int i = 0; i < 1024; ++i)
    {
      if (i < 80)
	a[i] = (i & 1) ? 0.25f : 0.5f;
      else if (i < 200)
	a[i] = (i % 3) == 0 ? 2.0f : (i % 3) == 1 ? 4.0f : 1.0f;
      else if (i < 280)
	a[i] = (i & 1) ? 0.25f : 0.5f;
      else if (i < 380)
	a[i] = (i % 3) == 0 ? 2.0f : (i % 3) == 1 ? 4.0f : 1.0f;
      else
	switch (i % 6)
	  {
	  case 0: a[i] = 0.25f; break;
	  case 1: a[i] = 2.0f; break;
	  case 2: a[i] = -1.0f; break;
	  case 3: a[i] = -4.0f; break;
	  case 4: a[i] = 0.5f; break;
	  case 5: a[i] = 1.0f; break;
	  default: a[i] = 0.0f; break;
	  }
      b[i] = -19.0f;
      asm ("" : "+g" (i));
    }
  foo (a, b);
  if (r * 16384.0f != 0.125f)
    abort ();
  float m = -175.25f;
#pragma GCC novector
  for (int i = 0; i < 1024; ++i)
    {
      s *= a[i];
      if (b[i] != s)
	abort ();
      else
	{
	  a[i] = m - ((i % 3) == 1 ? 2.0f : (i % 3) == 2 ? 4.0f : 0.0f);
	  b[i] = -231.75f;
	  m += 0.75f;
	}
    }
  if (bar () != 592.0f)
    abort ();
  s = FLT_MIN_VALUE;
#pragma GCC novector
  for (int i = 0; i < 1024; ++i)
    {
      if (s < a[i])
	s = a[i];
      if (b[i] != s)
	abort ();
    }
  return 0;
}
