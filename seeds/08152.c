/* Disabling epilogues until we find a better way to deal with scans.  */
/* { dg-additional-options "--param vect-epilogues-nomask=0 -fdump-tree-optimized-details-blocks" } */
/* { dg-require-effective-target vect_float } */

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
   

#define N 256

float pa[N] __attribute__ ((__aligned__(__BIGGEST_ALIGNMENT__)));
float pb[N] __attribute__ ((__aligned__(__BIGGEST_ALIGNMENT__))) = {0,3,6,9,12,15,18,21,24,27,30,33,36,39,42,45,48,51,54,57};
float pc[N] __attribute__ ((__aligned__(__BIGGEST_ALIGNMENT__))) = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};

/* Check handling of unaligned accesses when the misalignment is
   known at compile time and different accesses have the same
   misalignment (e.g. peeling to align one access will align all
   accesses with the same misalignment.  Also, the number of 
   peeled iterations is known in this case, and the vectorizer
   can use this information (generate prolog and epilog loops
   with known number of iterations, and only if needed).  */

#if VECTOR_BITS > 128
#define NITER (VECTOR_BITS * 3 / 32)
#else
#define NITER 12
#endif

__attribute__ ((noinline)) int
main1 ()
{
  int i;

  for (i = 0; i < NITER - 2; i++)
    {
      pa[i+1] = pb[i+1] * pc[i+1];
    }

  /* check results:  */
#pragma GCC novector
  for (i = 0; i < 10; i++)
    {
      if (pa[i+1] != (pb[i+1] * pc[i+1]))
	abort ();
    }

  return 0;
}

__attribute__ ((noinline)) int
main2 ()
{
  int i;

  for (i = 0; i < NITER; i++)
    {
      pa[i+1] = pb[i+1] * pc[i+1];
    }

  /* check results:  */
#pragma GCC novector
  for (i = 0; i < 12; i++)
    {
      if (pa[i+1] != (pb[i+1] * pc[i+1]))
	abort ();
    }

  return 0;
}

__attribute__ ((noinline)) int
main3 (int n)
{
  int i;

  for (i = 0; i < n; i++)
    {
      pa[i+1] = pb[i+1] * pc[i+1];
    }

  /* check results:  */
#pragma GCC novector
  for (i = 0; i < n; i++)
    {
      if (pa[i+1] != (pb[i+1] * pc[i+1]))
	abort ();
    }

  return 0;
}

int main (void)
{
  int i;

  check_vect ();

  main1 ();
  main2 ();
  main3 (N-1);

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 3 "vect" } } */
/* { dg-final { scan-tree-dump-times "Vectorizing an unaligned access" 0 "vect" } } */
/* { dg-final { scan-tree-dump-times "Alignment of access forced using peeling" 3 "vect" { xfail vect_element_align_preferred } } } */
/* { dg-final { scan-tree-dump-not "Invalid sum" "optimized" } } */