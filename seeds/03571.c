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
   

#define N 16

_Complex float a[N] =
    { 10.0F + 20.0iF, 11.0F + 21.0iF, 12.0F + 22.0iF, 13.0F + 23.0iF,
      14.0F + 24.0iF, 15.0F + 25.0iF, 16.0F + 26.0iF, 17.0F + 27.0iF,
      18.0F + 28.0iF, 19.0F + 29.0iF, 20.0F + 30.0iF, 21.0F + 31.0iF,
      22.0F + 32.0iF, 23.0F + 33.0iF, 24.0F + 34.0iF, 25.0F + 35.0iF };
_Complex float b[N] =
    { 30.0F + 40.0iF, 31.0F + 41.0iF, 32.0F + 42.0iF, 33.0F + 43.0iF,
      34.0F + 44.0iF, 35.0F + 45.0iF, 36.0F + 46.0iF, 37.0F + 47.0iF,
      38.0F + 48.0iF, 39.0F + 49.0iF, 40.0F + 50.0iF, 41.0F + 51.0iF,
      42.0F + 52.0iF, 43.0F + 53.0iF, 44.0F + 54.0iF, 45.0F + 55.0iF };

_Complex float c[N];
_Complex float res[N] =
    { -500.0F + 1000.0iF, -520.0F + 1102.0iF,
      -540.0F + 1208.0iF, -560.0F + 1318.0iF,
      -580.0F + 1432.0iF, -600.0F + 1550.0iF,
      -620.0F + 1672.0iF, -640.0F + 1798.0iF,
      -660.0F + 1928.0iF, -680.0F + 2062.0iF,
      -700.0F + 2200.0iF, -720.0F + 2342.0iF,
      -740.0F + 2488.0iF, -760.0F + 2638.0iF,
      -780.0F + 2792.0iF, -800.0F + 2950.0iF };


__attribute__ ((noinline)) void
foo (void)
{
  int i;

  for (i = 0; i < N; i++)
    c[i] = a[i] * b[i];

}

int
main (void)
{ 
  int i;
  check_vect ();
  
  foo ();

  /* check results:  */
#pragma GCC novector
  for (i = 0; i < N; i++)
  {
    if (c[i] != res[i])
      abort ();
  }

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" { target vect_strided2 } } } */
