/* { dg-do compile } */
/* { dg-options "-O3 -fdump-tree-vect-details -march=skylake -mtune=haswell" } */

/* { dg-do run } */
/* { dg-require-effective-target avx2 } */
/* { dg-options "-O3 -mavx2 -mtune=skylake" } */

#include <stdlib.h>
#include "cpuid.h"
#include <immintrin.h>
#include <stdio.h>
#include <xmmintrin.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(A) (sizeof (A) / sizeof ((A)[0]))
#endif

#ifdef __SSE2__
#include <emmintrin.h>

typedef union
{
  __m128i x;
  char a[16];
} union128i_b;

typedef union
{
  __m128i x;
  unsigned char a[16];
} union128i_ub;

typedef union
{
  __m128i x;
  short a[8];
} union128i_w;

typedef union
{
  __m128i x;
  unsigned short a[8];
} union128i_uw;

typedef union
{
  __m128i x;
  int a[4];
} union128i_d;

typedef union
{
  __m128i x;
  unsigned int a[4];
} union128i_ud;

typedef union
{
  __m128i x;
  long long a[2];
} union128i_q;

typedef union
{
  __m128i x;
  unsigned long long a[2];
} union128i_uq;


typedef union
{
  __m128d x;
  double a[2];
} union128d;
#endif

typedef union
{
  __m128  x;
  float a[4];
} union128;

#ifdef DEBUG
#define PRINTF printf
#else
#define PRINTF(...)	
#endif

#define CHECK_EXP(UNION_TYPE, VALUE_TYPE, FMT)		\
static int						\
__attribute__((noinline, unused))			\
check_##UNION_TYPE (UNION_TYPE u, const VALUE_TYPE *v)	\
{							\
  int i;						\
  int err = 0;						\
							\
  for (i = 0; i < ARRAY_SIZE (u.a); i++)		\
    if (u.a[i] != v[i])					\
      {							\
	err++;						\
	PRINTF ("%i: " FMT " != " FMT "\n",		\
		i, v[i], u.a[i]);			\
      }							\
  return err;						\
}

#ifdef __SSE2__
CHECK_EXP (union128i_b, char, "%d")
CHECK_EXP (union128i_ub, unsigned char, "%d")
CHECK_EXP (union128i_w, short, "%d")
CHECK_EXP (union128i_uw, unsigned short, "%d")
CHECK_EXP (union128i_d, int, "0x%x")
CHECK_EXP (union128i_ud, unsigned int, "0x%x")
CHECK_EXP (union128i_q, long long, "0x%llx")
CHECK_EXP (union128i_uq, unsigned long long, "0x%llx")
CHECK_EXP (union128d, double, "%f")
#endif

CHECK_EXP (union128, float, "%f")

#ifndef ESP_FLOAT
#define ESP_FLOAT 0.000001
#endif
#ifndef ESP_DOUBLE
#define ESP_DOUBLE 0.000001
#endif
#define CHECK_ARRAY(ARRAY, TYPE, FMT)                   \
static int                                              \
__attribute__((noinline, unused))                       \
checkV##ARRAY (const TYPE *v, const TYPE *e, int n)     \
{                                                       \
  int i;                                                \
  int err = 0;                                          \
                                                        \
  for (i = 0; i < n; i++)                               \
    if (v[i] != e[i])                                   \
      {                                                 \
        err++;                                          \
        PRINTF ("%i: " FMT " != " FMT "\n",             \
                i, v[i], e[i]);                 \
      }                                                 \
  return err;                                           \
}

CHECK_ARRAY(c, char, "0x%hhx")
CHECK_ARRAY(s, short, "0x%hx")
CHECK_ARRAY(i, int, "0x%x")
CHECK_ARRAY(l, long long, "0x%llx")
CHECK_ARRAY(uc, unsigned char, "0x%hhx")
CHECK_ARRAY(us, unsigned short, "0x%hx")
CHECK_ARRAY(ui, unsigned int, "0x%x")
CHECK_ARRAY(ul, unsigned long long, "0x%llx")



