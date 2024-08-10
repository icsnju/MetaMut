/* { dg-require-effective-target vect_int } */

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
   

#define N 32

unsigned int ub[N] = {0,3,6,9,12,15,18,21,24,27,30,33,36,39,42,45,
    0,3,6,9,12,15,18,21,24,27,30,33,36,39,42,45};
unsigned int uc[N] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
    0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

/* Vectorization of reduction using loop-aware SLP (with two copies).  */

__attribute__ ((noinline))
int main1 (int n, int res0, int res1, int res2, int res3,
	   int res4, int res5, int res6, int res7)
{
  int i;
  unsigned int udiff0 = 5, udiff1 = 10, udiff2 = 20, udiff3 = 30;
  unsigned int udiff4 = 45, udiff5 = 50, udiff6 = 60, udiff7 = 70;

  for (i = 0; i < n; i++) {
    udiff7 += (ub[8*i + 7] - uc[8*i + 7]);
    udiff6 += (ub[8*i + 6] - uc[8*i + 6]);
    udiff5 += (ub[8*i + 5] - uc[8*i + 5]);
    udiff4 += (ub[8*i + 4] - uc[8*i + 4]);
    udiff3 += (ub[8*i + 3] - uc[8*i + 3]);
    udiff2 += (ub[8*i + 2] - uc[8*i + 2]);
    udiff1 += (ub[8*i + 1] - uc[8*i + 1]);
    udiff0 += (ub[8*i] - uc[8*i]);
  }

  /* Check results:  */
  if (udiff0 != res0
      || udiff1 != res1
      || udiff2 != res2
      || udiff3 != res3
      || udiff4 != res4
      || udiff5 != res5
      || udiff6 != res6
      || udiff7 != res7)
    abort ();

  return 0;
}

int main (void)
{
  check_vect ();

  main1 (N/8, 37, 50, 68, 86, 109, 122, 140, 158);
  main1 (N/8 - 1, 21, 32, 48, 64, 85, 96, 112, 128);
  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" { xfail vect_no_int_add } } } */
/* For variable-length SVE, the number of scalar statements in the
   reduction exceeds the number of elements in a 128-bit granule.  */
/* { dg-final { scan-tree-dump-times "vectorizing stmts using SLP" 1 "vect" { xfail { vect_no_int_add || { { aarch64_sve && vect_variable_length } || { riscv_vector && vect_variable_length } } } } } } */
/* { dg-final { scan-tree-dump-times "VEC_PERM_EXPR" 0 "vect" { xfail { aarch64_sve && vect_variable_length } } } } */
