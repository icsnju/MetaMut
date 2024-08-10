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
   

#define N 50

typedef struct {
  short a;
  short b;
} data;

data in1[N], in2[N], out[N];
short result[N*2] = {7,-7,9,-6,11,-5,13,-4,15,-3,17,-2,19,-1,21,0,23,1,25,2,27,3,29,4,31,5,33,6,35,7,37,8,39,9,41,10,43,11,45,12,47,13,49,14,51,15,53,16,55,17,57,18,59,19,61,20,63,21,65,22,67,23,69,24,71,25,73,26,75,27,77,28,79,29,81,30,83,31,85,32,87,33,89,34,91,35,93,36,95,37,97,38,99,39,101,40,103,41,105,42};
short out1[N], out2[N];

__attribute__ ((noinline)) void
foo ()
{
  int i;
  short c, d;

  /* Vectorizable with conditional store sinking.  */
  for (i = 0; i < N; i++)
    {
      c = in1[i].b;
      d = in2[i].b;

      if (c >= d)
        {
          out[i].b = c;
          out[i].a = d + 5;
        }
      else
        {
          out[i].b = d - 12;
          out[i].a = c + d;
        }
    }

  /* Only vectorizable with masked stores.  */
  for (i = 0; i < N; i++)
    {
      c = in1[i].b;
      d = in2[i].b;

      if (c >= d)
        {
          out1[i] = c;
        }
      else
        {
          out2[i] = c + d;
        }
    }
}

int
main (void)
{
  int i;

  check_vect ();

  for (i = 0; i < N; i++)
    {
      in1[i].a = i;
      in1[i].b = i + 2;
      in2[i].a = 5;
      in2[i].b = i + 5;
      __asm__ volatile ("");
    }

  foo ();

#pragma GCC novector
  for (i = 0; i < N; i++)
    {
      if (out[i].a != result[2*i] || out[i].b != result[2*i+1])
        abort ();
    }

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" { target { ! vect_masked_store } xfail { { vect_no_align && { ! vect_hw_misalign } } || { ! vect_strided2 } } } } } */
/* { dg-final { scan-tree-dump-times "vectorized 2 loops" 1 "vect" { target { vect_masked_store } } } } */
