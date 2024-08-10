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
   

#define N 128 

__attribute__ ((noinline)) int
main1 (unsigned short a0, unsigned short a1, unsigned short a2, 
       unsigned short a3, unsigned short a4, unsigned short a5,
       unsigned short a6, unsigned short a7, unsigned short a8,
       unsigned short a9, unsigned short a10, unsigned short a11,
       unsigned short a12, unsigned short a13, unsigned short a14,
       unsigned short a15, unsigned char b0, unsigned char b1)
{
  int i;
  unsigned short out[N*16];
  unsigned char out2[N*16];

  for (i = 0; i < N; i++)
    {
      out[i*16] = a8;
      out[i*16 + 1] = a7;
      out[i*16 + 2] = a1;
      out[i*16 + 3] = a2;
      out[i*16 + 4] = a8;
      out[i*16 + 5] = a5;
      out[i*16 + 6] = a5;
      out[i*16 + 7] = a4;
      out[i*16 + 8] = a12;
      out[i*16 + 9] = a13;
      out[i*16 + 10] = a14;
      out[i*16 + 11] = a15;
      out[i*16 + 12] = a6;
      out[i*16 + 13] = a9;
      out[i*16 + 14] = a0;
      out[i*16 + 15] = a7;

      out2[i*2] = b1;
      out2[i*2+1] = b0;
    }

  /* check results:  */
#pragma GCC novector
  for (i = 0; i < N; i++)
    {
      if (out[i*16] != a8
          || out[i*16 + 1] != a7
          || out[i*16 + 2] != a1
          || out[i*16 + 3] != a2
          || out[i*16 + 4] != a8
          || out[i*16 + 5] != a5
          || out[i*16 + 6] != a5
          || out[i*16 + 7] != a4
          || out[i*16 + 8] != a12
          || out[i*16 + 9] != a13
          || out[i*16 + 10] != a14
          || out[i*16 + 11] != a15
          || out[i*16 + 12] != a6
          || out[i*16 + 13] != a9
          || out[i*16 + 14] != a0
          || out[i*16 + 15] != a7
          || out2[i*2] != b1
          || out2[i*2 + 1] != b0)
        abort ();
    }

  return 0;
}

int main (void)
{
  check_vect ();

  main1 (15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,20,21);

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect"  } } */
/* { dg-final { scan-tree-dump-times "vectorizing stmts using SLP" 2 "vect"  } } */
  
