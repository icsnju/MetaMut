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

unsigned int out[N];
unsigned int in1[N] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
unsigned int in2[N] = {10,11,12,13,14,15,16,17,18,19,110,111,112,113,114,115};

__attribute__ ((noinline)) int
main1 (unsigned int x, unsigned int y)
{
  int i;
  unsigned int *pin1 = &in1[0];
  unsigned int *pin2 = &in2[0];
  unsigned int *pout = &out[0];
  unsigned int a0, a1, a2, a3;

  a0 = *pin2++ - *pin1++ + 23;
  a1 = *pin2++ - *pin1++ + 142;
  a2 = *pin2++ - *pin1++ + 2;
  a3 = *pin2++ - *pin1++ + 31;
  
  *pout++ = a0 * x;
  *pout++ = a1 * y;
  *pout++ = a2 * x;
  *pout++ = a3 * y;

  /* Check results.  */
  if (out[0] != (in2[0] - in1[0] + 23) * x
      || out[1] != (in2[1] - in1[1] + 142) * y
      || out[2] != (in2[2] - in1[2] + 2) * x
      || out[3] != (in2[3] - in1[3] + 31) * y)
    abort();

  return 0;
}

int main (void)
{
  check_vect ();

  main1 (2, 3);

  return 0;
}

/* { dg-final { scan-tree-dump-times "optimized: basic block" 1 "slp2" { target vect_int_mult } } } */
  