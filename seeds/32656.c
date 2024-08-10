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
   

#define N (VECTOR_BITS / 32)
#define MAX_COUNT 4

#define RUN_COUNT(COUNT)				\
  void __attribute__ ((noipa))				\
  run_##COUNT (int *restrict a, int *restrict b)	\
  {							\
    for (int i = 0; i < N * COUNT; ++i)			\
      {							\
	a[i * 2] = b[i * 2] + COUNT;			\
	a[i * 2 + 1] = COUNT;				\
      }							\
  }

RUN_COUNT (1)
RUN_COUNT (2)
RUN_COUNT (3)
RUN_COUNT (4)

void __attribute__ ((noipa))
check (int *restrict a, int count)
{
#pragma GCC novector
  for (int i = 0; i < count * N; ++i)
    if (a[i * 2] != i * 41 + count || a[i * 2 + 1] != count)
      __builtin_abort ();
  if (a[count * 2 * N] != 999)
    __builtin_abort ();
}

int a[N * MAX_COUNT * 2 + 1], b[N * MAX_COUNT * 2];

int
main (void)
{
  check_vect ();

  for (int i = 0; i < N * MAX_COUNT; ++i)
    {
      b[i * 2] = i * 41;
      asm volatile ("" ::: "memory");
    }

  a[N * 2] = 999;
  run_1 (a, b);
  check (a, 1);

  a[N * 4] = 999;
  run_2 (a, b);
  check (a, 2);

  a[N * 6] = 999;
  run_3 (a, b);
  check (a, 3);

  a[N * 8] = 999;
  run_4 (a, b);
  check (a, 4);

  return 0;
}

/* { dg-final { scan-tree-dump {LOOP VECTORIZED} "vect" { target { { vect_int && vect_perm } && vect_element_align } } } } */
