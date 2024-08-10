/* Disabling epilogues until we find a better way to deal with scans.  */
/* { dg-additional-options "--param vect-epilogues-nomask=0" } */
/* { dg-require-effective-target vect_int } */
/* { dg-add-options bind_pic_locally } */

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
   

#define N 32

unsigned short sa[N];
unsigned short sc[N] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
		16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
unsigned short sb[N] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
		16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
unsigned int ia[N];
unsigned int ic[N] = {0,3,6,9,12,15,18,21,24,27,30,33,36,39,42,45,
	       0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
unsigned int ib[N] = {0,3,6,9,12,15,18,21,24,27,30,33,36,39,42,45,
	       0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

/* Current peeling-for-alignment scheme will consider the 'sa[i+7]'
   access for peeling, and therefore will examine the option of
   using a peeling factor = VF-7%VF. This will result in a peeling factor 1,
   which will also align the access to 'ia[i+3]', and the loop could be
   vectorized on all targets that support unaligned loads.
   Without cost model on targets that support misaligned stores, no peeling
   will be applied since we want to keep the four loads aligned.  */

__attribute__ ((noinline))
int main1 (int n)
{
  int i;

  /* Multiple types with different sizes, used in independent
     copmutations. Vectorizable.  */
  for (i = 0; i < n; i++)
    {
      sa[i+7] = sb[i] + sc[i];
      ia[i+3] = ib[i] + ic[i];
    }

  /* check results:  */
#pragma GCC novector
  for (i = 0; i < n; i++)
    {
      if (sa[i+7] != sb[i] + sc[i] || ia[i+3] != ib[i] + ic[i])
	abort ();
    }

  return 0;
}

/* Current peeling-for-alignment scheme will consider the 'ia[i+3]'
   access for peeling, and therefore will examine the option of
   using a peeling factor = VF-3%VF. This will result in a peeling factor
   1 if VF=4,2. This will not align the access to 'sa[i+3]', for which we 
   need to peel 5,1 iterations for VF=4,2 respectively, so the loop can not 
   be vectorized.  However, 'ia[i+3]' also gets aligned if we peel 5
   iterations, so the loop is vectorizable on all targets that support
   unaligned loads.
   Without cost model on targets that support misaligned stores, no peeling
   will be applied since we want to keep the four loads aligned.  */

__attribute__ ((noinline))
int main2 (int n)
{
  int i;

  /* Multiple types with different sizes, used in independent
     copmutations. Vectorizable.  */
  for (i = 0; i < n; i++)
    {
      ia[i+3] = ib[i] + ic[i];
      sa[i+3] = sb[i] + sc[i];
    }

  /* check results:  */
#pragma GCC novector
  for (i = 0; i < n; i++)
    {
      if (sa[i+3] != sb[i] + sc[i] || ia[i+3] != ib[i] + ic[i])
        abort ();
    }

  return 0;
}

int main (void)
{ 
  check_vect ();
  
  main1 (N-7);
  main2 (N-3);

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 2 "vect" { xfail { vect_no_align && { ! vect_hw_misalign } } } } } */
/* { dg-final { scan-tree-dump-times "Alignment of access forced using peeling" 0 "vect" { target { vect_element_align}  } } } */
/* { dg-final { scan-tree-dump-times "Alignment of access forced using peeling" 2 "vect" { xfail { vect_no_align || vect_element_align } } } } */
/* { dg-final { scan-tree-dump-times "Vectorizing an unaligned access" 8 "vect" { target { ! vect_element_align } xfail { ! vect_unaligned_possible } } } } */
/* { dg-final { scan-tree-dump-times "Vectorizing an unaligned access" 4 "vect" { target { vect_element_align } xfail { ! vect_unaligned_possible } } } } */

