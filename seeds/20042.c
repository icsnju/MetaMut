/* PR tree-optimization/71488 */
/* { dg-require-effective-target vect_int } */
/* { dg-require-effective-target vect_pack_trunc } */
/* { dg-additional-options "-msse4" { target sse4_runtime } } */

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
   

int i1, i2;

void __attribute__((noclone,noinline))
fn1 (int * __restrict__ p1, int * __restrict__ p2, int * __restrict__ p3, int size)
{
  int i;

  for (i = 0; i < size; i++)
    p1[i] = ((p2[i] == 0) > (unsigned)(p3[i] == 0)) + (p2[i] == 0);
}

void __attribute__((noclone,noinline))
fn2 (int * __restrict__ p1, int * __restrict__ p2, short * __restrict__ p3, int size)
{
  int i;

  for (i = 0; i < size; i++)
    p1[i] = ((p2[i] == 0) > (unsigned)(p3[i] == 0)) + (p2[i] == 0);
}

void __attribute__((noclone,noinline))
fn3 (int * __restrict__ p1, int * __restrict__ p2, long long * __restrict__ p3, int size)
{
  int i;

  for (i = 0; i < size; i++)
    p1[i] = ((p2[i] == 0) > (unsigned)(p3[i] == 0)) + (p2[i] == 0);
}

void __attribute__((noclone,noinline))
fn4 (int * __restrict__ p1, int * __restrict__ p2, int * __restrict__ p3, int size)
{
  int i;

  for (i = 0; i < size; i++)
    p1[i] = ((p2[i] == 0) >= (unsigned)(p3[i] == 0)) + (p2[i] == 0);
}

void __attribute__((noclone,noinline))
fn5 (int * __restrict__ p1, int * __restrict__ p2, short * __restrict__ p3, int size)
{
  int i;

  for (i = 0; i < size; i++)
    p1[i] = ((p2[i] == 0) >= (unsigned)(p3[i] == 0)) + (p2[i] == 0);
}

void __attribute__((noclone,noinline))
fn6 (int * __restrict__ p1, int * __restrict__ p2, long long * __restrict__ p3, int size)
{
  int i;

  for (i = 0; i < size; i++)
    p1[i] = ((p2[i] == 0) >= (unsigned)(p3[i] == 0)) + (p2[i] == 0);
}

void __attribute__((noclone,noinline))
fn7 (int * __restrict__ p1, int * __restrict__ p2, int * __restrict__ p3, int size)
{
  int i;

  for (i = 0; i < size; i++)
    p1[i] = ((p2[i] == 0) < (unsigned)(p3[i] == 0)) + (p2[i] == 0);
}

void __attribute__((noclone,noinline))
fn8 (int * __restrict__ p1, int * __restrict__ p2, short * __restrict__ p3, int size)
{
  int i;

  for (i = 0; i < size; i++)
    p1[i] = ((p2[i] == 0) < (unsigned)(p3[i] == 0)) + (p2[i] == 0);
}

void __attribute__((noclone,noinline))
fn9 (int * __restrict__ p1, int * __restrict__ p2, long long * __restrict__ p3, int size)
{
  int i;

  for (i = 0; i < size; i++)
    p1[i] = ((p2[i] == 0) < (unsigned)(p3[i] == 0)) + (p2[i] == 0);
}

void __attribute__((noclone,noinline))
fn10 (int * __restrict__ p1, int * __restrict__ p2, int * __restrict__ p3, int size)
{
  int i;

  for (i = 0; i < size; i++)
    p1[i] = ((p2[i] == 0) <= (unsigned)(p3[i] == 0)) + (p2[i] == 0);
}

void __attribute__((noclone,noinline))
fn11 (int * __restrict__ p1, int * __restrict__ p2, short * __restrict__ p3, int size)
{
  int i;

  for (i = 0; i < size; i++)
    p1[i] = ((p2[i] == 0) <= (unsigned)(p3[i] == 0)) + (p2[i] == 0);
}

void __attribute__((noclone,noinline))
fn12 (int * __restrict__ p1, int * __restrict__ p2, long long * __restrict__ p3, int size)
{
  int i;

  for (i = 0; i < size; i++)
    p1[i] = ((p2[i] == 0) <= (unsigned)(p3[i] == 0)) + (p2[i] == 0);
}

