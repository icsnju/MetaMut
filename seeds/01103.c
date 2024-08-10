
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
   

/* Extracted from MultiSource/Benchmarks/TSVC/tsc.inc
   From LLVM test-suite */

#define N 40

int dummy(double[N], double[N], double[N], double[N]);

double array[256*256] __attribute__((aligned(32)));

double x[N] __attribute__((aligned(32)));
double temp;
int temp_int;
struct GlobalData
{
  __attribute__((aligned(32))) double a[N];
  int pad1[3];
  __attribute__((aligned(32))) double b[N];
  int pad2[5];
  __attribute__((aligned(32))) double c[N];
  int pad3[7];
  __attribute__((aligned(32))) double d[N];
  int pad4[11];
} global_data;

__attribute__((aligned(32))) double * const a = global_data.a;
__attribute__((aligned(32))) double * const b = global_data.b;
__attribute__((aligned(32))) double * const c = global_data.c;
__attribute__((aligned(32))) double * const d = global_data.d;

void init(void);
void check(double *_a, double *_b);
int s221(void)
{
  int i;

  init();
  for (i = 1; i < N; i++)
    {
      a[i] += c[i] * d[i];
      b[i] = b[i - 1] + a[i] + d[i];
    }
  check(a, b);
  return 0;
}

int set1d(double arr[N], double value)
{
  int i;

  for (i = 0; i < N; i++) {
    arr[i] = value;
  }
  return 0;
}

void init(void)
{
  set1d(a, 1);
  set1d(b, 2);
  set1d(c, 3);
  set1d(d, 4);
}

void abort(void);

void check(double *_a, double *_b)
{
  int i;

  double suma = 0;
  double sumb = 0;
  for (i = 0; i < N; i++){
    suma += _a[i];
    sumb += _b[i];
  }
  if (suma != 508)
    abort();
  if (sumb != 13340.00)
    abort();
}

int main(int argc, char *argv[])
{
  check_vect ();
  s221();
  return 0;
}

