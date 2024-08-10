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
   

#define N 128 

int
main1 ()
{
  unsigned short i;
  unsigned short out[N*8], out2[N*8], b0, b1, b2, b3, b4, a0, a1, a2, a3, b5;
  unsigned short in[N*8];

  for (i = 0; i < N*8; i++)
    {
      in[i] = i;
    }

  /* Different operations in both cases - vectorization with interleaving.  */
  for (i = 0; i < N; i++)
    {
      a0 = in[i*4];
      a1 = in[i*4 + 1];
      a2 = in[i*4 + 2];
      a3 = in[i*4 + 3];

      b0 = a0 * 8;
      b1 = a1 + 7;
      b2 = a2 + 6;
      b3 = a3 * 5;
      
      b4 = a2 + 4;
      b5 = a3 + 3;

      out[i*4] = b0;
      out[i*4 + 1] = b1;
      out[i*4 + 2] = b2;
      out[i*4 + 3] = b3;

      out2[i*4] = b0;
      out2[i*4 + 1] = b1;
      out2[i*4 + 2] = b4;
      out2[i*4 + 3] = b5;
    }

  /* check results:  */
#pragma GCC novector
  for (i = 0; i < N; i++)
    {
      a0 = in[i*4];
      a1 = in[i*4 + 1];
      a2 = in[i*4 + 2];
      a3 = in[i*4 + 3];

      b0 = a0 * 8;
      b1 = a1 + 7;
      b2 = a2 + 6;
      b3 = a3 * 5;

      b4 = a2 + 4;
      b5 = a3 + 3;

      if (out[i*4] != b0 
         || out[i*4 + 1] != b1
         || out[i*4 + 2] != b2
         || out[i*4 + 3] != b3)
	abort ();

      if (out2[i*4] != b0  
         || out2[i*4 + 1] != b1
         || out2[i*4 + 2] != b4
         || out2[i*4 + 3] != b5)
        abort ();
    }

  /* Different operations in the first case - vectorization with interleaving.  */
  for (i = 0; i < N; i++)
    {
      a0 = in[i*4];
      a1 = in[i*4 + 1];
      a2 = in[i*4 + 2];
      a3 = in[i*4 + 3];

      b0 = a0 + 8;
      b1 = a1 + 7;
      b2 = a2 + 6;
      b3 = a3 * 5;

      b4 = a2 + 4;
      b5 = a3 + 3;

      out[i*4] = b0;
      out[i*4 + 1] = b1;
      out[i*4 + 2] = b2;
      out[i*4 + 3] = b3;

      out2[i*4] = b0;
      out2[i*4 + 1] = b1;
      out2[i*4 + 2] = b4;
      out2[i*4 + 3] = b5;
    }

  /* check results:  */
#pragma GCC novector
  for (i = 0; i < N; i++)
    {
      a0 = in[i*4];
      a1 = in[i*4 + 1];
      a2 = in[i*4 + 2];
      a3 = in[i*4 + 3];

      b0 = a0 + 8;
      b1 = a1 + 7;
      b2 = a2 + 6;
      b3 = a3 * 5;

      b4 = a2 + 4;
      b5 = a3 + 3;

      if (out[i*4] != b0
         || out[i*4 + 1] != b1
         || out[i*4 + 2] != b2
         || out[i*4 + 3] != b3)
        abort ();

      if (out2[i*4] != b0
         || out2[i*4 + 1] != b1
         || out2[i*4 + 2] != b4
         || out2[i*4 + 3] != b5)
        abort ();
    }


  /* Different operations in the second case - vectorization with interleaving.  */
  for (i = 0; i < N; i++) 
    { 
      a0 = in[i*4];
      a1 = in[i*4 + 1];
      a2 = in[i*4 + 2];
      a3 = in[i*4 + 3];

      b0 = a0 + 8;
      b1 = a1 + 7;
      b2 = a2 + 6;
      b3 = a3 + 5;

      b4 = a2 * 4;
      b5 = a3 + 3;

      out[i*4] = b0;
      out[i*4 + 1] = b1;
      out[i*4 + 2] = b2;
      out[i*4 + 3] = b3;

      out2[i*4] = b0;
      out2[i*4 + 1] = b1;
      out2[i*4 + 2] = b4;
      out2[i*4 + 3] = b5;
    }

  /* check results:  */
#pragma GCC novector
  for (i = 0; i < N; i++)
    {
      a0 = in[i*4];
      a1 = in[i*4 + 1];
      a2 = in[i*4 + 2];
      a3 = in[i*4 + 3];

      b0 = a0 + 8;
      b1 = a1 + 7;
      b2 = a2 + 6;
      b3 = a3 + 5;

      b4 = a2 * 4;
      b5 = a3 + 3;

      if (out[i*4] != b0
         || out[i*4 + 1] != b1
         || out[i*4 + 2] != b2
         || out[i*4 + 3] != b3)
        abort ();

      if (out2[i*4] != b0
         || out2[i*4 + 1] != b1
         || out2[i*4 + 2] != b4
         || out2[i*4 + 3] != b5)
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

/* { dg-final { scan-tree-dump-times "vectorized 4 loops" 1 "vect"  { target { vect_strided4 || vect_extract_even_odd } } } } */
/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect"  { target  { ! { vect_strided4 || vect_extract_even_odd } } } } } */
/* Some targets can vectorize the second of the three main loops using
   hybrid SLP.  For 128-bit vectors, the required 4->3 permutations are:

   { 0, 1, 2, 4, 5, 6, 8, 9 }
   { 2, 4, 5, 6, 8, 9, 10, 12 }
   { 5, 6, 8, 9, 10, 12, 13, 14 }

   Not all vect_perm targets support that, and it's a bit too specific to have
   its own effective-target selector, so we just test targets directly.  */
/* { dg-final { scan-tree-dump-times "vectorizing stmts using SLP" 4 "vect" { target { powerpc64*-*-* s390*-*-* } } } } */
/* { dg-final { scan-tree-dump-times "vectorizing stmts using SLP" 2 "vect" { target { vect_strided4 && { ! { powerpc64*-*-* s390*-*-* } } } } } } */
/* { dg-final { scan-tree-dump-times "vectorizing stmts using SLP" 0 "vect"  { target { ! { vect_strided4 } } } } } */
  
