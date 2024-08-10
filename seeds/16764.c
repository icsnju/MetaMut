/* { dg-require-effective-target vect_cond_mixed } */
/* { dg-require-effective-target vect_float } */

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
   

#define N 128
int d[N], e[N], f[N];
unsigned char k[N];
float a[N], b[N];

__attribute__((noinline, noclone)) void
f1 (void)
{
  int i;
  for (i = 0; i < N/4; i++)
    {
      k[4*i] = a[4*i] < b[4*i] ? 17 : 0;
      k[4*i+1] = a[4*i+1] < b[4*i+1] ? 17 : 0;
      k[4*i+2] = a[4*i+2] < b[4*i+2] ? 17 : 0;
      k[4*i+3] = a[4*i+3] < b[4*i+3] ? 17 : 0;
    }
}

__attribute__((noinline, noclone)) void
f2 (void)
{
  int i;
  for (i = 0; i < N/2; ++i)
    {
      k[2*i] = a[2*i] < b[2*i] ? 0 : 24;
      k[2*i+1] = a[2*i+1] < b[2*i+1] ? 7 : 4;
    }
}

__attribute__((noinline, noclone)) void
f3 (void)
{
  int i;
  for (i = 0; i < N/2; ++i)
    {
      k[2*i] = a[2*i] < b[2*i] ? 51 : 12;
      k[2*i+1] = a[2*i+1] > b[2*i+1] ? 51 : 12;
    }
}

__attribute__((noinline, noclone)) void
f4 (void)
{
  int i;
  for (i = 0; i < N/2; ++i)
    {
      int d0 = d[2*i], e0 = e[2*i];
      int d1 = d[2*i+1], e1 = e[2*i+1];
      f[2*i] = a[2*i] >= b[2*i] ? d0 : e0;
      f[2*i+1] = a[2*i+1] >= b[2*i+1] ? d1 : e1;
    }
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
      d[i] = i;
      e[i] = 2 * i;
    }

  f1 ();
#pragma GCC novector
  for (i = 0; i < N; i++)
    if (k[i] != ((i % 3) == 0 ? 17 : 0))
      abort ();

  f2 ();
#pragma GCC novector
  for (i = 0; i < N; i++)
    {
      switch (i % 9)
        {
        case 0:
	case 6:
	  if (k[i] != ((i/9 % 2) == 0 ? 0 : 7))
	    abort ();
	  break;
        case 1:
        case 5:
        case 7:
	  if (k[i] != ((i/9 % 2) == 0 ? 4 : 24))
            abort ();
          break;
        case 2:
        case 4:
        case 8:
	  if (k[i] != ((i/9 % 2) == 0 ? 24 : 4))
            abort ();
          break;
        case 3:
	  if (k[i] != ((i/9 % 2) == 0 ? 7 : 0))
            abort ();
          break;
        }
    }

  f3 ();

  f4 ();
#pragma GCC novector
  for (i = 0; i < N; i++)
    if (f[i] != ((i % 3) == 0 ? e[i] : d[i]))
      abort ();

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorizing stmts using SLP" 3 "vect" } } */
