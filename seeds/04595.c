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
   

#define N 128

/* Comparison in short, then/else and result in int.  */
static inline int
foo (short x, short y, int a, int b)
{
  if (x >= y)
    return a;
  else
    return b;
}

__attribute__((noinline, noclone)) void
bar (short * __restrict__ a, short * __restrict__ b,
     short * __restrict__ c, short * __restrict__ d,
     int * __restrict__ e, int w)
{
  int i;
  int stride = 16;

  for (i = 0; i < N/stride; i++, a += stride, b += stride, c += stride,
                                d += stride, e += stride)
    {
      e[0] = foo (c[0], d[0], a[0], b[0]);
      e[1] = foo (c[1], d[1], a[1], b[1]);
      e[2] = foo (c[2], d[2], a[2], b[2]);
      e[3] = foo (c[3], d[3], a[3], b[3]);
      e[4] = foo (c[4], d[4], a[4], b[4]);
      e[5] = foo (c[5], d[5], a[5], b[5]);
      e[6] = foo (c[6], d[6], a[6], b[6]);
      e[7] = foo (c[7], d[7], a[7], b[7]);
      e[8] = foo (c[8], d[8], a[8], b[8]);
      e[9] = foo (c[9], d[9], a[9], b[9]);
      e[10] = foo (c[10], d[10], a[10], b[10]);
      e[11] = foo (c[11], d[11], a[11], b[11]);
      e[12] = foo (c[12], d[12], a[12], b[12]);
      e[13] = foo (c[13], d[13], a[13], b[13]);
      e[14] = foo (c[14], d[14], a[14], b[14]);
      e[15] = foo (c[15], d[15], a[15], b[15]);
    }
}


short a[N], b[N], c[N], d[N];
int e[N];

int main ()
{
  int i;

  check_vect ();

  for (i = 0; i < N; i++)
    {
      a[i] = i;
      b[i] = 5;
      e[i] = 0;

      switch (i % 9)
        {
        case 0: asm (""); c[i] = - i - 1; d[i] = i + 1; break;
        case 1: c[i] = 0; d[i] = 0; break;
        case 2: c[i] = i + 1; d[i] = - i - 1; break;
        case 3: c[i] = i; d[i] = i + 7; break;
        case 4: c[i] = i; d[i] = i; break;
        case 5: c[i] = i + 16; d[i] = i + 3; break;
        case 6: c[i] = - i - 5; d[i] = - i; break;
        case 7: c[i] = - i; d[i] = - i; break;
        case 8: c[i] = - i; d[i] = - i - 7; break;
        }
    }

  bar (a, b, c, d, e, 2);
#pragma GCC novector
  for (i = 0; i < N; i++)
    if (e[i] != ((i % 3) == 0 ? 5 : i))
      abort ();

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorizing stmts using SLP" 1 "vect" } } */
