/* { dg-require-effective-target vect_int } */
/* We now if-convert the loop unconditonally as the memory locations
   are always stored to.  */
/* { dg-additional-options "-fno-tree-loop-if-convert" } */

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
   

#define N 50

int a[N], b[N], in1[N], in2[N];
int result[2*N] = {5,-7,7,-6,9,-5,11,-4,13,-3,15,-2,17,-1,19,0,21,1,23,2,25,3,27,4,29,5,31,6,33,7,35,8,37,9,39,10,41,11,43,12,45,13,47,14,49,15,51,16,53,17,55,18,57,19,59,20,61,21,63,22,65,23,67,24,69,25,71,26,73,27,75,28,77,29,79,30,81,31,83,32,85,33,87,34,89,35,91,36,93,37,95,38,97,39,99,40,101,41,103,42};

__attribute__ ((noinline)) void
foo (int *pa, int *pb)
{
  int i;
  int c, d;

  /* Store sinking should not work here since the pointers may alias.  */
  for (i = 0; i < N; i++)
    {
      c = in1[i];
      d = in2[i];

      if (c >= d)
        {
          *pa = c;
          *pb = d + 5;
        }
      else
        {
          *pb = d - 12;
          *pa = c + d;
        }

      pa++;
      pb++;
    }
}

int
main (void)
{
  int i;

  check_vect ();

  for (i = 0; i < N; i++)
    {
      in1[i] = i;
      in2[i] = i + 5;
      __asm__ volatile ("");
    }

  foo (a, b);

#pragma GCC novector
  for (i = 0; i < N; i++)
    {
      if (a[i] != result[2*i] || b[i] != result[2*i+1])
        abort ();
    }

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 0 "vect"  } } */
