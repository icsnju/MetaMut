/* PR tree-optimization/51581 */

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
   

short int a[16], b[16];
unsigned short int c[16], d[16];

void
f1 (void)
{
  a[0] = b[0] / 8;
  a[1] = b[1] / 8;
  a[2] = b[2] / 8;
  a[3] = b[3] / 8;
  a[4] = b[4] / 8;
  a[5] = b[5] / 8;
  a[6] = b[6] / 8;
  a[7] = b[7] / 8;
  a[8] = b[8] / 8;
  a[9] = b[9] / 8;
  a[10] = b[10] / 8;
  a[11] = b[11] / 8;
  a[12] = b[12] / 8;
  a[13] = b[13] / 8;
  a[14] = b[14] / 8;
  a[15] = b[15] / 8;
}

void
f2 (void)
{
  c[0] = d[0] / 3;
  c[1] = d[1] / 3;
  c[2] = d[2] / 3;
  c[3] = d[3] / 3;
  c[4] = d[4] / 3;
  c[5] = d[5] / 3;
  c[6] = d[6] / 3;
  c[7] = d[7] / 3;
  c[8] = d[8] / 3;
  c[9] = d[9] / 3;
  c[10] = d[10] / 3;
  c[11] = d[11] / 3;
  c[12] = d[12] / 3;
  c[13] = d[13] / 3;
  c[14] = d[14] / 3;
  c[15] = d[15] / 3;
}

void
f3 (void)
{
  a[0] = b[0] / 8;
  a[1] = b[1] / 4;
  a[2] = b[2] / 8;
  a[3] = b[3] / 4;
  a[4] = b[4] / 8;
  a[5] = b[5] / 4;
  a[6] = b[6] / 8;
  a[7] = b[7] / 4;
  a[8] = b[8] / 8;
  a[9] = b[9] / 4;
  a[10] = b[10] / 8;
  a[11] = b[11] / 4;
  a[12] = b[12] / 8;
  a[13] = b[13] / 4;
  a[14] = b[14] / 8;
  a[15] = b[15] / 4;
}

void
f4 (void)
{
  c[0] = d[0] / 3;
  c[1] = d[1] / 5;
  c[2] = d[2] / 3;
  c[3] = d[3] / 5;
  c[4] = d[4] / 3;
  c[5] = d[5] / 5;
  c[6] = d[6] / 3;
  c[7] = d[7] / 5;
  c[8] = d[8] / 3;
  c[9] = d[9] / 5;
  c[10] = d[10] / 3;
  c[11] = d[11] / 5;
  c[12] = d[12] / 3;
  c[13] = d[13] / 5;
  c[14] = d[14] / 3;
  c[15] = d[15] / 5;
}

void
f5 (void)
{
  a[0] = b[0] / 14;
  a[1] = b[1] / 15;
  a[2] = b[2] / 14;
  a[3] = b[3] / 15;
  a[4] = b[4] / 14;
  a[5] = b[5] / 15;
  a[6] = b[6] / 14;
  a[7] = b[7] / 15;
  a[8] = b[8] / 14;
  a[9] = b[9] / 15;
  a[10] = b[10] / 14;
  a[11] = b[11] / 15;
  a[12] = b[12] / 14;
  a[13] = b[13] / 15;
  a[14] = b[14] / 14;
  a[15] = b[15] / 15;
}

void
f6 (void)
{
  c[0] = d[0] / 6;
  c[1] = d[1] / 5;
  c[2] = d[2] / 6;
  c[3] = d[3] / 5;
  c[4] = d[4] / 6;
  c[5] = d[5] / 5;
  c[6] = d[6] / 13;
  c[7] = d[7] / 5;
  c[8] = d[8] / 6;
  c[9] = d[9] / 5;
  c[10] = d[10] / 6;
  c[11] = d[11] / 5;
  c[12] = d[12] / 6;
  c[13] = d[13] / 5;
  c[14] = d[14] / 13;
  c[15] = d[15] / 5;
}

int
main ()
{
  int i;
  check_vect ();
  asm ("");
  for (i = 0; i < 16; i++)
    {
      asm ("");
      b[i] = i - 8;
      d[i] = i - 8;
    }
  f1 ();
  f2 ();
#pragma GCC novector
  for (i = 0; i < 16; i++)
    if (a[i] != b[i] / 8 || c[i] != d[i] / 3)
      abort ();
  f3 ();
  f4 ();
#pragma GCC novector
  for (i = 0; i < 16; i+= 2)
    if (a[i] != b[i] / 8 || a[i + 1] != b[i + 1] / 4
	|| c[i] != d[i] / 3 || c[i + 1] != d[i + 1] / 5)
      abort ();
  f5 ();
  f6 ();
#pragma GCC novector
  for (i = 0; i < 16; i+= 2)
    if (a[i] != b[i] / 14 || a[i + 1] != b[i + 1] / 15
	|| c[i] != d[i] / ((i & 7) == 6 ? 13 : 6) || c[i + 1] != d[i + 1] / 5)
      abort ();
  return 0;
}

