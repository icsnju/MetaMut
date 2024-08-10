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
   

#define N 16

struct foostr {
  _Complex short f1;
  _Complex short f2;
};

struct foostr a[16] __attribute__ ((__aligned__(__BIGGEST_ALIGNMENT__))) =
  {
    11 + 23i, 24 + 22i,
    11 + 26i, 24 + 35i,
    19 + 20i, 29 + 14i,
    23 + 31i, 26 + 30i,
    29 + 39i, 24 + 18i,
    20 + 32i, 16 + 23i,
    13 + 26i, 37 + 34i,
    12 + 23i, 26 + 14i,
    36 + 14i, 31 + 17i,
    35 + 17i, 17 + 36i,
    13 + 34i, 19 + 12i,
    27 + 34i, 36 + 19i,
    21 + 39i, 16 + 33i,
    28 + 18i, 39 + 26i,
    32 + 27i, 13 + 38i,
    35 + 36i, 34 + 28i,
  };

struct foostr b[16] __attribute__ ((__aligned__(__BIGGEST_ALIGNMENT__))) =
  {
    37 + 12i, 23 + 15i,
    14 + 11i, 13 + 25i,
    35 + 29i, 22 + 34i,
    24 + 34i, 16 + 39i,
    34 + 32i, 26 + 21i,
    34 + 36i, 11 + 37i,
    25 + 21i, 10 + 39i,
    10 + 36i, 35 + 22i,
    39 + 29i, 23 + 21i,
    34 + 33i, 39 + 14i,
    16 + 31i, 32 + 33i,
    20 + 14i, 35 + 30i,
    26 + 24i, 36 + 37i,
    31 + 20i, 32 + 28i,
    25 + 27i, 15 + 30i,
    10 + 31i, 37 + 37i,
  };
struct foostr c[16] __attribute__ ((__aligned__(__BIGGEST_ALIGNMENT__)));
struct foostr res[N] =
  {
    48 + 35i, 47 + 37i,
    25 + 37i, 37 + 60i,
    54 + 49i, 51 + 48i,
    47 + 65i, 42 + 69i,
    63 + 71i, 50 + 39i,
    54 + 68i, 27 + 60i,
    38 + 47i, 47 + 73i,
    22 + 59i, 61 + 36i,
    75 + 43i, 54 + 38i,
    69 + 50i, 56 + 50i,
    29 + 65i, 51 + 45i,
    47 + 48i, 71 + 49i,
    47 + 63i, 52 + 70i,
    59 + 38i, 71 + 54i,
    57 + 54i, 28 + 68i,
    45 + 67i, 71 + 65i,
  };

__attribute__ ((noinline)) void
foo (void)
{
  int i;

  for (i = 0; i < N; i++)
    {
      c[i].f1 = a[i].f1 + b[i].f1;
      c[i].f2 = a[i].f2 + b[i].f2;
    }

}

int
main (void)
{ 
  int i;
  check_vect ();
  
  foo ();

  /* check results:  */
#pragma GCC novector
  for (i = 0; i < N; i++)
    {
      if (c[i].f1 != res[i].f1)
	abort ();
      if (c[i].f2 != res[i].f2)
	abort ();
    }

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" } } */
/* { dg-final { scan-tree-dump-times "vectorizing stmts using SLP" 1 "vect" } } */
