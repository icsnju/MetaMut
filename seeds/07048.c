/* { dg-additional-options "-mavx2 -mtune-ctrl=use_gather" { target avx2_runtime } } */

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
   

#define MASKGATHER(SUFF, TYPE1, TYPE2) \
TYPE1 * __attribute__((noipa)) \
maskgather ## SUFF (int n, TYPE2 *indices, TYPE1 *data) \
{ \
  TYPE1 *out = __builtin_malloc (sizeof (TYPE1) * n); \
  for (int i = 0; i < n; ++i) \
    { \
      TYPE2 d = indices[i]; \
      if (d > 1) \
        out[i] = data[d]; \
    } \
  return out; \
}

MASKGATHER(udiusi, unsigned long long, unsigned int)
MASKGATHER(usiusi, unsigned int, unsigned int)
MASKGATHER(udiudi, unsigned long long, unsigned long long)
MASKGATHER(usiudi, unsigned int, unsigned long long)

int
main()
{
  check_vect ();

    unsigned int idx4[32], data4[32];
  unsigned long long idx8[32], data8[32];
  for (int i = 0; i < 32; ++i)
    {
      idx4[i] = i;
      idx8[i] = i;
      data4[i] = i;
      data8[i] = i;
    }
  unsigned long long *resudiusi = maskgatherudiusi (16, idx4, data8);
  unsigned int *resusiusi = maskgatherusiusi (16, idx4, data4);
  unsigned long long *resudiudi = maskgatherudiudi (16, idx8, data8);
  unsigned int *resusiudi = maskgatherusiudi (16, idx8, data4);
#pragma GCC novector
  for (int i = 0; i < 16; ++i)
    {
      unsigned int d = idx4[i];
      if (d > 1)
        {
          if (resudiusi[i] != data4[d])
            __builtin_abort ();
          if (resudiudi[i] != data4[d])
            __builtin_abort ();
          if (resusiudi[i] != data4[d])
            __builtin_abort ();
          if (resusiusi[i] != data4[d])
            __builtin_abort ();
        }
    }
  return 0;
}
