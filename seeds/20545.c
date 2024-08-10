/* Disabling epilogues until we find a better way to deal with scans.  */
/* { dg-additional-options "--param vect-epilogues-nomask=0" } */
/* { dg-require-effective-target vect_long } */
/* { dg-additional-options "-fno-tree-scev-cprop" } */

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
   

/* Statement in SLP vectorization used outside the loop.
   NOTE: SCEV disabled to ensure the live operation is not removed before
   vectorization.  */
#define LIVELOOP(RET) \
__attribute__ ((noinline)) long \
liveloop##RET (int n, long *x, long *y) \
{ \
  long n0, n1, n2, n3; \
  int j; \
  for (j = 0; j < n; ++j) \
    { \
      n0 = x[(j*4)]; \
      n1 = x[(j*4)+1]; \
      n2 = x[(j*4)+2]; \
      n3 = x[(j*4)+3]; \
      y[(j*4)] = n0 + 1; \
      y[(j*4)+1] = n1 + 2; \
      y[(j*4)+2] = n2 + 3; \
      y[(j*4)+3] = n3 + 4; \
    } \
  return n##RET; \
}

LIVELOOP (0)
LIVELOOP (1)
LIVELOOP (2)
LIVELOOP (3)
typedef long (*FP)(int n, long *x, long *y);
const FP llf[]= {&liveloop0, &liveloop1, &liveloop2, &liveloop3};

#define MAX 153

int
main (void)
{
  long a[MAX*4];
  long b[MAX*4];
  int i;

  check_vect ();

  for (i=0; i<MAX*4; i++)
    {
      __asm__ volatile ("");
      a[i] = i;
    }

#pragma GCC novector
  for (i=0; i<4; i++)
    {
      __asm__ volatile ("");

      int ret = llf[i] (MAX, a, b);

      if (ret != (MAX * 4) - 4 + i)
	abort ();

#pragma GCC novector
      for (i=0; i<MAX*4; i++)
	{
	  __asm__ volatile ("");
	  if (b[i] != i + (i%4) + 1)
	    abort ();
	}
    }
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 4 "vect" } } */
/* { dg-final { scan-tree-dump-times "vectorizing stmts using SLP" 4 "vect" { xfail vect_variable_length } } } */
/* { dg-final { scan-tree-dump-times "vec_stmt_relevant_p: stmt live but not relevant" 4 "vect" } } */
