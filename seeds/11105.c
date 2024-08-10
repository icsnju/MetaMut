/* { dg-additional-options "-Wno-shift-overflow" } */
/* { dg-require-effective-target vect_shift } */
/* { dg-require-effective-target vect_int } */
/* Check the standard integer types for left and right shifts to see if the
   compiler replaced a scalar instruction with a vector instruction whether the
   correct value is generated.  */

#ifdef TRACE
#endif

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
   

#ifndef ALIGN
#define ALIGN __attribute__((__aligned__(__BIGGEST_ALIGNMENT__)))
#endif

#ifndef NOINLINE
#define NOINLINE __attribute__((__noinline__))
#endif

#ifdef TRACE
#define TRACE_FUNC(PREFIX, NAME) printf (#PREFIX #NAME "\n")
#define TRACE_DONE()  printf ("done!\n")
#define TRACE_ABORT(I,E,G)						\
do {									\
  printf ("Element %d, expected 0x%lx, got 0x%lx\n",			\
	  I, (long)(E), (long)(G));					\
  abort ();								\
} while (0)

#else
#define TRACE_FUNC(PREFIX, A)
#define TRACE_DONE()
#define TRACE_ABORT(I,E,G) abort ()
#endif

#define NAME(A,B) A ## B

#define VECT_TESTS(PREFIX, TYPE, N)					\
 /* Restrict the optimizer from optimizing the setup loops.  */		\
volatile TYPE NAME (PREFIX, zero) = 0;					\
									\
TYPE NAME (PREFIX, a)[N] ALIGN;						\
TYPE NAME (PREFIX, b)[N] ALIGN;						\
TYPE NAME (PREFIX, c)[N] ALIGN;						\
TYPE NAME (PREFIX, d)[N] ALIGN;						\
									\
static void NOINLINE							\
NAME (PREFIX, lshift_2) (void)						\
{									\
  int i;								\
									\
  TRACE_FUNC (PREFIX, lshift_2);					\
  for (i = 0; i < N; i++)						\
    NAME (PREFIX, a)[i] = NAME (PREFIX, b)[i] << 2;			\
}									\
									\
static void NOINLINE							\
NAME (PREFIX, lshift_var) (int shift)					\
{									\
  int i;								\
									\
  TRACE_FUNC (PREFIX, lshift_var);					\
  for (i = 0; i < N; i++)						\
    NAME (PREFIX, a)[i] = NAME (PREFIX, b)[i] << shift;			\
}									\
									\
static void NOINLINE							\
NAME (PREFIX, lshift_vect) (void)					\
{									\
  int i;								\
									\
  TRACE_FUNC (PREFIX, lshift_vect);					\
  for (i = 0; i < N; i++)						\
    NAME (PREFIX, a)[i] = NAME (PREFIX, b)[i] << NAME (PREFIX, c)[i];	\
}									\
									\
static void NOINLINE							\
NAME (PREFIX, rshift_2) (void)						\
{									\
  int i;								\
									\
  TRACE_FUNC (PREFIX, rshift_2);					\
  for (i = 0; i < N; i++)						\
    NAME (PREFIX, a)[i] = NAME (PREFIX, b)[i] >> 2;			\
}									\
									\
static void NOINLINE							\
NAME (PREFIX, rshift_var) (int shift)					\
{									\
  int i;								\
									\
  TRACE_FUNC (PREFIX, rshift_var);					\
  for (i = 0; i < N; i++)						\
    NAME (PREFIX, a)[i] = NAME (PREFIX, b)[i] >> shift;			\
}									\
									\
static void NOINLINE							\
NAME (PREFIX, rshift_vect) (void)					\
{									\
  int i;								\
									\
  TRACE_FUNC (PREFIX, rshift_vect);					\
  for (i = 0; i < N; i++)						\
    NAME (PREFIX, a)[i] = NAME (PREFIX, b)[i] >> NAME (PREFIX, c)[i];	\
}									\
									\
static void NOINLINE							\
NAME (PREFIX, check) (void)						\
{									\
  int i;								\
									\
  TRACE_FUNC (PREFIX, check);						\
  for (i = 0; i < N; i++)						\
    if (NAME (PREFIX, a)[i] != NAME (PREFIX, d)[i])			\
      TRACE_ABORT (i, NAME (PREFIX, d)[i], NAME (PREFIX, a)[i]);	\
}									\
									\
static void NOINLINE							\
NAME (PREFIX, tests) (void)						\
{									\
  int i;								\
									\
  TRACE_FUNC (PREFIX, tests);						\
  for (i = 0; i < N; i++)						\
    {									\
      NAME (PREFIX, b)[i] = (i + NAME (PREFIX, zero));			\
      NAME (PREFIX, c)[i] = 2;						\
      NAME (PREFIX, d)[i] = (i + NAME (PREFIX, zero)) << 2;		\
    }									\
									\
  NAME (PREFIX, lshift_2) ();						\
  NAME (PREFIX, check) ();						\
									\
  NAME (PREFIX, lshift_var) (2);					\
  NAME (PREFIX, check) ();						\
									\
  NAME (PREFIX, lshift_vect) ();					\
  NAME (PREFIX, check) ();						\
									\
  for (i = 0; i < N; i++)						\
    {									\
      NAME (PREFIX, b)[i] = ((i + NAME (PREFIX, zero)) << 4)		\
	| (((TYPE)0x80) << ((sizeof (TYPE) * 8) - 8));			\
      NAME (PREFIX, c)[i] = 2;						\
      NAME (PREFIX, d)[i] = (TYPE)((NAME (PREFIX, b)[i]			\
				    + NAME (PREFIX, zero)) >> 2);	\
    }									\
									\
  NAME (PREFIX, rshift_2) ();						\
  NAME (PREFIX, check) ();						\
									\
  NAME (PREFIX, rshift_var) (2);					\
  NAME (PREFIX, check) ();						\
									\
  NAME (PREFIX, rshift_vect) ();					\
  NAME (PREFIX, check) ();						\
}

VECT_TESTS(uc_, unsigned char,  16)
VECT_TESTS(us_, unsigned short, 32)
VECT_TESTS(ui_, unsigned int,   32)
VECT_TESTS(ul_, unsigned long,  32)

VECT_TESTS(sc_, signed char,    16)
VECT_TESTS(ss_, short,          32)
VECT_TESTS(si_, int,            32)
VECT_TESTS(sl_, long,           32)

int main ()
{
  int i;

  check_vect ();

  uc_tests ();
  us_tests ();
  ui_tests ();
  ul_tests ();

  sc_tests ();
  ss_tests ();
  si_tests ();
  sl_tests ();

  TRACE_DONE ();
  return 0;
}

