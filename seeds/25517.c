/* { dg-require-effective-target scalar_all_fma } */
/* { dg-additional-options "-fdump-tree-optimized" } */

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
   

#define N (VECTOR_BITS * 11 / 64 + 3)

#define DEF(INV)					\
  void __attribute__ ((noipa))				\
  f_##INV (double *restrict a, double *restrict b,	\
	   double *restrict c, double *restrict d)	\
  {							\
    for (int i = 0; i < N; ++i)				\
      {							\
	double mb = (INV & 1 ? -b[i] : b[i]);		\
	double mc = c[i];				\
	double md = (INV & 2 ? -d[i] : d[i]);		\
	double fma = __builtin_fma (mb, mc, md);	\
	double truev = (INV & 4 ? -fma : fma);		\
	a[i] = b[i] < 10 ? truev : 10.0;		\
      }							\
  }

#define TEST(INV)					\
  {							\
    f_##INV (a, b, c, d);				\
    _Pragma("GCC novector")				\
    for (int i = 0; i < N; ++i)				\
      {							\
	double mb = (INV & 1 ? -b[i] : b[i]);		\
	double mc = c[i];				\
	double md = (INV & 2 ? -d[i] : d[i]);		\
	double fma = __builtin_fma (mb, mc, md);	\
	double truev = (INV & 4 ? -fma : fma);		\
	if (a[i] != (i % 17 < 10 ? truev : 10.0))	\
	  __builtin_abort ();				\
	asm volatile ("" ::: "memory");			\
      }							\
  }

#define FOR_EACH_INV(T) \
  T (0) T (1) T (2) T (3) T (4) T (5) T (6) T (7)

FOR_EACH_INV (DEF)

int
main (void)
{
  double a[N], b[N], c[N], d[N];
  for (int i = 0; i < N; ++i)
    {
      b[i] = i % 17;
      c[i] = i % 9 + 11;
      d[i] = i % 13 + 14;
      asm volatile ("" ::: "memory");
    }
  FOR_EACH_INV (TEST)
  return 0;
}

/* { dg-final { scan-tree-dump-times { = \.COND_FMA } 2 "optimized" { target vect_double_cond_arith } } } */
/* { dg-final { scan-tree-dump-times { = \.COND_FMS } 2 "optimized" { target vect_double_cond_arith } } } */
/* { dg-final { scan-tree-dump-times { = \.COND_FNMA } 2 "optimized" { target vect_double_cond_arith } } } */
/* { dg-final { scan-tree-dump-times { = \.COND_FNMS } 2 "optimized" { target vect_double_cond_arith } } } */
