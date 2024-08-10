/* { dg-require-effective-target vect_int } */
/* { dg-additional-options "--param=vect-epilogues-nomask=0" } */

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
   

#define N 64

#define DOT1 21834 
#define DOT2 21876

unsigned short X[N] __attribute__ ((__aligned__(__BIGGEST_ALIGNMENT__)));
unsigned short Y[N] __attribute__ ((__aligned__(__BIGGEST_ALIGNMENT__)));

/* short->short->int dot product. 
   Not detected as a dot-product pattern.
   Requires support for non-widneing multiplication and widening-summation.  
   Vectorized with loop-aware SLP. */
__attribute__ ((noinline)) unsigned int
foo1(int len, int *result1, int *result2) 
{
  int i;
  unsigned int res1 = 10, res2 = 20;
  unsigned short prod;

  for (i=0; i<len; i++) {
    prod = X[2*i] * Y[2*i];
    res1 += prod;
    prod = X[2*i+1] * Y[2*i+1];
    res2 += prod;
  }

  *result1 = res1;
  *result2 = res2;

  return 0;
}

int main (void)
{
  unsigned int dot1, dot2;
  unsigned short i;

  check_vect ();

  for (i=0; i<N; i++) {
    X[i] = i;
    Y[i] = 64-i;
    asm volatile ("" ::: "memory");
  }

  foo1 (N/2, &dot1, &dot2);

  if (dot1 != DOT1 || dot2 != DOT2)
    abort ();

  return 0;
}

/* The initialization loop in main also gets vectorized.  */
/* { dg-final { scan-tree-dump-times "vect_recog_dot_prod_pattern: detected" 1 "vect" { xfail *-*-* } } } */
/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" { target { vect_short_mult && { vect_widen_sum_hi_to_si  && vect_unpack } } } } } */ 
/* { dg-final { scan-tree-dump-times "vectorizing stmts using SLP" 1 "vect" { xfail { vect_widen_sum_hi_to_si_pattern || { ! { vect_short_mult && { vect_widen_sum_hi_to_si  && vect_unpack } } } } } } } */
/* Check we can elide permutes if SLP vectorizing the reduction.  */
/* { dg-final { scan-tree-dump-times "VEC_PERM_EXPR" 0 "vect" { xfail { { { vect_widen_sum_hi_to_si_pattern || { ! vect_unpack } } && { ! vect_load_lanes } } && { vect_short_mult && { vect_widen_sum_hi_to_si  && vect_unpack } } } } } } */
