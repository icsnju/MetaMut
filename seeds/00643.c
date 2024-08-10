/* { dg-additional-options "-fopenmp-simd -fno-tree-vectorize" } */
/* { dg-additional-options "-mavx" { target avx_runtime } } */
/* { dg-final { scan-tree-dump "vectorized 1\[1-2] loops" "vect" { target i?86-*-* x86_64-*-* } } } */

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
   

int x, i, j;
volatile int a, b, c, d, e, f, g, h;
int k[11][101];

__attribute__((noipa)) void
doit (void)
{
  int niters, err = 0;
  for (i = 1; i <= 10; i++)
    for (j = 1; j <= 10 * i; j++)
      {
	k[i][j] = 1;
	asm volatile ("" : : : "memory");
      }
  a = 1; b = 11; c = 1; d = 0; e = 1; f = 10; g = 1; h = 1;
  niters = 0; i = -100; j = -100; x = -100;
  #pragma omp simd collapse(2) lastprivate (i, j, x) reduction(+:niters) reduction(|:err)
  for (i = 1; i <= 10; i++)
    for (j = 1; j <= 10 * i; j++)
      {
	err |= (i < 1);
	err |= (i > 10);
	err |= (j < 1);
	err |= (j > 10 * i);
	err |= (k[i][j] != 1);
	k[i][j]++;
	x = i * 1024 + (j & 1023);
	niters++;
      }
  if (i != 11 || j != 101 || x != 10340 || niters != 550 || err)
    abort ();
  niters = 0; i = -100; j = -100; x = -100;
  #pragma omp simd collapse(2) lastprivate (i, j, x) reduction(+:niters) reduction(|:err)
  for (i = a; i < b; i += c)
    for (j = d * i + e; j < g + i * f; j += h)
      {
	err |= (i < 1);
	err |= (i > 10);
	err |= (j < 1);
	err |= (j > 10 * i);
	err |= (k[i][j] != 2);
	k[i][j]++;
	x = i * 1024 + (j & 1023);
	niters++;
      }
  if (i != 11 || j != 101 || x != 10340 || niters != 550 || err)
    abort ();
  for (i = 1; i <= 10; i++)
#pragma GCC novector
    for (j = 1; j <= 10 * i; j++)
      if (k[i][j] == 3)
	k[i][j] = 0;
      else
	abort ();
  for (i = 0; i < 11; i++)
#pragma GCC novector
    for (j = 0; j < 101; j++)
      if (k[i][j] != 0)
	abort ();
  for (i = 0; i < 10; i++)
    for (j = 0; j < 10 * i; j++)
      {
        k[i][j] = 1;
	asm volatile ("" : : : "memory");
      }
  a = 0; b = 10; c = 1; d = 0; e = 0; f = 10; g = 0; h = 1;
  niters = 0; i = -100; j = -100; x = -100;
  #pragma omp simd collapse(2) lastprivate (i, j, x) reduction(+:niters) reduction(|:err)
  for (i = 0; i < 10; i++)
    for (j = 0; j < 10 * i; j++)
      {
	err |= (i < 0);
	err |= (i >= 10);
	err |= (j < 0);
	err |= (j >= 10 * i);
	err |= (k[i][j] != 1);
	k[i][j]++;
	x = i * 1024 + (j & 1023);
	niters++;
      }
  if (i != 10 || j != 90 || x != 9305 || niters != 450 || err)
    abort ();
  niters = 0; i = -100; j = -100; x = -100;
  #pragma omp simd collapse(2) lastprivate (i, j, x) reduction(+:niters) reduction(|:err)
  for (i = a; i < b; i += c)
    for (j = d * i + e; j < g + i * f; j += h)
      {
	err |= (i < 0);
	err |= (i >= 10);
	err |= (j < 0);
	err |= (j >= 10 * i);
	err |= (k[i][j] != 2);
	k[i][j]++;
	x = i * 1024 + (j & 1023);
	niters++;
      }
  if (i != 10 || j != 90 || x != 9305 || niters != 450 || err)
    abort ();
  for (i = 0; i < 10; i++)
#pragma GCC novector
    for (j = 0; j < 10 * i; j++)
      if (k[i][j] == 3)
	k[i][j] = 0;
      else
	abort ();
  for (i = 0; i < 11; i++)
#pragma GCC novector
    for (j = 0; j < 101; j++)
      if (k[i][j] != 0)
	abort ();
  for (i = 4; i < 10; i++)
    for (j = -9 + 2 * i; j < i; j++)
      {
        k[i][j + 1] = 1;
	asm volatile ("" : : : "memory");
      }
  a = 4; b = 10; c = 1; d = 2; e = -9; f = 1; g = 0; h = 1;
  niters = 0; i = -100; j = -100; x = -100;
  #pragma omp simd collapse(2) lastprivate (i, j, x) reduction(+:niters) reduction(|:err)
  for (i = 4; i < 10; i++)
    for (j = -9 + 2 * i; j < i; j++)
      {
	err |= (i < 4);
	err |= (i >= 10);
	err |= (j < -9 + 2 * i);
	err |= (j >= i);
	err |= (k[i][j + 1] != 1);
	k[i][j + 1]++;
	x = i * 1024 + (j & 1023);
	niters++;
      }
  if (/*i != 10 || j != 9 || */x != 8199 || niters != 15 || err)
    abort ();
  niters = 0; i = -100; j = -100; x = -100;
  #pragma omp simd collapse(2) lastprivate (i, j, x) reduction(+:niters) reduction(|:err)
  for (i = a; i < b; i += c)
    for (j = d * i + e; j < g + i * f; j += h)
      {
	err |= (i < 4);
	err |= (i >= 10);
	err |= (j < -9 + 2 * i);
	err |= (j >= i);
	err |= (k[i][j + 1] != 2);
	k[i][j + 1]++;
	x = i * 1024 + (j & 1023);
	niters++;
      }
  if (/*i != 10 || j != 9 || */x != 8199 || niters != 15 || err)
    abort ();
  for (i = 4; i < 10; i++)
    for (j = -9 + 2 * i; j < i; j++)
      if (k[i][j + 1] == 3)
	k[i][j + 1] = 0;
      else
	abort ();
  for (i = 0; i < 11; i++)
#pragma GCC novector
    for (j = 0; j < 101; j++)
      if (k[i][j] != 0)
	abort ();
  for (i = 1; i < 10; i += 2)
    for (j = 1; j < i + 1; j++)
      {
	k[i][j] = 1;
	asm volatile ("" : : : "memory");
      }
  a = 1; b = 10; c = 2; d = 0; e = 1; f = 1; g = 1; h = 1;
  niters = 0; i = -100; j = -100; x = -100;
  #pragma omp simd collapse(2) lastprivate (i, j, x) reduction(+:niters) reduction(|:err)
  for (i = 1; i < 10; i += 2)
    for (j = 1; j < i + 1; j++)
      {
	err |= (i < 1);
	err |= (i >= 10);
	err |= (j < 1);
	err |= (j >= i + 1);
	err |= (k[i][j] != 1);
	k[i][j]++;
	x = i * 1024 + (j & 1023);
	niters++;
      }
  if (i != 11 || j != 10 || x != 9225 || niters != 25 || err)
    abort ();
  niters = 0; i = -100; j = -100; x = -100;
  #pragma omp simd collapse(2) lastprivate (i, j, x) reduction(+:niters) reduction(|:err)
  for (i = a; i < b; i += c)
    for (j = d * i + e; j < g + i * f; j += h)
      {
	err |= (i < 1);
	err |= (i >= 10);
	err |= (j < 1);
	err |= (j >= i + 1);
	err |= (k[i][j] != 2);
	k[i][j]++;
	x = i * 1024 + (j & 1023);
	niters++;
      }
  if (i != 11 || j != 10 || x != 9225 || niters != 25 || err)
    abort ();
  for (i = 1; i < 10; i += 2)
#pragma GCC novector
    for (j = 1; j < i + 1; j++)
      if (k[i][j] == 3)
	k[i][j] = 0;
      else
	abort ();
  for (i = 0; i < 11; i++)
#pragma GCC novector
    for (j = 0; j < 101; j++)
      if (k[i][j] != 0)
	abort ();
  for (j = -11; j >= -41; j -= 15)
    {
      k[0][-j] = 1;
      asm volatile ("" : : : "memory");
    }
  a = 4; b = 8; c = 12; d = -8; e = -9; f = -3; g = 6; h = 15;
  niters = 0; i = -100; j = -100; x = -100;
  #pragma omp simd collapse(2) lastprivate (i, j, x) reduction(+:niters) reduction(|:err)
  for (i = 4; i < 8; i += 12)
    for (j = -8 * i - 9; j < i * -3 + 6; j += 15)
      {
	err |= (i != 4);
	err |= (j < -41);
	err |= (j > -11);
	err |= (k[0][-j] != 1);
	k[0][-j]++;
	x = i * 1024 + (j & 1023);
	niters++;
      }
  if (i != 16 || j != 4 || x != 5109 || niters != 3 || err)
    abort ();
  niters = 0; i = -100; j = -100; x = -100;
  #pragma omp simd collapse(2) lastprivate (i, j, x) reduction(+:niters) reduction(|:err)
  for (i = a; i < b; i += c)
    for (j = d * i + e; j < g + i * f; j += h)
      {
	err |= (i != 4);
	err |= (j < -41);
	err |= (j > -11);
	err |= (k[0][-j] != 2);
	k[0][-j]++;
	x = i * 1024 + (j & 1023);
	niters++;
      }
  if (i != 16 || j != 4 || x != 5109 || niters != 3 || err)
    abort ();
#pragma GCC novector
  for (j = -11; j >= -41; j -= 15)
    if (k[0][-j] == 3)
      k[0][-j] = 0;
    else
      abort ();
#pragma GCC novector
  for (j = -11; j >= -41; j--)
    if (k[0][-j] != 0)
      abort ();
  for (j = -34; j <= -7; j++)
    {
      k[0][-j] = 1;
      asm volatile ("" : : : "memory");
    }
  a = -13; b = 7; c = 12; d = 3; e = 5; f = 0; g = -6; h = 1;
  niters = 0; i = -100; j = -100; x = -100;
  #pragma omp simd collapse(2) lastprivate (i, j, x) reduction(+:niters) reduction(|:err)
  for (i = -13; i < 7; i += 12)
    for (j = 3 * i + 5; j < -6; j++)
      {
	err |= (i != -13);
	err |= (j < -34);
	err |= (j > -7);
	err |= (k[0][-j] != 1);
	k[0][-j]++;
	x = i * 1024 + (j & 1023);
	niters++;
      }
  if (/*i != 11 || j != 2 || */x != -12295 || niters != 28 || err)
    abort ();
  niters = 0; i = -100; j = -100; x = -100;
  #pragma omp simd collapse(2) lastprivate (i, j, x) reduction(+:niters) reduction(|:err)
  for (i = a; i < b; i += c)
    for (j = d * i + e; j < g + i * f; j += h)
      {
	err |= (i != -13);
	err |= (j < -34);
	err |= (j > -7);
	err |= (k[0][-j] != 2);
	k[0][-j]++;
	x = i * 1024 + (j & 1023);
	niters++;
      }
  if (/*i != 11 || j != 2 || */x != -12295 || niters != 28 || err)
    abort ();
#pragma GCC novector
  for (j = -34; j <= -7; j++)
    if (k[0][-j] == 3)
      k[0][-j] = 0;
    else
      abort ();
}

int
main ()
{
  check_vect ();
  doit ();
  return 0;
}
