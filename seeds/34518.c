/* { dg-require-effective-target vect_condition } */

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
   

extern void abort (void) __attribute__ ((noreturn));

#define N 43

/* Condition reduction with comparison is a different type to the data.  Will
   fail to vectorize.  */

int __attribute__ ((noipa))
condition_reduction (short *a, int min_v, int *b)
{
  int last = N + 65;
  short aval;

  for (int i = 0; i < N; i++)
    {
      aval = a[i];
      if (b[i] < min_v)
	last = aval;
    }
  return last;
}

int
main (void)
{
  short a[N] = {
  31, -32, 133, 324, 335, 36, 37, 45, 11, 65,
  1, -28, 3, 48, 5, -68, 7, 88, 89, 180,
  121, -122, 123, 124, -125, 126, 127, 128, 129, 130,
  11, 12, 13, 14, -15, -16, 17, 18, 19, 20,
  33, 27, 99
  };
  int b[N] = {
  11, -12, -13, 14, 15, 16, 17, 18, 19, 20,
  21, -22, 23, 24, -25, 26, 27, 28, 29, 30,
  1, 62, 3, 14, -15, 6, 37, 48, 99, 10,
  31, -32, 33, 34, -35, 36, 37, 56, 54, 22,
  73, 2, 87
  };

  check_vect ();

  int ret = condition_reduction (a, 16, b);

  if (ret != 27)
    abort ();

  return 0;
}

/* { dg-final { scan-tree-dump "optimizing condition reduction with FOLD_EXTRACT_LAST" "vect" { target vect_fold_extract_last } } } */
/* { dg-final { scan-tree-dump "LOOP VECTORIZED" "vect" { target aarch64*-*-* } } } */
