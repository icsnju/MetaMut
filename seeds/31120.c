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

int
main1 ()
{
  int i;
  unsigned short out[N*8];
  unsigned short in[N*8];
  unsigned int in2[N*8];
  unsigned int out2[N*8];

  for (i = 0; i < N*8; i++)
    {
      in[i] = in2[i] = i;
      asm volatile ("" ::: "memory");
    }

  /* Induction is SLPable.  */
  for (i = 0; i < N; i++)
    {
      out[i*8] = in[i*8] + i;
      out[i*8 + 1] = in[i*8 + 1] + i;
      out[i*8 + 2] = in[i*8 + 2] + i;
      out[i*8 + 3] = in[i*8 + 3] + i;
      out[i*8 + 4] = in[i*8 + 4] + i;
      out[i*8 + 5] = in[i*8 + 5] + i;
      out[i*8 + 6] = in[i*8 + 6] + i;
      out[i*8 + 7] = in[i*8 + 7] + i;
    }

  /* check results:  */
#pragma GCC novector
  for (i = 0; i < N; i++)
    {
      if (out[i*8] !=  in[i*8] + i
         || out[i*8 + 1] != in[i*8 + 1] + i
         || out[i*8 + 2] != in[i*8 + 2] + i
         || out[i*8 + 3] != in[i*8 + 3] + i
         || out[i*8 + 4] != in[i*8 + 4] + i
         || out[i*8 + 5] != in[i*8 + 5] + i
         || out[i*8 + 6] != in[i*8 + 6] + i
         || out[i*8 + 7] != in[i*8 + 7] + i)
	abort ();
    }

  /* Induction is SLPable.  */
  for (i = 0; i < N/2; i++)
    {
      out2[i*12] = in2[i*12] + i;
      out2[i*12 + 1] = in2[i*12 + 1] + i;
      out2[i*12 + 2] = in2[i*12 + 2] + i;
      out2[i*12 + 3] = in2[i*12 + 3] + i;
      out2[i*12 + 4] = in2[i*12 + 4] + i;
      out2[i*12 + 5] = in2[i*12 + 5] + i;
      out2[i*12 + 6] = in2[i*12 + 6] + i;
      out2[i*12 + 7] = in2[i*12 + 7] + i;
      out2[i*12 + 8] = in2[i*12 + 8] + i;
      out2[i*12 + 9] = in2[i*12 + 9] + i;
      out2[i*12 + 10] = in2[i*12 + 10] + i;
      out2[i*12 + 11] = in2[i*12 + 11] + i;
    }

  /* check results:  */
#pragma GCC novector
  for (i = 0; i < N/2; i++)
    {
        if (out2[i*12] != in2[i*12] + i
            || out2[i*12 + 1] != in2[i*12 + 1] + i
            || out2[i*12 + 2] != in2[i*12 + 2] + i
            || out2[i*12 + 3] != in2[i*12 + 3] + i
            || out2[i*12 + 4] != in2[i*12 + 4] + i
            || out2[i*12 + 5] != in2[i*12 + 5] + i
            || out2[i*12 + 6] != in2[i*12 + 6] + i
            || out2[i*12 + 7] != in2[i*12 + 7] + i
            || out2[i*12 + 8] != in2[i*12 + 8] + i
            || out2[i*12 + 9] != in2[i*12 + 9] + i
            || out2[i*12 + 10] != in2[i*12 + 10] + i
            || out2[i*12 + 11] != in2[i*12 + 11] + i)
          abort ();
    }

  /* Not power of 2 but SLPable.  */
  for (i = 0; i < N/2; i++)
    {
      out2[i*12] = in2[i*12] + 1;
      out2[i*12 + 1] = in2[i*12 + 1] + 2;
      out2[i*12 + 2] = in2[i*12 + 2] + 3;
      out2[i*12 + 3] = in2[i*12 + 3] + 4;
      out2[i*12 + 4] = in2[i*12 + 4] + 5;
      out2[i*12 + 5] = in2[i*12 + 5] + 6;
      out2[i*12 + 6] = in2[i*12 + 6] + 7;
      out2[i*12 + 7] = in2[i*12 + 7] + 8;
      out2[i*12 + 8] = in2[i*12 + 8] + 9;
      out2[i*12 + 9] = in2[i*12 + 9] + 10;
      out2[i*12 + 10] = in2[i*12 + 10] + 11;
      out2[i*12 + 11] = in2[i*12 + 11] + 12;
    }

  /* check results:  */
#pragma GCC novector
  for (i = 0; i < N/2; i++)
    {
        if (out2[i*12] != in2[i*12] + 1
            || out2[i*12 + 1] != in2[i*12 + 1] + 2
            || out2[i*12 + 2] != in2[i*12 + 2] + 3
            || out2[i*12 + 3] != in2[i*12 + 3] + 4
            || out2[i*12 + 4] != in2[i*12 + 4] + 5
            || out2[i*12 + 5] != in2[i*12 + 5] + 6
            || out2[i*12 + 6] != in2[i*12 + 6] + 7
            || out2[i*12 + 7] != in2[i*12 + 7] + 8
            || out2[i*12 + 8] != in2[i*12 + 8] + 9
            || out2[i*12 + 9] != in2[i*12 + 9] + 10
            || out2[i*12 + 10] != in2[i*12 + 10] + 11
            || out2[i*12 + 11] != in2[i*12 + 11] + 12)
          abort ();
    }



  return 0;
}

int main (void)
{
  check_vect ();

  main1 ();

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 2 loops" 1 "vect" { target { { vect_interleave && vect_extract_even_odd } && { ! vect_pack_trunc } } } } } */
/* { dg-final { scan-tree-dump-times "vectorizing stmts using SLP" 2 "vect" { target { ! vect_pack_trunc } } } } */
/* { dg-final { scan-tree-dump-times "vectorized 3 loops" 1 "vect" { target { { vect_interleave && vect_extract_even_odd } && vect_pack_trunc } } } } */
/* { dg-final { scan-tree-dump-times "vectorizing stmts using SLP" 3 "vect" { target vect_pack_trunc xfail vect_variable_length } } } */
