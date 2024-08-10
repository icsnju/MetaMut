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
   

#define N 256

__attribute__ ((noinline))
void bar (float *pd, float *pa, float *pb, float *pc) 
{
  int i;

  /* check results:  */
#pragma GCC novector
  for (i = 0; i < N; i++)
    {
      if (pa[i] != (pb[i] * pc[i]))
	abort ();
      if (pd[i] != 5.0)
	abort ();
    }

  return;
}


__attribute__ ((noinline)) int
main1 (int n, float * __restrict__ pd, float * __restrict__ pa, float * __restrict__ pb, float * __restrict__ pc)
{
  int i;

  for (i = 0; i < n; i++)
    {
      pa[i] = pb[i] * pc[i];
      pd[i] = 5.0;
    }

  bar (pd,pa,pb,pc);

  return 0;
}

int main (void)
{
  int i;
  float a[N] __attribute__ ((__aligned__(__BIGGEST_ALIGNMENT__)));
  float d[N+1] __attribute__ ((__aligned__(__BIGGEST_ALIGNMENT__)));
  float b[N] = {0,3,6,9,12,15,18,21,24,27,30,33,36,39,42,45,48,51,54,57};
  float c[N] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};

  check_vect ();

  main1 (N,&d[1],a,b,c);
  main1 (N-2,&d[1],a,b,c);

  return 0;
}

/* For targets that support unaligned loads we version for the two unaligned 
   stores and generate misaligned accesses for the loads. For targets that 
   don't support unaligned loads we version for all four accesses.  */

/* { dg-final { scan-tree-dump-times "Vectorizing an unaligned access" 2 "vect" { xfail { vect_no_align || vect_element_align} } } }  */
/* { dg-final { scan-tree-dump-times "Alignment of access forced using versioning" 2 "vect" { xfail { vect_no_align || vect_element_align } } } } */
/*  { dg-final { scan-tree-dump-times "Vectorizing an unaligned access" 0 "vect" { target { vect_no_align && { ! vect_hw_misalign } } } } } */
/*  { dg-final { scan-tree-dump-times "Alignment of access forced using versioning" 4 "vect" { target { vect_no_align && { ! vect_hw_misalign } } } } } */
/* { dg-final { scan-tree-dump-not "Invalid sum" "optimized" } } */
