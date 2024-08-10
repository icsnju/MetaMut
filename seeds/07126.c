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

typedef struct {
   unsigned short a;
   unsigned short b;
   unsigned short c;
} s;

#define A(I) (I)
#define B(I) ((I) * 2)
#define C(I) ((unsigned short) ~((I) ^ 0x18))

void __attribute__ ((noinline))
check1 (s *res)
{
  int i;

#pragma GCC novector
  for (i = 0; i < N; i++)
    if (res[i].a != C (i)
	|| res[i].b != A (i)
	|| res[i].c != B (i))
      abort ();
}

void __attribute__ ((noinline))
check2 (unsigned short *res)
{
  int i;

#pragma GCC novector
  for (i = 0; i < N; i++)
    if (res[i] != (unsigned short) (A (i) + B (i) + C (i)))
      abort ();
}

void __attribute__ ((noinline))
check3 (s *res)
{
  int i;

#pragma GCC novector
  for (i = 0; i < N; i++)
    if (res[i].a != i
	|| res[i].b != i
	|| res[i].c != i)
      abort ();
}

void __attribute__ ((noinline))
check4 (unsigned short *res)
{
  int i;

#pragma GCC novector
  for (i = 0; i < N; i++)
    if (res[i] != (unsigned short) (A (i) + B (i)))
      abort ();
}

void __attribute__ ((noinline))
main1 (s *arr)
{
  int i;
  s *ptr = arr;
  s res1[N];
  unsigned short res2[N];

  for (i = 0; i < N; i++)
    {
      res1[i].a = arr[i].c;
      res1[i].b = arr[i].a;
      res1[i].c = arr[i].b;
    }
  check1 (res1);

  for (i = 0; i < N; i++)
    res2[i] = arr[i].a + arr[i].b + arr[i].c;
  check2 (res2);

  for (i = 0; i < N; i++)
    {
      res1[i].a = i;
      res1[i].b = i;
      res1[i].c = i;
    }
  check3 (res1);

  for (i = 0; i < N; i++)
    res2[i] = arr[i].a + arr[i].b;
  check4 (res2);
}

int main (void)
{
  int i;
  s arr[N];

  check_vect ();

  for (i = 0; i < N; i++)
    {
      arr[i].a = A (i);
      arr[i].b = B (i);
      arr[i].c = C (i);
    }
  main1 (arr);

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 4 loops" 1 "vect"  { target vect_strided3 } } } */
