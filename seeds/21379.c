/* { dg-require-effective-target vect_int } */
/* { dg-require-effective-target vect_shift } */
/* { dg-require-effective-target vect_pack_trunc } */
/* { dg-require-effective-target vect_unpack } */

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
   

/* Deliberate use of signed >>.  */
#define DEF_LOOP(SIGNEDNESS)			\
  void __attribute__ ((noipa))			\
  f_##SIGNEDNESS (SIGNEDNESS char *restrict a,	\
		  SIGNEDNESS char *restrict b,	\
		  SIGNEDNESS char c)		\
  {						\
    a[0] = (b[0] + c) >> 1;			\
    a[1] = (b[1] + c) >> 1;			\
    a[2] = (b[2] + c) >> 1;			\
    a[3] = (b[3] + c) >> 1;			\
    a[4] = (b[4] + c) >> 1;			\
    a[5] = (b[5] + c) >> 1;			\
    a[6] = (b[6] + c) >> 1;			\
    a[7] = (b[7] + c) >> 1;			\
    a[8] = (b[8] + c) >> 1;			\
    a[9] = (b[9] + c) >> 1;			\
    a[10] = (b[10] + c) >> 1;			\
    a[11] = (b[11] + c) >> 1;			\
    a[12] = (b[12] + c) >> 1;			\
    a[13] = (b[13] + c) >> 1;			\
    a[14] = (b[14] + c) >> 1;			\
    a[15] = (b[15] + c) >> 1;			\
  }

DEF_LOOP (signed)
DEF_LOOP (unsigned)

#define N 16

#define TEST_LOOP(SIGNEDNESS, BASE_B, C)		\
  {							\
    SIGNEDNESS char a[N], b[N], c[N];			\
    for (int i = 0; i < N; ++i)				\
      {							\
	b[i] = BASE_B + i * 15;				\
	asm volatile ("" ::: "memory");			\
      }							\
    f_##SIGNEDNESS (a, b, C);				\
    _Pragma("GCC novector")				\
    for (int i = 0; i < N; ++i)				\
      if (a[i] != (BASE_B + C + i * 15) >> 1)		\
	__builtin_abort ();				\
  }

int
main (void)
{
  check_vect ();

  TEST_LOOP (signed, -128, -120);
  TEST_LOOP (unsigned, 4, 250);

  return 0;
}

/* { dg-final { scan-tree-dump "demoting int to signed short" "slp2" { target { ! vect_widen_shift } } } } */
/* { dg-final { scan-tree-dump "demoting int to unsigned short" "slp2" { target { ! vect_widen_shift } } } } */
/* { dg-final { scan-tree-dump {\.AVG_FLOOR} "slp2" { target vect_avg_qi } } } */
/* { dg-final { scan-tree-dump-times "optimized: basic block" 2 "slp2" { target vect_hw_misalign } } } */
