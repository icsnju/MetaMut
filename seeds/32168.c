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

typedef struct {
   unsigned char a;
   unsigned char b;
   unsigned char c;
   unsigned char d;
   unsigned char e;
   unsigned char f;
   unsigned char g;
   unsigned char h;
} s;

__attribute__ ((noinline)) int
main1 ()
{
  int i;
  s arr[N];
  s *ptr = arr;
  s check_res[N];
  s res[N];
  unsigned char u, t, s, x, z, w;

  for (i = 0; i < N; i++)
    {
      arr[i].a = i;
      arr[i].b = i * 2;
      arr[i].c = 17;
      arr[i].d = i+34;
      arr[i].e = i * 3 + 5;
      arr[i].f = i * 5;
      arr[i].g = i - 3;
      arr[i].h = 67;

      u = arr[i].b - arr[i].a;
      t = arr[i].d - arr[i].c;
      check_res[i].c = u + t;
      x = arr[i].b + arr[i].d;
      check_res[i].a = arr[i].a + x;
      check_res[i].d = u + t;
      s = arr[i].h - arr[i].a;
      check_res[i].b = s + t;
      check_res[i].f = arr[i].f + arr[i].h;
      check_res[i].e = arr[i].b + arr[i].e;
      check_res[i].h = arr[i].d;
      check_res[i].g = u + t;

      asm volatile ("" ::: "memory");
    }

  for (i = 0; i < N; i++)
    {
      u = ptr->b - ptr->a;
      t = ptr->d - ptr->c;
      res[i].c = u + t;
      x = ptr->b + ptr->d;
      res[i].a = ptr->a + x;
      res[i].d = u + t;
      s = ptr->h - ptr->a;
      res[i].b = s + t;
      res[i].f = ptr->f + ptr->h;
      res[i].e = ptr->b + ptr->e;
      res[i].h = ptr->d;
      res[i].g = u + t;
      ptr++;
    }

  /* check results:  */
#pragma GCC novector
  for (i = 0; i < N; i++)
    {
      if (res[i].a != check_res[i].a
	  || res[i].b != check_res[i].b
	  || res[i].c != check_res[i].c
	  || res[i].d != check_res[i].d
	  || res[i].e != check_res[i].e
	  || res[i].f != check_res[i].f
	  || res[i].g != check_res[i].g
	  || res[i].h != check_res[i].h)
         abort ();
   }
}


int main (void)
{
  check_vect ();

  main1 ();

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" { target vect_strided8 } } } */
