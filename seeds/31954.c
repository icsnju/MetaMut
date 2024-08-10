/* { dg-require-effective-target vect_double } */

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
   

void __attribute__((noinline,noclone))
foo (double *a4, int n)
{
  for (int i = 0; i < n; ++i)
    {
      /* We may not apply interleaving to the group (a), (b) because of (c).
         Instead group (d) and (b).  */
      double tem1 = a4[i*4] + a4[i*4+n*4] /* (a) */;
      double tem2 = a4[i*4+2*n*4+1];
      a4[i*4+n*4+1] = tem1; /* (c) */
      a4[i*4+1] = tem2;
      double tem3 = a4[i*4] - tem2;
      double tem4 = tem3 + a4[i*4+n*4] /* (d) */;
      a4[i*4+n*4+1] = tem4 + a4[i*4+n*4+1] /* (b) */;
    }
}
int main(int argc, char **argv)
{
  int n = 11;
  double a4[4 * n * 3];
  double a42[4 * n * 3];
  check_vect ();
  for (int i = 0; i < 4 * n * 3; ++i)
    {
      a4[i] = a42[i] = i;
      __asm__ volatile ("": : : "memory");
    }
  foo (a4, n);
  for (int i = 0; i < n; ++i)
    {
      double tem1 = a42[i*4] + a42[i*4+n*4];
      double tem2 = a42[i*4+2*n*4+1];
      a42[i*4+n*4+1] = tem1;
      a42[i*4+1] = tem2;
      double tem3 = a42[i*4] - tem2;
      double tem4 = tem3 + a42[i*4+n*4];
      a42[i*4+n*4+1] = tem4 + a42[i*4+n*4+1];
      __asm__ volatile ("": : : "memory");
    }
#pragma GCC novector
  for (int i = 0; i < 4 * n * 3; ++i)
    if (a4[i] != a42[i])
      __builtin_abort ();
  return 0;
}

/* For v2df we try to use SLP and fail miserably.  */
/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" { target vect_sizes_32B_16B } } } */
