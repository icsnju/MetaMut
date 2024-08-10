/* { dg-do run } */
/* { dg-options "-O3 -mavx2 -fvect-cost-model=unlimited" } */
/* { dg-require-effective-target avx2 } */

#include <string.h>
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
/* { dg-do compile } */
/* { dg-options "-O3 -mavx2 -fvect-cost-model=unlimited -fdump-tree-vect-details" } */
/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 6 "vect" } } */

typedef struct { _Complex double c; double a1; double a2;}
  cdf;
typedef struct { _Complex double c; double a1; double a2; double a3; double a4;}
  cdf2;
typedef struct { _Complex double c1; _Complex double c2; double a1; double a2; double a3; double a4;}
  cdf3;
typedef struct { _Complex double c1; _Complex double c2; double a1; double a2;}
  cdf4;

#define N 100
/* VMAT_ELEMENTWISE.  */
void
__attribute__((noipa))
foo (cdf* a, cdf* __restrict b)
{
   for (int i = 0; i < N; ++i)
    {
      a[i].c = b[i].c;
      a[i].a1 = b[i].a1;
      a[i].a2 = b[i].a2;
    }
}

/* VMAT_CONTIGUOUS_PERMUTE.  */
void
__attribute__((noipa))
foo1 (cdf2* a, cdf2* __restrict b)
{
   for (int i = 0; i < N; ++i)
    {
      a[i].c = b[i].c;
      a[i].a1 = b[i].a1;
      a[i].a2 = b[i].a2;
      a[i].a3 = b[i].a3;
      a[i].a4 = b[i].a4;
    }
}

/* VMAT_CONTIGUOUS.  */
void
__attribute__((noipa))
foo2 (cdf3* a, cdf3* __restrict b)
{
   for (int i = 0; i < N; ++i)
    {
      a[i].c1 = b[i].c1;
      a[i].c2 = b[i].c2;
      a[i].a1 = b[i].a1;
      a[i].a2 = b[i].a2;
      a[i].a3 = b[i].a3;
      a[i].a4 = b[i].a4;
    }
}

/* VMAT_STRIDED_SLP.  */
void
__attribute__((noipa))
foo3 (cdf4* a, cdf4* __restrict b)
{
   for (int i = 0; i < N; ++i)
    {
      a[i].c1 = b[i].c1;
      a[i].c2 = b[i].c2;
      a[i].a1 = b[i].a1;
      a[i].a2 = b[i].a2;
    }
}

/* VMAT_CONTIGUOUS_REVERSE.  */
void
__attribute__((noipa))
foo4 (_Complex double* a, _Complex double* __restrict b)
{
  for (int i = 0; i != N; i++)
    a[i] = b[N-i-1];
}

/* VMAT_CONTIGUOUS_DOWN.  */
void
__attribute__((noipa))
foo5 (_Complex double* a, _Complex double* __restrict b)
{
  for (int i = 0; i != N; i++)
    a[N-i-1] = b[0];
}

