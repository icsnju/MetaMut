/* { dg-require-effective-target vect_int } */

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
   
int arr[2048];
int res[100] = { 2148, 2146, 2144, 2142, 2140, 2138, 2136, 2134, 2132, 2130,
		 2128, 2126, 2124, 2122, 2120, 2118, 2116, 2114, 2112, 2110,
		 2108, 2106, 2104, 2102, 2100, 2098, 2096, 2094, 2092, 2090,
		 2088, 2086, 2084, 2082, 2080, 2078, 2076, 2074, 2072, 2070,
		 2068, 2066, 2064, 2062, 2060, 2058, 2056, 2054, 3078, 2050};

__attribute__ ((noinline)) int
foo (int *a, int *b, int len)
{
  int i;
  int *a1 = a;
  int *a0 = a1 - 4;
  for (i = 0; i < len; i++)
    {
      *b = *a0 + *a1;
      b--;
      a0++;
      a1++;
    }
  return 0;
}

int main (void)
{
  int *a = &arr[1027];
  int *b = &arr[1024];

  int i;
  for (i = 0; i < 2048; i++)
    arr[i] = i;

  foo (a, b, 50);

#pragma GCC novector
  for (i = 975; i < 1025; i++)
    if (arr[i] != res[i - 975])
      abort ();

  return 0;
}

/* { dg-final { scan-tree-dump "improved number of alias checks from \[0-9\]* to 1" "vect" { target vect_perm } } } */
