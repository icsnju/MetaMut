/* { dg-require-effective-target vect_simd_clones } */
/* { dg-additional-options "-O3 -fopenmp-simd -ffast-math" } */
#include <stdlib.h>
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
   
#define N 64

float *px, *py;
float *tx, *ty;
float *x1, *z1, *t1, *t2;
int bound[N];

static void inline bar(const float cx, float cy,
                         float *vx, float *vy, int n)
{
  int j;
    for (j = 0; j < n; ++j)
    {
        const float dx  = cx - px[j];
        const float dy  = cy - py[j];
        *vx               -= dx * tx[j];
        *vy               -= dy * ty[j];
    }
}

__attribute__((noinline, noclone)) void foo1 ()
{
  int i;
  int n = bound[63];
#pragma omp simd
  for (i=0; i<N; i++)
    bar(px[i], py[i], x1+i, z1+i, n);
}

__attribute__((noinline, noclone)) void foo2 ()
{
  volatile int i;
  int n = bound[63];
  for (i=0; i<N; i++)
    bar(px[i], py[i], x1+i, z1+i, n);
}


int main()
{
  float *X = (float*)malloc(N * 8 * sizeof (float));
  int i;
  /* check_vect (); */
  px = &X[0];
  py = &X[N * 1];
  tx = &X[N * 2];
  ty = &X[N * 3];
  x1 = &X[N * 4];
  z1 = &X[N * 5];
  t1 = &X[N * 6];
  t2 = &X[N * 7];

  for (i=0; i<N; i++)
    {
      px[i] = (float) (i+2);
      tx[i] = (float) (i+1);
      py[i] = (float) (i+4);
      ty[i] = (float) (i+3);
      x1[i] = z1[i] = 1.0f;
      bound[i] = i + 1;
    }
  foo1 ();  /* vector variant.  */
  for (i=0; i<N;i++)
    {
      t1[i] = x1[i]; x1[i] = 1.0f;
      t2[i] = z1[i]; z1[i] = 1.0f;
    }
  foo2 ();  /* scalar variant.  */
#pragma GCC novector
  for (i=0; i<N; i++)
    if (x1[i] != t1[i] || z1[i] != t2[i])
      abort ();
  return 0;
}
/* { dg-final { scan-tree-dump "OUTER LOOP VECTORIZED" "vect" } } */
