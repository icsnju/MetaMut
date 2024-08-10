/* { dg-require-effective-target vect_float } */
/* { dg-additional-options "-fdump-tree-optimized-details-blocks" } */

#include <stdarg.h>
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
   

#define N 128

/* Check handling of accesses for which the "initial condition" -
   the expression that represents the first location accessed - is
   more involved than just an ssa_name.  */

float a[N] __attribute__ ((__aligned__(__BIGGEST_ALIGNMENT__)));
float b[N+4] __attribute__ ((__aligned__(__BIGGEST_ALIGNMENT__))) = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 7.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0};
float c[N] __attribute__ ((__aligned__(__BIGGEST_ALIGNMENT__))) = {0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 7.5, 9.5, 10.5, 11.5, 12.5, 13.5, 14.5, 15.5};

__attribute__ ((noinline)) int
main1 (float *__restrict__  pa, float * __restrict__ pb, float * __restrict__ pc)
{
  int i;
  float *q = pb + 4;

  for (i = 0; i < N; i++)
    {
      b[i] = i;
      c[i] = 0.5 + i;
      asm volatile ("" ::: "memory");
    }
  for (; i < N+4; i++)
    {
      b[i] = i;
      asm volatile ("" ::: "memory");
    }

  for (i = 0; i < N; i++)
    {
      pa[i] = q[i] * pc[i];
    }

#pragma GCC novector
  for (i = 0; i < N; i++)
    {
      if (pa[i] != q[i] * pc[i])
	abort ();
    }

  return 0;
}

int main (void)
{
  check_vect ();

  main1 (a, b, c);

  return 0;
}

/* Xfail until handling restrict is refined.  See pr29145.  */
/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" } } */
/* Uncomment when this testcase gets vectorized again:
 dg-final { scan-tree-dump-times "Alignment of access forced using versioning" 3 "vect" { target vect_no_align } }
 dg-final { scan-tree-dump-times "Vectorizing an unaligned access" 2 "vect" { xfail vect_no_align } }
 dg-final { scan-tree-dump-times "Alignment of access forced using peeling" 1 "vect" { xfail vect_no_align } }
*/
/* { dg-final { scan-tree-dump-not "Invalid sum" "optimized" } } */
