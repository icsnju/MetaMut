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
short result[N*2] = {10,-7,11,-6,12,-5,13,-4,14,-3,15,-2,16,-1,17,0,18,1,19,2,20,3,21,4,22,5,23,6,24,7,25,8,26,9,27,10,28,11,29,12,30,13,31,14,32,15,33,16,34,17,35,18,36,19,37,20,38,21,39,22,40,23,41,24,42,25,43,26,44,27,45,28,46,29,47,30,48,31,49,32,50,33,51,34,52,35,53,36,54,37,55,38,56,39,57,40,58,41,59,42};
short out1[N], out2[N];

__attribute__ ((noinline)) void
foo ()
{
  int i;
  short c, d;

  for (i = 0; i < N; i++)
    {
      c = in1[i].b;
      d = in2[i].b;

      if (c >= d)
        {
          out[i].b = in1[i].a;
          out[i].a = d + 5;
        }
      else
        {
          out[i].b = d - 12;
          out[i].a = in2[i].a + d;
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

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect"  { xfail { { vect_no_align && { ! vect_hw_misalign } } || { ! vect_strided2 } } } } } */
