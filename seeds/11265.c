/* Disabling epilogues until we find a better way to deal with scans.  */
/* { dg-additional-options "--param vect-epilogues-nomask=0" } */
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
   

extern float copysignf (float, float);
extern float sqrtf (float);
extern float fabsf (float);
extern void abort (void);
float a[64], b[64], c[64], d[64];

__attribute__((noinline, noclone)) void
f1 (int n)
{
  int i;
  for (i = 0; i < n; i++)
    {
      a[4 * i + 0] = copysignf (b[4 * i + 0], c[4 * i + 0]) + 1.0f + sqrtf (d[4 * i + 0]);
      a[4 * i + 1] = copysignf (b[4 * i + 1], c[4 * i + 1]) + 2.0f + sqrtf (d[4 * i + 1]);
      a[4 * i + 2] = copysignf (b[4 * i + 2], c[4 * i + 2]) + 3.0f + sqrtf (d[4 * i + 2]);
      a[4 * i + 3] = copysignf (b[4 * i + 3], c[4 * i + 3]) + 4.0f + sqrtf (d[4 * i + 3]);
    }
}

__attribute__((noinline, noclone)) void
f2 (int n)
{
  int i;
  for (i = 0; i < 2 * n; i++)
    {
      a[2 * i + 0] = copysignf (b[2 * i + 0], c[2 * i + 0]) + 1.0f + sqrtf (d[2 * i + 0]);
      a[2 * i + 1] = copysignf (b[2 * i + 1], c[2 * i + 1]) + 2.0f + sqrtf (d[2 * i + 1]);
    }
}

__attribute__((noinline, noclone)) void
f3 (void)
{
  int i;
  for (i = 0; i < 64; i++)
    a[i] = copysignf (b[i], c[i]) + 1.0f + sqrtf (d[i]);
}

__attribute__((noinline, noclone)) void
f4 (int n)
{
  int i;
  for (i = 0; i < 2 * n; i++)
    {
      a[3 * i + 0] = copysignf (b[3 * i + 0], c[3 * i + 0]) + 1.0f + sqrtf (d[3 * i + 0]);
      a[3 * i + 1] = copysignf (b[3 * i + 1], c[3 * i + 1]) + 2.0f + sqrtf (d[3 * i + 1]);
      a[3 * i + 2] = copysignf (b[3 * i + 2], c[3 * i + 2]) + 3.0f + sqrtf (d[3 * i + 2]);
    }
}

__attribute__((noinline, noclone)) int
main1 ()
{
  int i;

  for (i = 0; i < 64; i++)
    {
      asm ("");
      b[i] = (i & 1) ? -4 * i : 4 * i;
      c[i] = (i & 2) ? -8 * i : 8 * i;
      d[i] = i * i;
    }
  f1 (16);
#pragma GCC novector
  for (i = 0; i < 64; i++)
    if (fabsf (((i & 2) ? -4 * i : 4 * i) + 1 + (i & 3) + i - a[i]) >= 0.0001f)
      abort ();
    else
      a[i] = 131.25;
  f2 (16);
#pragma GCC novector
  for (i = 0; i < 64; i++)
    if (fabsf (((i & 2) ? -4 * i : 4 * i) + 1 + (i & 1) + i - a[i]) >= 0.0001f)
      abort ();
    else
      a[i] = 131.25;
  f3 ();
#pragma GCC novector
  for (i = 0; i < 64; i++)
    if (fabsf (((i & 2) ? -4 * i : 4 * i) + 1 + i - a[i]) >= 0.0001f)
      abort ();
    else
      a[i] = 131.25;
  f4 (10);
#pragma GCC novector
  for (i = 0; i < 60; i++)
    if (fabsf (((i & 2) ? -4 * i : 4 * i) + 1 + (i % 3) + i - a[i]) >= 0.0001f)
      abort ();
  return 0;
}

int
main ()
{
  check_vect ();
  return main1 ();
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 4 "vect" { target { vect_call_copysignf && vect_call_sqrtf } } } } */
/* { dg-final { scan-tree-dump-times "vectorizing stmts using SLP" 3 "vect" { target { { vect_call_copysignf && vect_call_sqrtf } && vect_perm3_int } } } } */