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
   

#define M 32
#define N 16

int x_in[M];
int x_out_a[M], x_out_b[M];
int c[N] = {3,2,1,10,1,42,3,4,50,9,32,8,11,10,1,2};
int a[N+1] = {0,16,32,48,64,128,256,512,0,16,32,48,64,128,256,512,1024};
int b[N+1] = {17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
int check_result_a[M] = {1024,1024,1024,256,256,256,256,256,256,256,256,128,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48};
int check_result_b[M] = {17,17,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

__attribute__ ((noinline)) void 
foo ()
{
  int j, i, x;
  int curr_a, flag, next_a, curr_b, next_b;

  for (j = 0; j < M; j++) 
    {
      x = x_in[j];
      curr_a = a[0];
      curr_b = b[0];

      for (i = 0; i < N; i++) 
        {
          flag = x > c[i];
          next_a = a[i+1];
          next_b = b[i+1];
          curr_a = flag ? curr_a : next_a;
          curr_b = flag ? next_b : curr_b;
        }

      x_out_a[j] = curr_a;
      x_out_b[j] = curr_b;
    }
}

int main (void)
{
  int i,j;

  check_vect ();

  for (j = 0; j < M; j++) 
    x_in[j] = j;

  foo ();

#pragma GCC novector
  for (j = 0; j < M; j++)
    if (x_out_a[j] != check_result_a[j]
        || x_out_b[j] != check_result_b[j])
      abort ();

  return 0;
}

/* { dg-final { scan-tree-dump-times "OUTER LOOP VECTORIZED" 1 "vect" { xfail { vect_no_align && { ! vect_hw_misalign } } } } } */


