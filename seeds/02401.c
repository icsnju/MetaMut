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
   

#define A 300
#define N 16

char src[N];
short dst[N];
short src1[N], dst1[N];

void foo (int a)
{
  dst[0] = (short) (a * (int) src[0]);
  dst[1] = (short) (a * (int) src[1]);
  dst[2] = (short) (a * (int) src[2]);
  dst[3] = (short) (a * (int) src[3]);
  dst[4] = (short) (a * (int) src[4]);
  dst[5] = (short) (a * (int) src[5]);
  dst[6] = (short) (a * (int) src[6]);
  dst[7] = (short) (a * (int) src[7]);
  dst[8] = (short) (a * (int) src[8]);
  dst[9] = (short) (a * (int) src[9]);
  dst[10] = (short) (a * (int) src[10]);
  dst[11] = (short) (a * (int) src[11]);
  dst[12] = (short) (a * (int) src[12]);
  dst[13] = (short) (a * (int) src[13]);
  dst[14] = (short) (a * (int) src[14]);
  dst[15] = (short) (a * (int) src[15]);

  dst1[0] += src1[0];
  dst1[1] += src1[1];
  dst1[2] += src1[2];
  dst1[3] += src1[3];
  dst1[4] += src1[4];
  dst1[5] += src1[5];
  dst1[6] += src1[6];
  dst1[7] += src1[7];
}


int main (void)
{
  int i;

  check_vect ();

  for (i = 0; i < N; i++)
    {
      dst[i] = 2;
      dst1[i] = 0;
      src[i] = i;
      src1[i] = i+2;
    }

  foo (A);

#pragma GCC novector
  for (i = 0; i < N; i++)
    {
      if (dst[i] != A * i
          || (i < N/2 && dst1[i] != i + 2))
        abort ();
    }

  return 0;
}

/* { dg-final { scan-tree-dump-times "optimized: basic block" 2 "slp2" { target { vect_int_mult &&  { vect_pack_trunc && vect_unpack } } } } } */

