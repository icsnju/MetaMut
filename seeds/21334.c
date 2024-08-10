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
   

extern void abort (void);
double ad[64], bd[64], cd[64], dd[64], ed[64];
float af[64], bf[64], cf[64], df[64], ef[64];
signed char ac[64], bc[64], cc[64], dc[64], ec[64];
short as[64], bs[64], cs[64], ds[64], es[64];
int ai[64], bi[64], ci[64], di[64], ei[64];
long long all[64], bll[64], cll[64], dll[64], ell[64];
unsigned char auc[64], buc[64], cuc[64], duc[64], euc[64];
unsigned short aus[64], bus[64], cus[64], dus[64], eus[64];
unsigned int au[64], bu[64], cu[64], du[64], eu[64];
unsigned long long aull[64], bull[64], cull[64], dull[64], eull[64];

#define F(var) \
__attribute__((noinline, noclone)) void \
f##var (void) \
{ \
  int i; \
  for (i = 0; i < 64; i++) \
    { \
      __typeof (a##var[0]) d = d##var[i], e = e##var[i]; \
      a##var[i] = b##var[i] > c##var[i] ? d : e; \
    } \
}

#define TESTS \
F (d) F (f) F (c) F (s) F (i) F (ll) F (uc) F (us) F (u) F (ull)

TESTS

int
main ()
{
  int i;

  check_vect ();
  for (i = 0; i < 64; i++)
    {
#undef F
#define F(var) \
      b##var[i] = i + 64; \
      switch (i % 3) \
	{ \
	case 0: c##var[i] = i + 64; break; \
	case 1: c##var[i] = 127 - i; break; \
	case 2: c##var[i] = i; break; \
	} \
      d##var[i] = i / 2; \
      e##var[i] = i * 2;
      TESTS
    }
#undef F
#define F(var) f##var ();
  TESTS
#pragma GCC novector
  for (i = 0; i < 64; i++)
    {
      asm volatile ("" : : : "memory");
#undef F
#define F(var) \
      if (a##var[i] != (b##var[i] > c##var[i] ? d##var[i] : e##var[i])) \
	abort ();
      TESTS
    }
  return 0;
}

