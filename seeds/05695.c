/* { dg-require-effective-target vect_int } */
/* { dg-additional-options "-O3 -fno-version-loops-for-strides" } */

#include <string.h>
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
   

/* AVX512BW has V64QImode, make char_1 vectorizable with that.  */
#define MAX_VEC_ELEMENTS 64

#define FOO(T,N) \
void __attribute__((noinline,noclone)) \
foo_ ## T ## _ ## N (T * __restrict__ in_, T * __restrict__ out_, int s) \
{ \
  T *in = __builtin_assume_aligned (in_, __BIGGEST_ALIGNMENT__); \
  T *out = __builtin_assume_aligned (out_, __BIGGEST_ALIGNMENT__); \
  for (int i = 0; i < MAX_VEC_ELEMENTS; i++) \
    { \
      for (int j = 0; j < N; ++j) \
        out[j] = in[j]; \
      in += s*N; \
      out += N; \
    } \
}

#define TEST(T,N) \
 _Pragma("GCC novector") \
 do { \
  memset (out, 0, 4096); \
  foo_ ## T ## _ ## N ((T *)in, (T *)out, 1); \
  if (memcmp (in, out, sizeof (T) * MAX_VEC_ELEMENTS * N) != 0) \
    __builtin_abort (); \
  _Pragma("GCC novector") \
  for (int i = sizeof (T) * MAX_VEC_ELEMENTS * N; i < 4096; ++i) \
    if (out[i] != 0) \
      __builtin_abort (); \
 } while (0)

FOO(char, 1)
FOO(char, 2)
FOO(char, 3)
FOO(char, 4)
FOO(char, 6)
FOO(char, 8)
FOO(int, 1)
FOO(int, 2)
FOO(int, 3)
FOO(int, 4)
FOO(int, 6)
FOO(int, 8)
FOO(int, 16)

char in[4096] __attribute__((aligned(__BIGGEST_ALIGNMENT__)));
char out[4096] __attribute__((aligned(__BIGGEST_ALIGNMENT__)));

int main()
{
  check_vect ();

  for (int i = 0; i < 4096; ++i)
    {
      in[i] = i;
      __asm__ volatile ("" : : : "memory");
    }

  TEST(char, 1);
  TEST(char, 2);
  TEST(char, 3);
  TEST(char, 4);
  TEST(char, 6);
  TEST(char, 8);
  TEST(int, 1);
  TEST(int, 2);
  TEST(int, 3);
  TEST(int, 4);
  TEST(int, 6);
  TEST(int, 8);
  TEST(int, 16);

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 13 "vect" { target vect_hw_misalign } } } */
/* For ! vect_hw_misalign it depends on vector size and actual alignment
   requirements of the target which functions can be vectorized.  Avoid
   that bean-counting and per-target listing here.  */