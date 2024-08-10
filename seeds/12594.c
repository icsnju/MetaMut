/* { dg-require-effective-target vect_int } */
/* { dg-require-effective-target vect_perm } */
/* { dg-additional-options "--param tree-reassoc-width=1" } */

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
   

#define M00 100
#define M10 216
#define M20 23
#define M30 237
#define M40 437

#define M01 1322
#define M11 13
#define M21 27271
#define M31 2280
#define M41 284

#define M02 74
#define M12 191
#define M22 500
#define M32 111
#define M42 1114

#define M03 134
#define M13 117
#define M23 11
#define M33 771
#define M43 71

#define M04 334
#define M14 147
#define M24 115
#define M34 7716
#define M44 16

#if VECTOR_BITS > 128
#define N (VECTOR_BITS * 5 / 32)
#else
#define N 20
#endif

void foo (unsigned int *__restrict__ pInput,
          unsigned int *__restrict__ pOutput,
          unsigned int *__restrict__ pInput2,
          unsigned int *__restrict__ pOutput2)
{
  unsigned int i, a, b, c, d, e;

  for (i = 0; i < N / 5; i++)
    {
       a = *pInput++;
       b = *pInput++;
       c = *pInput++;
       d = *pInput++;
       e = *pInput++;

       *pOutput++ = M00 * a + M01 * b + M02 * c + M03 * d + M04 * e;
       *pOutput++ = M10 * a + M11 * b + M12 * c + M13 * d + M14 * e;
       *pOutput++ = M20 * a + M21 * b + M22 * c + M23 * d + M24 * e;
       *pOutput++ = M30 * a + M31 * b + M32 * c + M33 * d + M34 * e;
       *pOutput++ = M40 * a + M41 * b + M42 * c + M43 * d + M44 * e;


       a = *pInput2++;
       b = *pInput2++;
       c = *pInput2++;
       d = *pInput2++;
       e = *pInput2++;

       *pOutput2++ = M00 * a + M01 * b + M02 * c + M03 * d + M04 * e;
       *pOutput2++ = M10 * a + M11 * b + M12 * c + M13 * d + M14 * e;
       *pOutput2++ = M20 * a + M21 * b + M22 * c + M23 * d + M24 * e;
       *pOutput2++ = M30 * a + M31 * b + M32 * c + M33 * d + M34 * e;
       *pOutput2++ = M40 * a + M41 * b + M42 * c + M43 * d + M44 * e;

    }
}

int main (int argc, const char* argv[])
{
  unsigned int input[N], output[N], i, input2[N], output2[N];

  check_vect ();

  for (i = 0; i < N; i++)
    {
      input[i] = i%256;
      input2[i] = i + 2;
      output[i] = 0;
      output2[i] = 0;
      __asm__ volatile ("");
    }

#if N == 20
  unsigned int check_results[N]
    = { 3208, 1334, 28764, 35679, 2789, 13028, 4754, 168364, 91254, 12399, 
	22848, 8174, 307964, 146829, 22009, 32668, 11594, 447564, 202404,
	31619 };
  unsigned int check_results2[N]
    = { 7136, 2702, 84604, 57909, 6633, 16956, 6122, 224204, 113484, 16243, 
	26776, 9542, 363804, 169059, 25853, 36596, 12962, 503404, 224634,
	35463 };
#else
  volatile unsigned int check_results[N];
  volatile unsigned int check_results2[N];

  for (i = 0; i < N / 5; i++)
    {
      unsigned int a = input[i * 5];
      unsigned int b = input[i * 5 + 1];
      unsigned int c = input[i * 5 + 2];
      unsigned int d = input[i * 5 + 3];
      unsigned int e = input[i * 5 + 4];

      check_results[i * 5] = M00 * a + M01 * b + M02 * c + M03 * d + M04 * e;
      check_results[i * 5 + 1] = (M10 * a + M11 * b + M12 * c
				  + M13 * d + M14 * e);
      check_results[i * 5 + 2] = (M20 * a + M21 * b + M22 * c
				  + M23 * d + M24 * e);
      check_results[i * 5 + 3] = (M30 * a + M31 * b + M32 * c
				  + M33 * d + M34 * e);
      check_results[i * 5 + 4] = (M40 * a + M41 * b + M42 * c
				  + M43 * d + M44 * e);

      a = input2[i * 5];
      b = input2[i * 5 + 1];
      c = input2[i * 5 + 2];
      d = input2[i * 5 + 3];
      e = input2[i * 5 + 4];

      check_results2[i * 5] = M00 * a + M01 * b + M02 * c + M03 * d + M04 * e;
      check_results2[i * 5 + 1]	= (M10 * a + M11 * b + M12 * c
				   + M13 * d + M14 * e);
      check_results2[i * 5 + 2]	= (M20 * a + M21 * b + M22 * c
				   + M23 * d + M24 * e);
      check_results2[i * 5 + 3]	= (M30 * a + M31 * b + M32 * c
				   + M33 * d + M34 * e);
      check_results2[i * 5 + 4] = (M40 * a + M41 * b + M42 * c
				   + M43 * d + M44 * e);

      asm volatile ("" ::: "memory");
    }
#endif

  foo (input, output, input2, output2);

#pragma GCC novector
  for (i = 0; i < N; i++)
    if (output[i] != check_results[i]
        || output2[i] != check_results2[i])
      abort ();

  return 0;
}

/* Currently interleaving is not supported for a group-size of 5.  */

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" } } */
/* { dg-final { scan-tree-dump-times "gaps requires scalar epilogue loop" 0 "vect" } } */
/* { dg-final { scan-tree-dump-times "vectorizing stmts using SLP" 2 "vect" } } */
