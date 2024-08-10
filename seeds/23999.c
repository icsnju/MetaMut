/* { dg-require-effective-target vect_int } */
/* { dg-add-options bind_pic_locally } */
/* { dg-additional-options "--param vect-max-peeling-for-alignment=0" } */

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
   

#if VECTOR_BITS > 128
#define NINTS (VECTOR_BITS / 32)
#else
#define NINTS 4
#endif

#define N (NINTS * 6)

struct s{
  int m;
  int n[N][N][N];
};

struct s2{
  int m;
  int n[N-1][N-1][N-1];
};

struct test1{
  struct s a; /* array a.n is unaligned */
  int pad[NINTS - 2];
  struct s e; /* array e.n is aligned */
};

struct test2{
  struct s2 a; /* array a.n is unaligned */
  int b;
  int c;
  struct s2 e; /* array e.n is aligned */
};


struct test1 tmp1[4];
struct test2 tmp2[4];

__attribute__ ((noinline))
int main1 ()
{  
  int i,j;

  /* 1. unaligned (known misalignment) */
  for (i = 0; i < N; i++)
    {
      tmp1[2].a.n[1][2][i] = 5;
    }

  /* check results:  */
#pragma GCC novector
  for (i = 0; i <N; i++)
    {
      if (tmp1[2].a.n[1][2][i] != 5)
        abort ();
    }

  /* 2. aligned */
  for (i = NINTS - 1; i < N - 1; i++)
    {
      tmp1[2].a.n[1][2][i] = 6;
    }

  /* check results:  */
#pragma GCC novector
  for (i = NINTS; i < N - 1; i++)
    {
      if (tmp1[2].a.n[1][2][i] != 6)
        abort ();
    }

  /* 3. aligned */
  for (i = 0; i < N; i++)
    {
      for (j = 0; j < N; j++)
	{
          tmp1[2].e.n[1][i][j] = 8;
	}
    }

  /* check results:  */
#pragma GCC novector
  for (i = 0; i < N; i++)
    {
      for (j = 0; j < N; j++)
	{
          if (tmp1[2].e.n[1][i][j] != 8)
	    abort ();
	}
    }

  /* 4. unaligned (unknown misalignment) */
  for (i = 0; i < N - NINTS; i++)
    {
      for (j = 0; j < N - NINTS; j++)
	{
          tmp2[2].e.n[1][i][j] = 8;
	}
    }

  /* check results:  */
#pragma GCC novector
  for (i = 0; i < N - NINTS; i++)
    {
      for (j = 0; j < N - NINTS; j++)
	{
          if (tmp2[2].e.n[1][i][j] != 8)
	    abort ();
	}
    }

  return 0;
}

int main (void)
{ 
  check_vect ();
  
  return main1 ();
} 

/* { dg-final { scan-tree-dump-times "vectorized 4 loops" 1 "vect" { target vect_hw_misalign } } } */
/* { dg-final { scan-tree-dump-times "vectorized 3 loops" 1 "vect" { target { ! vect_hw_misalign } } } } */
/* { dg-final { scan-tree-dump-times "Alignment of access forced using versioning" 1 "vect" { target { { ! vector_alignment_reachable } && { ! vect_hw_misalign } } } } } */