static void
avx2_test (void)
{
  _Complex double* pd_src = (_Complex double*) malloc (N * sizeof (_Complex double));
  _Complex double* pd_dst = (_Complex double*) malloc (N * sizeof (_Complex double));
  _Complex double* pd_src2 = (_Complex double*) malloc (N * sizeof (_Complex double));
  _Complex double* pd_dst2 = (_Complex double*) malloc (N * sizeof (_Complex double));
  cdf* cdf_src = (cdf*) malloc (N * sizeof (cdf));
  cdf* cdf_dst = (cdf*) malloc (N * sizeof (cdf));
  cdf2* cdf2_src = (cdf2*) malloc (N * sizeof (cdf2));
  cdf2* cdf2_dst = (cdf2*) malloc (N * sizeof (cdf2));
  cdf3* cdf3_src = (cdf3*) malloc (N * sizeof (cdf3));
  cdf3* cdf3_dst = (cdf3*) malloc (N * sizeof (cdf3));
  cdf4* cdf4_src = (cdf4*) malloc (N * sizeof (cdf4));
  cdf4* cdf4_dst = (cdf4*) malloc (N * sizeof (cdf4));
  
  char* p_init = (char*) malloc (N * sizeof (cdf3));

  __builtin_memset (cdf_dst, 0, N * sizeof (cdf));
  __builtin_memset (cdf2_dst, 0, N * sizeof (cdf2));
  __builtin_memset (cdf3_dst, 0, N * sizeof (cdf3));
  __builtin_memset (cdf4_dst, 0, N * sizeof (cdf4));
  __builtin_memset (pd_dst, 0, N * sizeof (_Complex double));
  __builtin_memset (pd_dst2, 0, N * sizeof (_Complex double));

  for (int i = 0; i != N * sizeof (cdf3); i++)
    p_init[i] = i;

  memcpy (cdf_src, p_init, N * sizeof (cdf));
  memcpy (cdf2_src, p_init, N * sizeof (cdf2));
  memcpy (cdf3_src, p_init, N * sizeof (cdf3));
  memcpy (cdf4_src, p_init, N * sizeof (cdf4));
  memcpy (pd_src, p_init, N * sizeof (_Complex double));
  for (int i = 0; i != 2 * N * sizeof (double); i++)
    p_init[i] = i % 16;
  memcpy (pd_src2, p_init, N * sizeof (_Complex double));

  foo (cdf_dst, cdf_src);
  foo1 (cdf2_dst, cdf2_src);
  foo2 (cdf3_dst, cdf3_src);
  foo3 (cdf4_dst, cdf4_src);
  foo4 (pd_dst, pd_src);
  foo5 (pd_dst2, pd_src2);
  for (int i = 0; i != N; i++)
    {
      p_init[(N - i - 1) * 16] = i * 16;
      p_init[(N - i - 1) * 16 + 1] = i * 16 + 1;
      p_init[(N - i - 1) * 16 + 2] = i * 16 + 2;
      p_init[(N - i - 1) * 16 + 3] = i * 16 + 3;
      p_init[(N - i - 1) * 16 + 4] = i * 16 + 4;
      p_init[(N - i - 1) * 16 + 5] = i * 16 + 5;
      p_init[(N - i - 1) * 16 + 6] = i * 16 + 6;
      p_init[(N - i - 1) * 16 + 7] = i * 16 + 7;
      p_init[(N - i - 1) * 16 + 8] = i * 16 + 8;
      p_init[(N - i - 1) * 16 + 9] = i * 16 + 9;
      p_init[(N - i - 1) * 16 + 10] = i * 16 + 10;
      p_init[(N - i - 1) * 16 + 11] = i * 16 + 11;
      p_init[(N - i - 1) * 16 + 12] = i * 16 + 12;
      p_init[(N - i - 1) * 16 + 13] = i * 16 + 13;
      p_init[(N - i - 1) * 16 + 14] = i * 16 + 14;
      p_init[(N - i - 1) * 16 + 15] = i * 16 + 15;
    }
  memcpy (pd_src, p_init, N * 16);
 
  if (__builtin_memcmp (pd_dst, pd_src, N * 2 * sizeof (double)) != 0)
    __builtin_abort ();

  if (__builtin_memcmp (pd_dst2, pd_src2, N * 2 * sizeof (double)) != 0)
    __builtin_abort ();

  if (__builtin_memcmp (cdf_dst, cdf_src, N * sizeof (cdf)) != 0)
    __builtin_abort ();

  if (__builtin_memcmp (cdf2_dst, cdf2_src, N * sizeof (cdf2)) != 0)
    __builtin_abort ();

  if (__builtin_memcmp (cdf3_dst, cdf3_src, N * sizeof (cdf3)) != 0)
    __builtin_abort ();

  if (__builtin_memcmp (cdf4_dst, cdf4_src, N * sizeof (cdf4)) != 0)
    __builtin_abort ();
}
