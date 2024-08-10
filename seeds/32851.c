/* { dg-additional-options "-O3 -fno-fast-math" } */

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
   

double __attribute__((noinline,noclone))
compute_integral (double w_1[18])
{
  double A = 0;
  double t33[2][6] = {{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
	{0.0, 0.0, 0.0, 0.0, 0.0, 0.0}};
  double t43[2] = {0.0, 0.0};
  double t31[2][2] = {{1.0, 1.0}, {1.0, 1.0}};
  double t32[2][3] = {{0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}};

  for (int ip_1 = 0; ip_1 < 2; ++ip_1)
    {
#pragma GCC unroll 0
      for (int i_0 = 0; i_0 < 6; ++i_0)
	t33[ip_1][i_0] = ((w_1[i_0*3] * t32[ip_1][0])
			  + (w_1[i_0*3+2] * t32[ip_1][2]));
      t43[ip_1] = 2.0;
    }
  for (int i_0 = 0; i_0 < 6; ++i_0)
    A += t43[1]*t33[1][i_0];
  return A;
}

int main()
{
  check_vect ();

  double w_1[18] = {0., 1.0, 1.0,
      0., 1.0, 1.0,
      0., 1.0, 1.0,
      0., 1.0, 1.0,
      0., 1.0, 1.0,
      0., 1.0, 1.0};
  double A = compute_integral(w_1);
  if (A != 12.0)
    __builtin_abort ();
  return 0;
}

/* { dg-final { scan-tree-dump-times {using an in-order \(fold-left\) reduction} 1 "vect" { target vect_double } } } */
/* { dg-final { scan-tree-dump-times "vectorized 2 loops" 1 "vect" { target { vect_double && { vect_perm && vect_hw_misalign } } } } } */
