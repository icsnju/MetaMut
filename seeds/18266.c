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
   

extern double sqrt (double);
extern double fabs (double);
extern double ceil (double);
extern double floor (double);

extern void abort (void);

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

typedef struct {
    int initialHeight, initialWidth;
    int rotatedHeight, rotatedWidth;
    int autoCropHeight, autoCropWidth;
} ufraw_data;

void __attribute__((noinline,noclone))
ufraw_test(ufraw_data *uf)
{
  int iWidth = uf->initialWidth;
  int iHeight = uf->initialHeight;
  double aspectRatio = ((double)iWidth) / iHeight;
  double midX = iWidth / 2.0 - 0.5;
  double midY = iHeight / 2.0 - 0.5;
  double maxX = 0, maxY = 0;
  double minX = 999999, minY = 999999;
  double lastX = 0, lastY = 0, area = 0;
  double scale;
  int i;
  for (i = 0; i < iWidth + iHeight - 1; i++)
    {
      int x, y;
      if (i < iWidth) { // Trace the left border of the image
	  x = i;
	  y = 0;
      } else { // Trace the bottom border of the image
	  x = iWidth - 1;
	  y = i - iWidth + 1;
      }
      double srcX = x - midX;
      double srcY = y - midY;
      // A digital planimeter:
      area += srcY * lastX - srcX * lastY;
      lastX = srcX;
      lastY = srcY;
      maxX = MAX(maxX, fabs(srcX));
      maxY = MAX(maxY, fabs(srcY));
      if (fabs(srcX / srcY) > aspectRatio)
	minX = MIN(minX, fabs(srcX));
      else
	minY = MIN(minY, fabs(srcY));
    }
  scale = sqrt((iWidth - 1) * (iHeight - 1) / area);
  uf->rotatedWidth = MIN(ceil(2 * maxX + 1.0) * scale, 2 * iWidth);
  uf->rotatedHeight = MIN(ceil(2 * maxY + 1.0) * scale, 2 * iHeight);
  uf->autoCropWidth = MIN(floor(2 * minX) * scale, 2 * iWidth);
  uf->autoCropHeight = MIN(floor(2 * minY) * scale, 2 * iHeight);
  if (uf->autoCropWidth != 3)
    abort ();
}

int main()
{
  ufraw_data uf_data;
  ufraw_data *uf = &uf_data;
  check_vect ();
  uf->initialWidth = 4;
  uf->initialHeight = 5;
  ufraw_test(uf);
  return 0;
}

