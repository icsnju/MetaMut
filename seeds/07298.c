/* { dg-require-effective-target vect_simd_clones } */
/* { dg-additional-options "-fopenmp-simd" } */
/* { dg-additional-options "-mavx" { target avx_runtime } } */

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
   

#ifndef N
#define N 1024
#endif

int a[N], b[N];
long int c[N];
unsigned char d[N];

#pragma omp declare simd notinbranch
__attribute__((noinline)) static int
foo (long int a, int b, int c)
{
  return a + b + c;
}

#pragma omp declare simd notinbranch
__attribute__((noinline)) static long int
bar (int a, int b, long int c)
{
  return a + b + c;
}

__attribute__((noinline)) void
fn1 (void)
{
  int i;
  #pragma omp simd
  for (i = 0; i < N; i++)
    a[i] = foo (c[i], a[i], b[i]) + 6;
  #pragma omp simd
  for (i = 0; i < N; i++)
    c[i] = bar (a[i], b[i], c[i]) * 2;
}

__attribute__((noinline)) void
fn2 (void)
{
  int i;
  #pragma omp simd
  for (i = 0; i < N; i++)
    {
      a[i] = foo (c[i], a[i], b[i]) + 6;
      d[i]++;
    }
  #pragma omp simd
  for (i = 0; i < N; i++)
    {
      c[i] = bar (a[i], b[i], c[i]) * 2;
      d[i] /= 2;
    }
}

__attribute__((noinline)) void
fn3 (void)
{
  int i;
  for (i = 0; i < N; i++)
    {
      a[i] = i * 2;
      b[i] = 17 + (i % 37);
      c[i] = (i & 63);
      d[i] = 16 + i;
    }
}

int
main ()
{
  int i;
  check_vect ();
  fn3 ();
  fn1 ();
#pragma GCC novector
  for (i = 0; i < N; i++)
    if (a[i] != i * 2 + 23 + (i % 37) + (i & 63)
	|| b[i] != 17 + (i % 37)
	|| c[i] != i * 4 + 80 + 4 * (i % 37) + 4 * (i & 63))
      abort ();
  fn3 ();
  fn2 ();
#pragma GCC novector
  for (i = 0; i < N; i++)
    if (a[i] != i * 2 + 23 + (i % 37) + (i & 63)
	|| b[i] != 17 + (i % 37)
	|| c[i] != i * 4 + 80 + 4 * (i % 37) + 4 * (i & 63)
	|| d[i] != ((unsigned char) (17 + i)) / 2)
      abort ();
  return 0;
}
