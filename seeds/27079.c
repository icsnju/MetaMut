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
   

#if VECTOR_BITS > 128
#define N (VECTOR_BITS * 2 / 32)
#else
#define N 16
#endif

int
main1 ()
{
  unsigned int i;
  unsigned int out[N*8];
#if N == 16
  unsigned int in[N*8] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63};
#else
  unsigned int in[N * 8];
  for (unsigned int i = 0; i < N * 8; ++i)
    {
      in[i] = i & 63;
      asm volatile ("" ::: "memory");
    }
#endif
  unsigned int ia[N*2], a0, a1, a2, a3;

  /* The last stmt requires interleaving of not power of 2 size - not
     vectorizable.  */
  for (i = 0; i < N/2; i++)
    {
      out[i*12] = in[i*12];
      out[i*12 + 1] = in[i*12 + 1];
      out[i*12 + 2] = in[i*12 + 2];
      out[i*12 + 3] = in[i*12 + 3];
      out[i*12 + 4] = in[i*12 + 4];
      out[i*12 + 5] = in[i*12 + 5];
      out[i*12 + 6] = in[i*12 + 6];
      out[i*12 + 7] = in[i*12 + 7];
      out[i*12 + 8] = in[i*12 + 8];
      out[i*12 + 9] = in[i*12 + 9];
      out[i*12 + 10] = in[i*12 + 10];
      out[i*12 + 11] = in[i*12 + 11];

      ia[i] = in[i*12 + 7];
    }

  /* check results:  */
#pragma GCC novector
  for (i = 0; i < N/2; i++)
    {
      if (out[i*12] !=  in[i*12]
         || out[i*12 + 1] != in[i*12 + 1]
         || out[i*12 + 2] != in[i*12 + 2]
         || out[i*12 + 3] != in[i*12 + 3]
         || out[i*12 + 4] != in[i*12 + 4]
         || out[i*12 + 5] != in[i*12 + 5]
         || out[i*12 + 6] != in[i*12 + 6]
         || out[i*12 + 7] != in[i*12 + 7]
         || out[i*12 + 8] != in[i*12 + 8]
         || out[i*12 + 9] != in[i*12 + 9]
         || out[i*12 + 10] != in[i*12 + 10]
         || out[i*12 + 11] != in[i*12 + 11]
         || ia[i] != in[i*12 + 7])
        abort ();
    }

  /* Hybrid SLP with unrolling by 2.  */
  for (i = 0; i < N; i++)
    {
      out[i*6] = in[i*6];
      out[i*6 + 1] = in[i*6 + 1];
      out[i*6 + 2] = in[i*6 + 2];
      out[i*6 + 3] = in[i*6 + 3];
      out[i*6 + 4] = in[i*6 + 4];
      out[i*6 + 5] = in[i*6 + 5];

      ia[i] = i;
    }

  /* check results:  */
#pragma GCC novector
  for (i = 0; i < N/2; i++)
    {
      if (out[i*6] !=  in[i*6]
         || out[i*6 + 1] != in[i*6 + 1]
         || out[i*6 + 2] != in[i*6 + 2]
         || out[i*6 + 3] != in[i*6 + 3]
         || out[i*6 + 4] != in[i*6 + 4]
         || out[i*6 + 5] != in[i*6 + 5]
         || ia[i] != i)
        abort ();
    }

  return 0;
}

int main (void)
{
  check_vect ();

  main1 ();

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" } } */
/* { dg-final { scan-tree-dump-times "vectorizing stmts using SLP" 1 "vect" } } */
