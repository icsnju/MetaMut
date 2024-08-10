/* PR tree-optimization/51581 */

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
   

#define main main1
/* PR tree-optimization/51581 */
/* { dg-require-effective-target int32plus } */

extern void abort (void);

#define N 4096
int a[N], c[N];
unsigned int b[N], d[N];

__attribute__((noinline, noclone)) void
f1 (void)
{
  int i;
  for (i = 0; i < N; i++)
    c[i] = a[i] % 3;
}

__attribute__((noinline, noclone)) void
f2 (void)
{
  int i;
  for (i = 0; i < N; i++)
    d[i] = b[i] % 3;
}

__attribute__((noinline, noclone)) void
f3 (void)
{
  int i;
  for (i = 0; i < N; i++)
    c[i] = a[i] % 18;
}

__attribute__((noinline, noclone)) void
f4 (void)
{
  int i;
  for (i = 0; i < N; i++)
    d[i] = b[i] % 18;
}

__attribute__((noinline, noclone)) void
f5 (void)
{
  int i;
  for (i = 0; i < N; i++)
    c[i] = a[i] % 19;
}

__attribute__((noinline, noclone)) void
f6 (void)
{
  int i;
  for (i = 0; i < N; i++)
    d[i] = b[i] % 19;
}

#if __SIZEOF_INT__ == 4 && __SIZEOF_LONG_LONG__ == 8
__attribute__((noinline, noclone)) void
f7 (void)
{
  int i;
  for (i = 0; i < N; i++)
    {
      int x = (int) ((unsigned long long) (a[i] * 0x55555556LL) >> 32) - (a[i] >> 31);
      c[i] = a[i] - x * 3;
    }
}

__attribute__((noinline, noclone)) void
f8 (void)
{
  int i;
  for (i = 0; i < N; i++)
    {
      unsigned int x = ((unsigned int) ((b[i] * 0xaaaaaaabULL) >> 32) >> 1);
      d[i] = b[i] - x * 3;
    }
}

__attribute__((noinline, noclone)) void
f9 (void)
{
  int i;
  for (i = 0; i < N; i++)
    {
      int x = (((int) ((unsigned long long) (a[i] * 0x38e38e39LL) >> 32)) >> 2) - (a[i] >> 31);
      c[i] = a[i] - x * 18;
    }
}

__attribute__((noinline, noclone)) void
f10 (void)
{
  int i;
  for (i = 0; i < N; i++)
    {
      unsigned int x = (unsigned int) ((b[i] * 0x38e38e39ULL) >> 32) >> 2;
      d[i] = b[i] - x * 18;
    }
}

__attribute__((noinline, noclone)) void
f11 (void)
{
  int i;
  for (i = 0; i < N; i++)
    {
      int x = (((int) ((unsigned long long) (a[i] * 0x6bca1af3LL) >> 32)) >> 3) - (a[i] >> 31);
      c[i] = a[i] - x * 19;
    }
}

__attribute__((noinline, noclone)) void
f12 (void)
{
  int i;
  for (i = 0; i < N; i++)
    {
      unsigned int tmp = (b[i] * 0xaf286bcbULL) >> 32;
      unsigned int x = (((b[i] - tmp) >> 1) + tmp) >> 4;
      d[i] = b[i] - x * 19;
    }
}
#endif

int
main ()
{
  int i;
  for (i = 0; i < N; i++)
    {
      asm ("");
      a[i] = i - N / 2;
      b[i] = i;
    }
  a[0] = -__INT_MAX__ - 1;
  a[1] = -__INT_MAX__;
  a[N - 1] = __INT_MAX__;
  b[N - 1] = ~0;
  f1 ();
  f2 ();
  for (i = 0; i < N; i++)
    if (c[i] != a[i] % 3 || d[i] != b[i] % 3)
      abort ();
  f3 ();
  f4 ();
  for (i = 0; i < N; i++)
    if (c[i] != a[i] % 18 || d[i] != b[i] % 18)
      abort ();
  f5 ();
  f6 ();
  for (i = 0; i < N; i++)
    if (c[i] != a[i] % 19 || d[i] != b[i] % 19)
      abort ();
#if __SIZEOF_INT__ == 4 && __SIZEOF_LONG_LONG__ == 8
  f7 ();
  f8 ();
  for (i = 0; i < N; i++)
    if (c[i] != a[i] % 3 || d[i] != b[i] % 3)
      abort ();
  f9 ();
  f10 ();
  for (i = 0; i < N; i++)
    if (c[i] != a[i] % 18 || d[i] != b[i] % 18)
      abort ();
  f11 ();
  f12 ();
  for (i = 0; i < N; i++)
    if (c[i] != a[i] % 19 || d[i] != b[i] % 19)
      abort ();
#endif
  return 0;
}
#undef main

int
main ()
{
  int i;
  check_vect ();
  asm ("");
  return main1 ();
}

