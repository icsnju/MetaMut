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

void __attribute__((noipa))
f (int *restrict y, int *restrict x, int *restrict indices)
{
  for (int i = 0; i < N; ++i)
    {
      y[i * 2] = x[indices[i * 2]] + 1;
      y[i * 2 + 1] = x[indices[i * 2 + 1]] + 2;
    }
}

int y[N * 2];
int x[N * 2] = {
  72704, 52152, 51301, 96681,
  57937, 60490, 34504, 60944,
  42225, 28333, 88336, 74300,
  29250, 20484, 38852, 91536,
  86917, 63941, 31590, 21998,
  22419, 26974, 28668, 13968,
  3451, 20247, 44089, 85521,
  22871, 87362, 50555, 85939
};
int indices[N * 2] = {
  15, 16, 9, 19,
  7, 22, 19, 1,
  22, 13, 15, 30,
  5, 12, 11, 11,
  10, 25, 5, 20,
  22, 24, 24, 28,
  30, 19, 6, 4,
  7, 12, 8, 21
};
int expected[N * 2] = {
  91537, 86919, 28334, 22000,
  60945, 28670, 21999, 52154,
  28669, 20486, 91537, 50557,
  60491, 29252, 74301, 74302,
  88337, 20249, 60491, 22421,
  28669, 3453, 3452, 22873,
  50556, 22000, 34505, 57939,
  60945, 29252, 42226, 26976
};

int
main (void)
{
  check_vect ();

  f (y, x, indices);
#pragma GCC novector
  for (int i = 0; i < 32; ++i)
    if (y[i] != expected[i])
      __builtin_abort ();

  return 0;
}

/* { dg-final { scan-tree-dump "Loop contains only SLP stmts" vect { target vect_gather_load_ifn } } } */