void __attribute__((noclone,noinline))
fn13 (int * __restrict__ p1, int * __restrict__ p2, int * __restrict__ p3, int size)
{
  int i;

  for (i = 0; i < size; i++)
    p1[i] = ((p2[i] == 0) == (unsigned)(p3[i] == 0)) + (p2[i] == 0);
}

void __attribute__((noclone,noinline))
fn14 (int * __restrict__ p1, int * __restrict__ p2, short * __restrict__ p3, int size)
{
  int i;

  for (i = 0; i < size; i++)
    p1[i] = ((p2[i] == 0) == (unsigned)(p3[i] == 0)) + (p2[i] == 0);
}

void __attribute__((noclone,noinline))
fn15 (int * __restrict__ p1, int * __restrict__ p2, long long * __restrict__ p3, int size)
{
  int i;

  for (i = 0; i < size; i++)
    p1[i] = ((p2[i] == 0) == (unsigned)(p3[i] == 0)) + (p2[i] == 0);
}

void __attribute__((noclone,noinline))
fn16 (int * __restrict__ p1, int * __restrict__ p2, int * __restrict__ p3, int size)
{
  int i;

  for (i = 0; i < size; i++)
    p1[i] = ((p2[i] == 0) != (unsigned)(p3[i] == 0)) + (p2[i] == 0);
}

void __attribute__((noclone,noinline))
fn17 (int * __restrict__ p1, int * __restrict__ p2, short * __restrict__ p3, int size)
{
  int i;

  for (i = 0; i < size; i++)
    p1[i] = ((p2[i] == 0) != (unsigned)(p3[i] == 0)) + (p2[i] == 0);
}

void __attribute__((noclone,noinline))
fn18 (int * __restrict__ p1, int * __restrict__ p2, long long * __restrict__ p3, int size)
{
  int i;

  for (i = 0; i < size; i++)
    p1[i] = ((p2[i] == 0) != (unsigned)(p3[i] == 0)) + (p2[i] == 0);
}

int eq (int i1, int i2) { return i1 == i2; }
int ne (int i1, int i2) { return i1 != i2; }
int lt (int i1, int i2) { return i1 < i2; }
int le (int i1, int i2) { return i1 <= i2; }
int gt (int i1, int i2) { return i1 > i2; }
int ge (int i1, int i2) { return i1 >= i2; }

typedef int (*cmp_fn)(int, int);

void
check (int *p, cmp_fn fn)
{
  int i;

#pragma GCC novector
  for (i = 0; i < 32; i++)
    {
      int t1 = ((i % 4) > 1) == 0;
      int t2 = (i % 2) == 0;
      int res = fn (t1, t2) + t1;
      if (p[i] != res)
	__builtin_abort ();
    }
}

int
main (int argc, char **argv)
{
  int i1[32], i2[32], res[32];
  short s2[32];
  long long l2[32];
  int i;

  check_vect ();

  for (i = 0; i < 32; i++)
    {
      l2[i] = i2[i] = s2[i] = i % 2;
      i1[i] = (i % 4) > 1;
      asm ("":::"memory");
    }

  fn1 (res, i1, i2, 32);
  check (res, gt);
  fn2 (res, i1, s2, 32);
  check (res, gt);
  fn3 (res, i1, l2, 32);
  check (res, gt);

  fn4 (res, i1, i2, 32);
  check (res, ge);
  fn5 (res, i1, s2, 32);
  check (res, ge);
  fn6 (res, i1, l2, 32);
  check (res, ge);

  fn7 (res, i1, i2, 32);
  check (res, lt);
  fn8 (res, i1, s2, 32);
  check (res, lt);
  fn9 (res, i1, l2, 32);
  check (res, lt);

  fn10 (res, i1, i2, 32);
  check (res, le);
  fn11 (res, i1, s2, 32);
  check (res, le);
  fn12 (res, i1, l2, 32);
  check (res, le);

  fn13 (res, i1, i2, 32);
  check (res, eq);
  fn14 (res, i1, s2, 32);
  check (res, eq);
  fn15 (res, i1, l2, 32);
  check (res, eq);

  fn16 (res, i1, i2, 32);
  check (res, ne);
  fn17 (res, i1, s2, 32);
  check (res, ne);
  fn18 (res, i1, l2, 32);
  check (res, ne);
}

/* { dg-final { scan-tree-dump-times "LOOP VECTORIZED" 18 "vect" { target sse4_runtime } } } */