#define CHECK_FP_ARRAY(ARRAY, TYPE, ESP, FMT)                   \
static int                                              \
__attribute__((noinline, unused))                       \
checkV##ARRAY (const TYPE *v, const TYPE *e, int n)     \
{                                                       \
  int i;                                                \
  int err = 0;                                          \
                                                        \
  for (i = 0; i < n; i++)                               \
    if (v[i] > (e[i] + (ESP)) || v[i] < (e[i] - (ESP))) \
    if (e[i] != v[i])                                   \
      {                                                 \
        err++;                                          \
        PRINTF ("%i: " FMT " != " FMT "\n",             \
                i, v[i], e[i]);                 \
      }                                                 \
  return err;                                           \
}

CHECK_FP_ARRAY (d, double, ESP_DOUBLE, "%f")
CHECK_FP_ARRAY (f, float, ESP_FLOAT, "%f")

#ifdef NEED_IEEE754_FLOAT
union ieee754_float
{
   float d;
   struct 
   {
      unsigned long frac : 23;
      unsigned exp : 8;
      unsigned sign : 1;
   } bits __attribute__((packed));
};
#endif

#ifdef NEED_IEEE754_DOUBLE
union ieee754_double
{
   double d;
   struct 
   {
      unsigned long frac1 : 32;
      unsigned long frac0 : 20;
      unsigned exp : 11;
      unsigned sign : 1;
   } bits __attribute__((packed));
};
#endif

#define CHECK_FP_EXP(UNION_TYPE, VALUE_TYPE, ESP, FMT)		\
static int							\
__attribute__((noinline, unused))				\
check_fp_##UNION_TYPE (UNION_TYPE u, const VALUE_TYPE *v)	\
{								\
  int i;							\
  int err = 0;							\
								\
  for (i = 0; i < ARRAY_SIZE (u.a); i++)			\
    if (u.a[i] > (v[i] + (ESP)) || u.a[i] < (v[i] - (ESP)))	\
      {								\
	err++;							\
	PRINTF ("%i: " FMT " != " FMT "\n",			\
		i, v[i], u.a[i]);				\
      }								\
  return err;							\
}

CHECK_FP_EXP (union128, float, ESP_FLOAT, "%f")
#ifdef __SSE2__
CHECK_FP_EXP (union128d, double, ESP_DOUBLE, "%f")
#endif

#ifndef max
#define max(a, b) (((a) > (b)) ? (a):(b))
#endif
#ifndef min
#define min(a, b) (((a) < (b)) ? (a):(b))
#endif

typedef union
{
  __m256i x;
  char a[32];
} union256i_b;

typedef union
{
  __m256i x;
  short a[16];
} union256i_w;

typedef union
{
  __m256i x;
  int a[8];
} union256i_d;

typedef union
{
  __m256i x;
  long long a[4];
} union256i_q;

typedef union
{
  __m256 x;
  float a[8];
} union256;

typedef union
{
  __m256d x;
  double a[4];
} union256d;

typedef union
{
  __m256i x;
  unsigned char a[32];
} union256i_ub;
         
typedef union
{
  __m256i x;
  unsigned short a[16];
} union256i_uw;
                 
typedef union
{
  __m256i x;
  unsigned int a[8];
} union256i_ud;
  
typedef union
{
  __m256i x;
  unsigned long long a[4];
} union256i_uq;
                          
CHECK_EXP (union256i_b, char, "%d")
CHECK_EXP (union256i_w, short, "%d")
CHECK_EXP (union256i_d, int, "0x%x")
CHECK_EXP (union256i_q, long long, "0x%llx")
CHECK_EXP (union256, float, "%f")
CHECK_EXP (union256d, double, "%f")
CHECK_EXP (union256i_ub, unsigned char, "%d")
CHECK_EXP (union256i_uw, unsigned short, "%d")
CHECK_EXP (union256i_ud, unsigned int, "0x%x")
CHECK_EXP (union256i_uq, unsigned long long, "0x%llx")

