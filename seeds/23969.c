/* { dg-require-effective-target vect_cond_mixed } */
/* { dg-require-effective-target vect_float } */
/* { dg-additional-options "-fno-ipa-icf" } */

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
   

#define N 1024
float a[N], b[N], c[N], d[N];
int j[N];
unsigned char k[N];

__attribute__((noinline, noclone)) void
f1 (void)
{
  int i;
  for (i = 0; i < N; ++i)
    {
      unsigned int x = a[i] < b[i] ? -1 : 0;
      unsigned int y = c[i] < d[i] ? -1 : 0;
      j[i] = (x & y) >> 31;
    }
}

__attribute__((noinline, noclone)) void
f2 (void)
{
  int i;
  for (i = 0; i < N; ++i)
    {
      int x = a[i] < b[i];
      int y = c[i] < d[i];
      j[i] = x & y;
    }
}

__attribute__((noinline, noclone)) void
f3 (void)
{
  int i;
  for (i = 0; i < N; ++i)
    j[i] = (a[i] < b[i]) & (c[i] < d[i]);
}

__attribute__((noinline, noclone)) void
f4 (void)
{
  int i;
  for (i = 0; i < N; ++i)
    {
      int x = a[i] < b[i];
      int y = c[i] < d[i];
      k[i] = x & y;
    }
}

__attribute__((noinline, noclone)) void
f5 (void)
{
  int i;
  for (i = 0; i < N; ++i)
    k[i] = (a[i] < b[i]) & (c[i] < d[i]);
}

__attribute__((noinline, noclone)) void
f6 (void)
{
  int i;
  for (i = 0; i < N; ++i)
    {
      unsigned int x = a[i] < b[i] ? -1 : 0;
      unsigned int y = c[i] < d[i] ? -1 : 0;
      j[i] = (x | y) >> 31;
    }
}

__attribute__((noinline, noclone)) void
f7 (void)
{
  int i;
  for (i = 0; i < N; ++i)
    {
      int x = a[i] < b[i];
      int y = c[i] < d[i];
      j[i] = x | y;
    }
}

__attribute__((noinline, noclone)) void
f8 (void)
{
  int i;
  for (i = 0; i < N; ++i)
    j[i] = (a[i] < b[i]) | (c[i] < d[i]);
}

__attribute__((noinline, noclone)) void
f9 (void)
{
  int i;
  for (i = 0; i < N; ++i)
    {
      int x = a[i] < b[i];
      int y = c[i] < d[i];
      k[i] = x | y;
    }
}

__attribute__((noinline, noclone)) void
f10 (void)
{
  int i;
  for (i = 0; i < N; ++i)
    k[i] = (a[i] < b[i]) | (c[i] < d[i]);
}

int
main ()
{
  int i;

  check_vect ();

  for (i = 0; i < N; i++)
    {
      switch (i % 9)
	{
	case 0: asm (""); a[i] = - i - 1; b[i] = i + 1; break;
	case 1: a[i] = 0; b[i] = 0; break;
	case 2: a[i] = i + 1; b[i] = - i - 1; break;
	case 3: a[i] = i; b[i] = i + 7; break;
	case 4: a[i] = i; b[i] = i; break;
	case 5: a[i] = i + 16; b[i] = i + 3; break;
	case 6: a[i] = - i - 5; b[i] = - i; break;
	case 7: a[i] = - i; b[i] = - i; break;
	case 8: a[i] = - i; b[i] = - i - 7; break;
	}
    }
  for (i = 0; i < N; i++)
    {
      switch ((i / 9) % 3)
	{
	case 0: c[i] = a[i / 9]; d[i] = b[i / 9]; break;
	case 1: c[i] = a[i / 9 + 3]; d[i] = b[i / 9 + 3]; break;
	case 2: c[i] = a[i / 9 + 6]; d[i] = b[i / 9 + 6]; break;
	}
    }
  f1 ();
#pragma GCC novector
  for (i = 0; i < N; i++)
    if (j[i] != ((i % 3) == 0 && ((i / 9) % 3) == 0))
      abort ();
  __builtin_memset (j, -6, sizeof (j));
  f2 ();
#pragma GCC novector
  for (i = 0; i < N; i++)
    if (j[i] != ((i % 3) == 0 && ((i / 9) % 3) == 0))
      abort ();
  __builtin_memset (j, -6, sizeof (j));
  f3 ();
#pragma GCC novector
  for (i = 0; i < N; i++)
    if (j[i] != ((i % 3) == 0 && ((i / 9) % 3) == 0))
      abort ();
  __builtin_memset (j, -6, sizeof (j));
  f4 ();
#pragma GCC novector
  for (i = 0; i < N; i++)
    if (k[i] != ((i % 3) == 0 && ((i / 9) % 3) == 0))
      abort ();
  __builtin_memset (k, -6, sizeof (k));
  f5 ();
#pragma GCC novector
  for (i = 0; i < N; i++)
    if (k[i] != ((i % 3) == 0 && ((i / 9) % 3) == 0))
      abort ();
  __builtin_memset (k, -6, sizeof (k));
  f6 ();
#pragma GCC novector
  for (i = 0; i < N; i++)
    if (j[i] != ((i % 3) == 0 || ((i / 9) % 3) == 0))
      abort ();
  __builtin_memset (j, -6, sizeof (j));
  f7 ();
#pragma GCC novector
  for (i = 0; i < N; i++)
    if (j[i] != ((i % 3) == 0 || ((i / 9) % 3) == 0))
      abort ();
  __builtin_memset (j, -6, sizeof (j));
  f8 ();
#pragma GCC novector
  for (i = 0; i < N; i++)
    if (j[i] != ((i % 3) == 0 || ((i / 9) % 3) == 0))
      abort ();
  __builtin_memset (j, -6, sizeof (j));
  f9 ();
#pragma GCC novector
  for (i = 0; i < N; i++)
    if (k[i] != ((i % 3) == 0 || ((i / 9) % 3) == 0))
      abort ();
  __builtin_memset (k, -6, sizeof (k));
  f10 ();
#pragma GCC novector
  for (i = 0; i < N; i++)
    if (k[i] != ((i % 3) == 0 || ((i / 9) % 3) == 0))
      abort ();
  __builtin_memset (k, -6, sizeof (k));

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 10 "vect" } } */
