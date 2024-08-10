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
/* { dg-options "-O2 -mavx -ftree-vectorize -fvect-cost-model=unlimited -fdump-tree-slp-details" } */
/* { dg-final { scan-tree-dump-times "basic block part vectorized using (?:32|64) byte vectors" 6 "slp2" } }*/
/* { dg-final { scan-tree-dump-times {(?n)add new stmt:.*MEM <vector\(4\) double>} 1 "slp2" } } */
/* { dg-final { scan-tree-dump-times {(?n)add new stmt:.*MEM <vector\(8\) float>} 1 "slp2" } } */
/* { dg-final { scan-tree-dump-times {(?n)add new stmt:.*MEM <vector\(4\) long long int>} 1 "slp2" } } */
/* { dg-final { scan-tree-dump-times {(?n)add new stmt:.*MEM <vector\(8\) int>} 1 "slp2" } } */
/* { dg-final { scan-tree-dump-times {(?n)add new stmt:.*MEM <vector\(16\) short int>} 1 "slp2" } } */
/* { dg-final { scan-tree-dump-times {(?n)add new stmt:.*MEM <vector\(32\) char>} 1 "slp2" } } */

void
__attribute__((noipa))
foo_pd (_Complex double* a,
	_Complex double b1,
	_Complex double b2)
{
  a[0] = b1;
  a[1] = b2;
}

void
__attribute__((noipa))
foo_ps (_Complex float* a,
	_Complex float b1, _Complex float b2,
	_Complex float b3, _Complex float b4)
{
  a[0] = b1;
  a[1] = b2;
  a[2] = b3;
  a[3] = b4;
}

void
__attribute__((noipa))
foo_epi64 (_Complex long long* a,
	   _Complex long long b1,
	   _Complex long long b2)
{
  a[0] = b1;
  a[1] = b2;
}

void
__attribute__((noipa))
foo_epi32 (_Complex int* a,
	   _Complex int b1, _Complex int b2,
	   _Complex int b3, _Complex int b4)
{
  a[0] = b1;
  a[1] = b2;
  a[2] = b3;
  a[3] = b4;
}

void
__attribute__((noipa))
foo_epi16 (_Complex short* a,
	   _Complex short b1, _Complex short b2,
	   _Complex short b3, _Complex short b4,
	   _Complex short b5, _Complex short b6,
	   _Complex short b7,_Complex short b8)
{
  a[0] = b1;
  a[1] = b2;
  a[2] = b3;
  a[3] = b4;
  a[4] = b5;
  a[5] = b6;
  a[6] = b7;
  a[7] = b8;
}

void
__attribute__((noipa))
foo_epi8 (_Complex char* a,
	  _Complex char b1, _Complex char b2,
	  _Complex char b3, _Complex char b4,
	  _Complex char b5, _Complex char b6,
	  _Complex char b7,_Complex char b8,
	  _Complex char b9, _Complex char b10,
	  _Complex char b11, _Complex char b12,
	  _Complex char b13, _Complex char b14,
	  _Complex char b15,_Complex char b16)
{
  a[0] = b1;
  a[1] = b2;
  a[2] = b3;
  a[3] = b4;
  a[4] = b5;
  a[5] = b6;
  a[6] = b7;
  a[7] = b8;
  a[8] = b9;
  a[9] = b10;
  a[10] = b11;
  a[11] = b12;
  a[12] = b13;
  a[13] = b14;
  a[14] = b15;
  a[15] = b16;
}

void
avx_test (void)
{
  _Complex double* pd_src = (_Complex double*) malloc (32);
  _Complex double* pd_dst = (_Complex double*) malloc (32);
  _Complex float* ps_src = (_Complex float*) malloc (32);
  _Complex float* ps_dst = (_Complex float*) malloc (32);
  _Complex long long* epi64_src = (_Complex long long*) malloc (32);
  _Complex long long* epi64_dst = (_Complex long long*) malloc (32);
  _Complex int* epi32_src = (_Complex int*) malloc (32);
  _Complex int* epi32_dst = (_Complex int*) malloc (32);
  _Complex short* epi16_src = (_Complex short*) malloc (32);
  _Complex short* epi16_dst = (_Complex short*) malloc (32);
  _Complex char* epi8_src = (_Complex char*) malloc (32);
  _Complex char* epi8_dst = (_Complex char*) malloc (32);
  char* p = (char* ) malloc (32);

  __builtin_memset (pd_dst, 0, 32);
  __builtin_memset (ps_dst, 0, 32);
  __builtin_memset (epi64_dst, 0, 32);
  __builtin_memset (epi32_dst, 0, 32);
  __builtin_memset (epi16_dst, 0, 32);
  __builtin_memset (epi8_dst, 0, 32);

  for (int i = 0; i != 32; i++)
    p[i] = i;
  __builtin_memcpy (pd_src, p, 32);
  __builtin_memcpy (ps_src, p, 32);
  __builtin_memcpy (epi64_src, p, 32);
  __builtin_memcpy (epi32_src, p, 32);
  __builtin_memcpy (epi16_src, p, 32);
  __builtin_memcpy (epi8_src, p, 32);

  foo_pd (pd_dst, pd_src[0], pd_src[1]);
  foo_ps (ps_dst, ps_src[0], ps_src[1], ps_src[2], ps_src[3]);
  foo_epi64 (epi64_dst, epi64_src[0], epi64_src[1]);
  foo_epi32 (epi32_dst, epi32_src[0], epi32_src[1], epi32_src[2], epi32_src[3]);
  foo_epi16 (epi16_dst, epi16_src[0], epi16_src[1], epi16_src[2], epi16_src[3],
	     epi16_src[4], epi16_src[5], epi16_src[6], epi16_src[7]);
  foo_epi8 (epi8_dst, epi8_src[0], epi8_src[1], epi8_src[2], epi8_src[3],
	    epi8_src[4], epi8_src[5], epi8_src[6], epi8_src[7],
	    epi8_src[8], epi8_src[9], epi8_src[10], epi8_src[11],
	    epi8_src[12], epi8_src[13], epi8_src[14], epi8_src[15]);

  if (__builtin_memcmp (pd_dst, pd_src, 32) != 0)
    __builtin_abort ();
  if (__builtin_memcmp (ps_dst, ps_src, 32) != 0)
    __builtin_abort ();
  if (__builtin_memcmp (epi64_dst, epi64_src, 32) != 0)
    __builtin_abort ();
  if (__builtin_memcmp (epi32_dst, epi32_src, 32) != 0)
    __builtin_abort ();
  if (__builtin_memcmp (epi16_dst, epi16_src, 32) != 0)
    __builtin_abort ();
  if (__builtin_memcmp (epi8_dst, epi8_src, 32) != 0)
    __builtin_abort ();

  return;
}