#define CHECK_FP_EXP(UNION_TYPE, VALUE_TYPE, ESP, FMT)		\
static int							\
__attribute__((noinline, unused))				\
check_fp_##UNION_TYPE (UNION_TYPE u, const VALUE_TYPE *v)	\
{								\
  int i;							\
  int err = 0;							\
								\
  for (i = 0; i < ARRAY_SIZE (u.a); i++)			\
    if (u.a[i] > (v[i] + (ESP)) || u.a[i] < (v[i] - (ESP)))	\
      {								\
	err++;							\
	PRINTF ("%i: " FMT " != " FMT "\n",			\
		i, v[i], u.a[i]);				\
      }								\
  return err;							\
}

CHECK_FP_EXP (union256, float, ESP_FLOAT, "%f")
CHECK_FP_EXP (union256d, double, ESP_DOUBLE, "%f")
/* Check if the OS supports executing AVX instructions.  */

#define XCR_XFEATURE_ENABLED_MASK	0x0

#define XSTATE_FP	0x1
#define XSTATE_SSE	0x2
#define XSTATE_YMM	0x4

static int
avx_os_support (void)
{
  unsigned int eax, edx;
  unsigned int ecx = XCR_XFEATURE_ENABLED_MASK;

  __asm__ ("xgetbv" : "=a" (eax), "=d" (edx) : "c" (ecx));

  return (eax & (XSTATE_SSE | XSTATE_YMM)) == (XSTATE_SSE | XSTATE_YMM);
}

static void avx2_test (void);

static void __attribute__ ((noinline)) do_test (void)
{
  avx2_test ();
}

static int
check_osxsave (void)
{
  unsigned int eax, ebx, ecx, edx;

  __cpuid (1, eax, ebx, ecx, edx);
  return (ecx & bit_OSXSAVE) != 0;
}

int
main ()
{
  unsigned int eax, ebx, ecx, edx;

  if (!__get_cpuid_count (7, 0, &eax, &ebx, &ecx, &edx))
    return 0;

  /* Run AVX2 test only if host has AVX2 support.  */
  if (check_osxsave () && (ebx & bit_AVX2) && avx_os_support ())
    {
      do_test ();
#ifdef DEBUG
      printf ("PASSED\n");
#endif
      return 0;
    }

#ifdef DEBUG
  printf ("SKIPPED\n");
#endif
  return 0;
}

#define N 1024
float vf1[N+16], vf2[N];
double vd1[N+16], vd2[N];
int k[N];
long l[N];
short n[N];

__attribute__((noinline, noclone)) void
f1 (void)
{
  int i;
  for (i = 0; i < N; i++)
    vf2[i] = vf1[k[i]];
}

__attribute__((noinline, noclone)) void
f2 (void)
{
  int i;
  for (i = 0; i < N; i++)
    n[i] = (int) vf1[k[i]];
}

__attribute__((noinline, noclone)) void
f3 (int x)
{
  int i;
  for (i = 0; i < N; i++)
    vf2[i] = vf1[k[i] + x];
}

__attribute__((noinline, noclone)) void
f4 (int x)
{
  int i;
  for (i = 0; i < N; i++)
    n[i] = (int) vf1[k[i] + x];
}

__attribute__((noinline, noclone)) void
f5 (void)
{
  int i;
  for (i = 0; i < N; i++)
    vd2[i] = vd1[k[i]];
}

__attribute__((noinline, noclone)) void
f6 (void)
{
  int i;
  for (i = 0; i < N; i++)
    n[i] = (int) vd1[k[i]];
}

__attribute__((noinline, noclone)) void
f7 (int x)
{
  int i;
  for (i = 0; i < N; i++)
    vd2[i] = vd1[k[i] + x];
}

__attribute__((noinline, noclone)) void
f8 (int x)
{
  int i;
  for (i = 0; i < N; i++)
    n[i] = (int) vd1[k[i] + x];
}

