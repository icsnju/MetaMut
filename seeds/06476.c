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
   

#define N 64

int ia[N + 1];
int ib[N + 1];

/* Vectorizable. Dependence distance -1.  */
__attribute__((noinline)) void
f1 (void)
{
  int i;
  for (i = 0; i < N; i++)
    {
      ia[i + 1] = 1;
      ib[i] = ia[i];
    }
}

/* Not vectorizable due to data dependence: dependence distance 1.  */
__attribute__((noinline)) void
f2 (void)
{
  int i;
  for (i = 0; i < N; i++)
    {
      ia[i] = 1;
      ib[i] = ia[i + 1];
    }
}

/* Not vectorizable due to data dependence: dependence distance 1.  */
__attribute__((noinline)) void
f3 (void)
{
  int i;
  for (i = N - 1; i >= 0; i--)
    {
      ia[i + 1] = 1;
      ib[i] = ia[i];
    }
}

/* Vectorizable. Dependence distance -1.  */
__attribute__((noinline)) void
f4 (void)
{
  int i;
  for (i = N - 1; i >= 0; i--)
    {
      ia[i] = 1;
      ib[i] = ia[i + 1];
    }
}

/* Vectorizable. Dependence distance -1.  */
__attribute__((noinline)) void
f5 (void)
{
  int i;
  for (i = 0; i < N; i++)
    {
      ia[i + 1] = 1;
      ia[i] = 2;
    }
}

/* Not vectorizable due to data dependence: dependence distance 1.  */
__attribute__((noinline)) void
f6 (void)
{
  int i;
  for (i = 0; i < N; i++)
    {
      ia[i] = 1;
      ia[i + 1] = 2;
    }
}

/* Not vectorizable due to data dependence: dependence distance 1.  */
__attribute__((noinline)) void
f7 (void)
{
  int i;
  for (i = N - 1; i >= 0; i--)
    {
      ia[i + 1] = 1;
      ia[i] = 2;
    }
}

/* Vectorizable. Dependence distance -1.  */
__attribute__((noinline)) void
f8 (void)
{
  int i;
  for (i = N - 1; i >= 0; i--)
    {
      ia[i] = 1;
      ia[i + 1] = 2;
    }
}

__attribute__ ((noinline)) int
main1 (void)
{
  int i, j;

  for (j = 0; j < 8; j++)
    {
      for (i = 0; i <= N; i++)
	{
	  ia[i] = i + 3;
	  ib[i] = i + N + 3;
	  asm ("");
	}

      switch (j)
	{
	case 0: f1 (); break;
	case 1: f2 (); break;
	case 2: f3 (); break;
	case 3: f4 (); break;
	case 4: f5 (); break;
	case 5: f6 (); break;
	case 6: f7 (); break;
	case 7: f8 (); break;
	}

#pragma GCC novector
      for (i = 0; i <= N; i++)
	{
	  int ea = i + 3;
	  int eb = i + N + 3;
	  switch (j)
	    {
	    case 0:
	      if (i) ea = 1;
	      if (i == 0) eb = 3;
	      else if (i != N) eb = 1;
	      break;
	    case 1:
	      if (i != N) ea = 1;
	      if (i != N) eb = i + 4;
	      break;
	    case 2:
	      if (i) ea = 1;
	      if (i != N) eb = i + 3;
	      break;
	    case 3:
	      if (i != N) ea = 1;
	      if (i < N - 1) eb = 1;
	      else if (i == N - 1) eb = 67;
	      break;
	    case 4:
	      ea = 1 + (i != N);
	      break;
	    case 5:
	      ea = 2 - (i != N);
	      break;
	    case 6:
	      ea = 1 + (i == 0);
	      break;
	    case 7:
	      ea = 2 - (i == 0);
	      break;
	    }
	  if (ia[i] != ea || ib[i] != eb)
	    abort ();
	}
    }

  return 0;
}

int main ()
{
  check_vect ();

  return main1 ();
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 4 "vect" {xfail { vect_no_align && { ! vect_hw_misalign } } } } } */
/* { dg-final { scan-tree-dump-times "dependence distance negative" 4 "vect" } } */
