/* { dg-do run } */
/* { dg-options "-O2 -mavx -ftree-vectorize -fvect-cost-model=unlimited -mprefer-vector-width=256" } */
/* { dg-require-effective-target avx } */

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

static void avx_test (void);

static void
__attribute__ ((noinline))
do_test (void)
{
  avx_test ();
}

int
main ()
{
  unsigned int eax, ebx, ecx, edx;
 
  if (!__get_cpuid (1, &eax, &ebx, &ecx, &edx))
    return 0;

  /* Run AVX test only if host has AVX support.  */
  if (((ecx & (bit_AVX | bit_OSXSAVE)) == (bit_AVX | bit_OSXSAVE))
      && avx_os_support ()
#ifdef AVXIFMA
      && __builtin_cpu_supports ("avxifma")
#endif
#ifdef AVXVNNIINT8
      && __builtin_cpu_supports ("avxvnniint8")
#endif
#ifdef AVXNECONVERT
      && __builtin_cpu_supports ("avxneconvert")
#endif
#ifdef AVXVNNIINT16
      && __builtin_cpu_supports ("avxvnniint16")
#endif
      )
    {
      do_test ();
#ifdef DEBUG
      printf ("PASSED\n");
#endif
    }
#ifdef DEBUG
  else
    printf ("SKIPPED\n");
#endif

  return 0;
}
#include <string.h>
/* { dg-do compile } */
/* { dg-options "-O2 -mavx -ftree-vectorize -fvect-cost-model=unlimited -fdump-tree-vect-details -mprefer-vector-width=256" } */
/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 6 "vect" } } */
/* { dg-final { scan-tree-dump-times {(?n)add new stmt:.*MEM <vector\(4\) double>} 1 "vect" } } */
/* { dg-final { scan-tree-dump-times {(?n)add new stmt:.*MEM <vector\(8\) float>} 1 "vect" } } */
/* { dg-final { scan-tree-dump-times {(?n)add new stmt:.*MEM <vector\(4\) long long int>} 1 "vect" } } */
/* { dg-final { scan-tree-dump-times {(?n)add new stmt:.*MEM <vector\(8\) int>} 1 "vect" } } */
/* { dg-final { scan-tree-dump-times {(?n)add new stmt:.*MEM <vector\(16\) short int>} 1 "vect" } } */
/* { dg-final { scan-tree-dump-times {(?n)add new stmt:.*MEM <vector\(32\) char>} 1 "vect" } } */

#define N 10000
void
__attribute__((noipa))
foo_pd (_Complex double* a, _Complex double b)
{
  for (int i = 0; i != N; i++)
    a[i] = b;
}

void
__attribute__((noipa))
foo_ps (_Complex float* a, _Complex float b)
{
  for (int i = 0; i != N; i++)
    a[i] = b;
}

void
__attribute__((noipa))
foo_epi64 (_Complex long long* a, _Complex long long b)
{
  for (int i = 0; i != N; i++)
    a[i] = b;
}

void
__attribute__((noipa))
foo_epi32 (_Complex int* a, _Complex int b)
{
  for (int i = 0; i != N; i++)
    a[i] = b;
}

void
__attribute__((noipa))
foo_epi16 (_Complex short* a, _Complex short b)
{
  for (int i = 0; i != N; i++)
    a[i] = b;
}

void
__attribute__((noipa))
foo_epi8 (_Complex char* a, _Complex char b)
{
  for (int i = 0; i != N; i++)
    a[i] = b;
}

void
avx_test (void)
{
  _Complex double* pd_src = (_Complex double*) malloc (2 * N * sizeof (double));
  _Complex double* pd_dst = (_Complex double*) malloc (2 * N * sizeof (double));
  _Complex float* ps_src = (_Complex float*) malloc (2 * N * sizeof (float));
  _Complex float* ps_dst = (_Complex float*) malloc (2 * N * sizeof (float));
  _Complex long long* epi64_src = (_Complex long long*) malloc (2 * N * sizeof (long long));
  _Complex long long* epi64_dst = (_Complex long long*) malloc (2 * N * sizeof (long long));
  _Complex int* epi32_src = (_Complex int*) malloc (2 * N * sizeof (int));
  _Complex int* epi32_dst = (_Complex int*) malloc (2 * N * sizeof (int));
  _Complex short* epi16_src = (_Complex short*) malloc (2 * N * sizeof (short));
  _Complex short* epi16_dst = (_Complex short*) malloc (2 * N * sizeof (short));
  _Complex char* epi8_src = (_Complex char*) malloc (2 * N * sizeof (char));
  _Complex char* epi8_dst = (_Complex char*) malloc (2 * N * sizeof (char));
  char* p_init = (char*) malloc (2 * N * sizeof (double));

  __builtin_memset (pd_dst, 0, 2 * N * sizeof (double));
  __builtin_memset (ps_dst, 0, 2 * N * sizeof (float));
  __builtin_memset (epi64_dst, 0, 2 * N * sizeof (long long));
  __builtin_memset (epi32_dst, 0, 2 * N * sizeof (int));
  __builtin_memset (epi16_dst, 0, 2 * N * sizeof (short));
  __builtin_memset (epi8_dst, 0, 2 * N * sizeof (char));

  for (int i = 0; i != 2 * N * sizeof (double); i++)
    p_init[i] = i % 2 + 3;

  memcpy (pd_src, p_init, 2 * N * sizeof (double));
  memcpy (ps_dst, p_init, 2 * N * sizeof (float));
  memcpy (epi64_dst, p_init, 2 * N * sizeof (long long));
  memcpy (epi32_dst, p_init, 2 * N * sizeof (int));
  memcpy (epi16_dst, p_init, 2 * N * sizeof (short));
  memcpy (epi8_dst, p_init, 2 * N * sizeof (char));

  foo_pd (pd_dst, pd_src[0]);
  foo_ps (ps_dst, ps_src[0]);
  foo_epi64 (epi64_dst, epi64_src[0]);
  foo_epi32 (epi32_dst, epi32_src[0]);
  foo_epi16 (epi16_dst, epi16_src[0]);
  foo_epi8 (epi8_dst, epi8_src[0]);
  if (__builtin_memcmp (pd_dst, pd_src, N * 2 * sizeof (double)) != 0)
    __builtin_abort ();
  if (__builtin_memcmp (ps_dst, ps_src, N * 2 * sizeof (float)) != 0)
    __builtin_abort ();
  if (__builtin_memcmp (epi64_dst, epi64_src, N * 2 * sizeof (long long)) != 0)
    __builtin_abort ();
  if (__builtin_memcmp (epi32_dst, epi32_src, N * 2 * sizeof (int)) != 0)
    __builtin_abort ();
  if (__builtin_memcmp (epi16_dst, epi16_src, N * 2 * sizeof (short)) != 0)
    __builtin_abort ();
  if (__builtin_memcmp (epi8_dst, epi8_src, N * 2 * sizeof (char)) != 0)
    __builtin_abort ();

  return;
}
