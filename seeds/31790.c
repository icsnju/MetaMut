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
   

#define M00 100
#define M10 216
#define M20 23
#define M01 1322
#define M11 13
#define M21 27271
#define M02 74
#define M12 191
#define M22 500

#define K00 405
#define K10 112
#define K01 4322
#define K11 135

#if VECTOR_BITS > 128
#define N (VECTOR_BITS * 3 / 32 + 4)
#else
#define N 16
#endif

/* SLP with load permutation and loop-based vectorization.  */
void foo (int *__restrict__ pInput, int *__restrict__ pOutput,
          int *__restrict__ pInput2, int *__restrict__ pOutput2)
{
  int i, a, b, c, d;

  for (i = 0; i < N / 3; i++)
    {
       a = *pInput++;
       b = *pInput++;
       c = *pInput++;
       d = *pInput2++;

       *pOutput++ = M00 * a + M01 * b + M02 * c;
       *pOutput++ = M10 * a + M11 * b + M12 * c;
       *pOutput++ = M20 * a + M21 * b + M22 * c;

       /* Loop-based vectorization.  */
       *pOutput2++ = K00 * d;
    }
}

int main (int argc, const char* argv[])
{
  int input[N], output[N], i;
  int input2[N], output2[N];

  check_vect ();

  for (i = 0; i < N; i++)
    {
      input[i] = i%256;
      input2[i] = i%256;
      output[i] = 0;
      output2[i] = 0;
      asm volatile ("" ::: "memory");
    }

#if N == 16
  int check_results[N] = {1470, 395, 28271, 5958, 1655, 111653, 10446, 2915, 195035, 14934, 4175, 278417, 19422, 5435, 361799, 0};
  int check_results2[N] = {0, 405, 810, 1215, 1620, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#else
  volatile int check_results[N] = {};
  volatile int check_results2[N] = {};

  for (int i = 0; i < N / 3; i++)
    {
      int a = input[i * 3];
      int b = input[i * 3 + 1];
      int c = input[i * 3 + 2];
      int d = input2[i];

      check_results[i * 3] = M00 * a + M01 * b + M02 * c;
      check_results[i * 3 + 1] = M10 * a + M11 * b + M12 * c;
      check_results[i * 3 + 2] = M20 * a + M21 * b + M22 * c;

      check_results2[i] = K00 * d;

      asm volatile ("" ::: "memory");
    }
#endif

  foo (input, output, input2, output2);

#pragma GCC novector
  for (i = 0; i < N; i++)
     if (output[i] != check_results[i] || output2[i] != check_results2[i])
       abort ();

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect"  { target vect_perm } } } */
/* { dg-final { scan-tree-dump-times "vectorizing stmts using SLP" 1 "vect" { target { vect_perm3_int && { ! vect_load_lanes } } } } } */
/* { dg-final { scan-tree-dump-times "vectorizing stmts using SLP" 0 "vect" { target vect_load_lanes } } } */
/* { dg-final { scan-tree-dump "Built SLP cancelled: can use load/store-lanes" "vect" { target { vect_perm3_int && vect_load_lanes } } } } */
/* { dg-final { scan-tree-dump "LOAD_LANES" "vect" { target vect_load_lanes } } } */
/* { dg-final { scan-tree-dump "STORE_LANES" "vect" { target vect_load_lanes } } } */