__attribute__((noinline, noclone)) void
f9 (void)
{
  int i;
  for (i = 0; i < N; i++)
    vf2[i] = vf1[l[i]];
}

__attribute__((noinline, noclone)) void
f10 (void)
{
  int i;
  for (i = 0; i < N; i++)
    n[i] = (int) vf1[l[i]];
}

__attribute__((noinline, noclone)) void
f11 (long x)
{
  int i;
  for (i = 0; i < N; i++)
    vf2[i] = vf1[l[i] + x];
}

__attribute__((noinline, noclone)) void
f12 (long x)
{
  int i;
  for (i = 0; i < N; i++)
    n[i] = (int) vf1[l[i] + x];
}

__attribute__((noinline, noclone)) void
f13 (void)
{
  int i;
  for (i = 0; i < N; i++)
    vd2[i] = vd1[l[i]];
}

__attribute__((noinline, noclone)) void
f14 (void)
{
  int i;
  for (i = 0; i < N; i++)
    n[i] = (int) vd1[l[i]];
}

__attribute__((noinline, noclone)) void
f15 (long x)
{
  int i;
  for (i = 0; i < N; i++)
    vd2[i] = vd1[l[i] + x];
}

__attribute__((noinline, noclone)) void
f16 (long x)
{
  int i;
  for (i = 0; i < N; i++)
    n[i] = (int) vd1[l[i] + x];
}

static void
avx2_test (void)
{
  int i;

  for (i = 0; i < N + 16; i++)
    {
      asm ("");
      vf1[i] = 17.0f + i;
      vd1[i] = 19.0 + i;
    }
  for (i = 0; i < N; i++)
    {
      asm ("");
      k[i] = (i * 731) & (N - 1);
      l[i] = (i * 657) & (N - 1);
    }

  f1 ();
  f2 ();
  for (i = 0; i < N; i++)
    if (vf2[i] != ((i * 731) & (N - 1)) + 17
	|| n[i] != ((i * 731) & (N - 1)) + 17)
      abort ();

  f3 (12);
  f4 (14);
  for (i = 0; i < N; i++)
    if (vf2[i] != ((i * 731) & (N - 1)) + 17 + 12
	|| n[i] != ((i * 731) & (N - 1)) + 17 + 14)
      abort ();

  f5 ();
  f6 ();
  for (i = 0; i < N; i++)
    if (vd2[i] != ((i * 731) & (N - 1)) + 19
	|| n[i] != ((i * 731) & (N - 1)) + 19)
      abort ();

  f7 (7);
  f8 (9);
  for (i = 0; i < N; i++)
    if (vd2[i] != ((i * 731) & (N - 1)) + 19 + 7
	|| n[i] != ((i * 731) & (N - 1)) + 19 + 9)
      abort ();

  f9 ();
  f10 ();
  for (i = 0; i < N; i++)
    if (vf2[i] != ((i * 657) & (N - 1)) + 17
	|| n[i] != ((i * 657) & (N - 1)) + 17)
      abort ();

  f11 (2);
  f12 (4);
  for (i = 0; i < N; i++)
    if (vf2[i] != ((i * 657) & (N - 1)) + 17 + 2
	|| n[i] != ((i * 657) & (N - 1)) + 17 + 4)
      abort ();

  f13 ();
  f14 ();
  for (i = 0; i < N; i++)
    if (vd2[i] != ((i * 657) & (N - 1)) + 19
	|| n[i] != ((i * 657) & (N - 1)) + 19)
      abort ();

  f15 (13);
  f16 (15);
  for (i = 0; i < N; i++)
    if (vd2[i] != ((i * 657) & (N - 1)) + 19 + 13
	|| n[i] != ((i * 657) & (N - 1)) + 19 + 15)
      abort ();
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops in function" 16 "vect" } } */
/* { dg-final { scan-assembler "vpcmpeqd" } } */
