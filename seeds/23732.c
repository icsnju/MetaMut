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
   

#if VECTOR_BITS > 512
#define N (VECTOR_BITS * 6 / 16)
#else
#define N 200
#endif

void __attribute__((noinline))
foo (unsigned char *__restrict__ pInput, unsigned char *__restrict__ pOutput)
{
  unsigned char a, b, c;
  unsigned int i;

  for (i = 0; i < N / 3; i++)
    {
       a = *pInput++;
       b = *pInput++;
       c = *pInput++;

       *pOutput++ = a + b + c + 3;
       *pOutput++ = a + b + c + 12;
       *pOutput++ = a + b + c + 1;
    }
}

int main (int argc, const char* argv[])
{
  unsigned char input[N], output[N];
  unsigned char check_results[N];
  unsigned int i;

  check_vect ();

  for (i = 0; i < N; i++)
    {
      input[i] = i;
      output[i] = 0;
      __asm__ volatile ("");
    }

  for (i = 0; i < N / 3; i++)
    {
      check_results[3*i] = 9 * i + 6;
      check_results[3*i+1] = 9 * i + 15;
      check_results[3*i+2] = 9 * i + 4;
      __asm__ volatile ("" : : : "memory");
    }

  foo (input, output);

#pragma GCC novector
  for (i = 0; i < N - (N % 3); i++)
     if (output[i] != check_results[i])
       abort ();

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" { target { vect_perm_byte } } } } */
/* { dg-final { scan-tree-dump-times "vectorizing stmts using SLP" 1 "vect" { target { vect_perm3_byte && { { ! vect_load_lanes } && { { ! vect_partial_vectors_usage_1 } || s390_vx } } } } } } */
/* The epilogues are vectorized using partial vectors.  */
/* { dg-final { scan-tree-dump-times "vectorizing stmts using SLP" 2 "vect" { target { vect_perm3_byte && { { ! vect_load_lanes } && { vect_partial_vectors_usage_1 && { ! s390_vx } } } } } } } */
/* { dg-final { scan-tree-dump-times "vectorizing stmts using SLP" 0 "vect" { target vect_load_lanes } } } */
/* { dg-final { scan-tree-dump "Built SLP cancelled: can use load/store-lanes" "vect" { target { vect_perm3_byte && vect_load_lanes } } } } */
/* { dg-final { scan-tree-dump "LOAD_LANES" "vect" { target vect_load_lanes } } } */
/* { dg-final { scan-tree-dump "STORE_LANES" "vect" { target vect_load_lanes } } } */
