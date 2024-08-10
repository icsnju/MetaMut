/* PR tree-optimization/70354 */

#ifndef main
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
   
#endif

long long int b[64], c[64], g[64];
unsigned long long int a[64], d[64], e[64], f[64], h[64];

__attribute__ ((noinline, noclone)) void
foo (void)
{
  int i;
  for (i = 0; i < 64; i++)
    {
      d[i] = h[i] << (((((unsigned long long int) b[i] * e[i])
			<< (-a[i] - 3752448776177690134ULL))
		       - 8214565720323784703ULL) - 1ULL);
      e[i] = (_Bool) (f[i] + (unsigned long long int) g[i]);
      g[i] = c[i];
    }
}

int
main ()
{
  int i;
#ifndef main
  check_vect ();
#endif
  if (__CHAR_BIT__ != 8 || sizeof (long long int) != 8)
    return 0;
  for (i = 0; i < 64; ++i)
    {
      a[i] = 14694295297531861425ULL;
      b[i] = -1725558902283030715LL;
      c[i] = 4402992416302558097LL;
      e[i] = 6297173129107286501ULL;
      f[i] = 13865724171235650855ULL;
      g[i] = 982871027473857427LL;
      h[i] = 8193845517487445944ULL;
    }
  foo ();
#pragma GCC novector
  for (i = 0; i < 64; i++)
    if (d[i] != 8193845517487445944ULL || e[i] != 1
	|| g[i] != 4402992416302558097ULL)
      abort ();
  return 0;
}
