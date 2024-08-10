/* { dg-additional-options "-fdump-tree-optimized-details-blocks" } */
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
typedef int V __attribute__((vector_size (4)));
unsigned int a[N * 2] __attribute__((aligned));
unsigned int b[N * 2] __attribute__((aligned));
V c[N];

__attribute__((noinline, noclone)) unsigned int
foo (unsigned int *a, unsigned int *b)
{
  int i;
  unsigned int r = 0;
  for (i = 0; i < N; i++)
    {
      unsigned int x = a[i], y = b[i];
      if (x < 32)
	{
	  x = x + 127;
	  y = y * 2;
	}
      else
	{
	  x = x - 16;
	  y = y + 1;
	}
      a[i] = x;
      b[i] = y;
      r += x;
    }
  return r;
}

__attribute__((noinline, noclone)) unsigned int
bar (unsigned int *a, unsigned int *b)
{
  int i;
  unsigned int r = 0;
  for (i = 0; i < N; i++)
    {
      unsigned int x = a[i], y = b[i];
      if (x < 32)
	{
	  x = x + 127;
	  y = y * 2;
	}
      else
	{
	  x = x - 16;
	  y = y + 1;
	}
      a[i] = x;
      b[i] = y;
      c[i] = c[i] + 1;
      r += x;
    }
  return r;
}

void
baz (unsigned int *a, unsigned int *b,
     unsigned int (*fn) (unsigned int *, unsigned int *))
{
  int i;
  for (i = -64; i < 0; i++)
    {
      a[i] = 19;
      b[i] = 17;
    }
  for (; i < N; i++)
    {
      a[i] = i - 512;
      b[i] = i;
    }
  for (; i < N + 64; i++)
    {
      a[i] = 27;
      b[i] = 19;
    }
  if (fn (a, b) != -512U - (N - 32) * 16U + 32 * 127U)
    __builtin_abort ();
#pragma GCC novector
  for (i = -64; i < 0; i++)
    if (a[i] != 19 || b[i] != 17)
      __builtin_abort ();
#pragma GCC novector
  for (; i < N; i++)
    if (a[i] != (i - 512U < 32U ? i - 512U + 127 : i - 512U - 16)
	|| b[i] != (i - 512U < 32U ? i * 2U : i + 1U))
      __builtin_abort ();
#pragma GCC novector
  for (; i < N + 64; i++)
    if (a[i] != 27 || b[i] != 19)
      __builtin_abort ();
}

int
main ()
{
  int i;
  check_vect ();
  baz (a + 512, b + 512, foo);
  baz (a + 512, b + 512, bar);
  baz (a + 512 + 1, b + 512 + 1, foo);
  baz (a + 512 + 1, b + 512 + 1, bar);
  baz (a + 512 + 31, b + 512 + 31, foo);
  baz (a + 512 + 31, b + 512 + 31, bar);
  baz (a + 512 + 1, b + 512, foo);
  baz (a + 512 + 1, b + 512, bar);
  baz (a + 512 + 31, b + 512, foo);
  baz (a + 512 + 31, b + 512, bar);
  baz (a + 512, b + 512 + 1, foo);
  baz (a + 512, b + 512 + 1, bar);
  baz (a + 512, b + 512 + 31, foo);
  baz (a + 512, b + 512 + 31, bar);
  return 0;
}

/* { dg-final { scan-tree-dump-not "Invalid sum" "optimized" } } */
