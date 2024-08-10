/* { dg-require-effective-target vect_int } */
/* { dg-additional-options "-fdump-tree-optimized-details-blocks" } */

#include <stdlib.h>
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
   

#define N 16

struct extraction
{
  int a[N][N];
  int b[N][N];
};

static int a[N][N];
static int b[N][N];
static int c[N][N];

__attribute__ ((noinline))
int main1 (int x) {
  int i,j, off;
  struct extraction *p;
  p = (struct extraction *) malloc (sizeof (struct extraction));
  for (i = 0; i < N; i++)
   {
    for (j = 0; j < N; j++)
     {
       a[i][j] = (i*7 + j*17)%53;
       b[i][j] = (i*11+ j*13)%41;
       asm volatile ("" ::: "memory");
     }
   }
  for (i = 0; i < N; i++)
   {
    for (j = 0; j < N; j++)
     {
       c[i][j] = a[i][j];
       asm volatile ("" ::: "memory");
     }
   }
  for (i = 1; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      off = x + i + j + N+1;
      if (x + i + j > N*N-1)
	break;
      if (off > N*N-1)
	*(&c[0][0]+x+i+j) = *(&b[0][0] + off - N*N);
      else
	*(&c[0][0]+x+i+j) = *(&a[0][0] + off);
      asm volatile ("" ::: "memory");
    }
  }

  for (i = 0; i < N; i++)
   {
    for (j = 0; j < N; j++)
     {
       p->a[i][j] = a[i][j];
       p->b[i][j] = b[i][j];
       asm volatile ("" ::: "memory");
     }
   }

  /* Vectorizable: distance > number of iterations.  */
  for (i = 1; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
       *((int *)p + x + i + j) = *((int *)p + x + i + j + N+1);
    }
  }

  /* check results: */
  for (i = 0; i < N; i++)
   {
#pragma GCC novector
    for (j = 0; j < N; j++)
     {
       if (p->a[i][j] != c[i][j])
         abort ();
     }
  }
  return 0;
}

int main (void)
{
  check_vect ();

  return main1 (N);
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" } } */
/* { dg-final { scan-tree-dump-times "Alignment of access forced using versioning" 2 "vect" { target { vect_no_align && { ! vect_hw_misalign } } } } } */
/* { dg-final { scan-tree-dump-times "possible dependence between data-refs" 0 "vect" } } */

/* { dg-final { scan-tree-dump-not "Invalid sum" "optimized" } } */
