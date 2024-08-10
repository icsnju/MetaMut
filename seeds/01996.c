/* { dg-do run } */
/* { dg-options "-O2 -mavx2 -ftree-vectorize -fvect-cost-model=unlimited -mprefer-vector-width=256" } */
/* { dg-require-effective-target avx2 } */

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
#include <string.h>
/* { dg-do compile } */
/* { dg-options "-O2 -mavx2 -ftree-vectorize -fvect-cost-model=unlimited -fdump-tree-slp-details -mprefer-vector-width=256" } */
/* { dg-final { scan-tree-dump-times "basic block part vectorized using (?:32|64) byte vectors" 6 "slp2" } }*/
/* { dg-final { scan-tree-dump-times {(?n)add new stmt:.*VEC_PERM_EXPR.*\{ 2, 3, 0, 1 \}} 4 "slp2" } }  */
/* { dg-final { scan-tree-dump-times {(?n)add new stmt:.*VEC_PERM_EXPR.*\{ 6, 7, 4, 5, 2, 3, 0, 1 \}} 4 "slp2" } }  */
/* { dg-final { scan-tree-dump-times {(?n)add new stmt:.*VEC_PERM_EXPR.*\{ 14, 15, 12, 13, 10, 11, 8, 9, 6, 7, 4, 5, 2, 3, 0, 1 \}} 2 "slp2" } }  */
/* { dg-final { scan-tree-dump-times {(?n)add new stmt:.*VEC_PERM_EXPR.*\{ 30, 31, 28, 29, 26, 27, 24, 25, 22, 23, 20, 21, 18, 19, 16, 17, 14, 15, 12, 13, 10, 11, 8, 9, 6, 7, 4, 5, 2, 3, 0, 1 \}} 2 "slp2" } }  */

void
__attribute__((noipa))
foo_pd (_Complex double* a, _Complex double* __restrict b)
{
  a[0] = b[3];
  a[1] = b[2];
  a[2] = b[1];
  a[3] = b[0];
}

void
__attribute__((noipa))
foo_ps (_Complex float* a, _Complex float* __restrict b)
{
  a[0] = b[7];
  a[1] = b[6];
  a[2] = b[5];
  a[3] = b[4];
  a[4] = b[3];
  a[5] = b[2];
  a[6] = b[1];
  a[7] = b[0];
}

void
__attribute__((noipa))
foo_epi64 (_Complex long long* a, _Complex long long* __restrict b)
{
  a[0] = b[3];
  a[1] = b[2];
  a[2] = b[1];
  a[3] = b[0];
}

void
__attribute__((noipa))
foo_epi32 (_Complex int* a, _Complex int* __restrict b)
{
  a[0] = b[7];
  a[1] = b[6];
  a[2] = b[5];
  a[3] = b[4];
  a[4] = b[3];
  a[5] = b[2];
  a[6] = b[1];
  a[7] = b[0];
}

void
__attribute__((noipa))
foo_epi16 (_Complex short* a, _Complex short* __restrict b)
{
  a[0] = b[15];
  a[1] = b[14];
  a[2] = b[13];
  a[3] = b[12];
  a[4] = b[11];
  a[5] = b[10];
  a[6] = b[9];
  a[7] = b[8];
  a[8] = b[7];
  a[9] = b[6];
  a[10] = b[5];
  a[11] = b[4];
  a[12] = b[3];
  a[13] = b[2];
  a[14] = b[1];
  a[15] = b[0];
}

void
__attribute__((noipa))
foo_epi8 (_Complex char* a, _Complex char* __restrict b)
{
  a[0] = b[31];
  a[1] = b[30];
  a[2] = b[29];
  a[3] = b[28];
  a[4] = b[27];
  a[5] = b[26];
  a[6] = b[25];
  a[7] = b[24];
  a[8] = b[23];
  a[9] = b[22];
  a[10] = b[21];
  a[11] = b[20];
  a[12] = b[19];
  a[13] = b[18];
  a[14] = b[17];
  a[15] = b[16];
  a[16] = b[15];
  a[17] = b[14];
  a[18] = b[13];
  a[19] = b[12];
  a[20] = b[11];
  a[21] = b[10];
  a[22] = b[9];
  a[23] = b[8];
  a[24] = b[7];
  a[25] = b[6];
  a[26] = b[5];
  a[27] = b[4];
  a[28] = b[3];
  a[29] = b[2];
  a[30] = b[1];
  a[31] = b[0];
}

void
avx2_test (void)
{
  _Complex double* pd_src = (_Complex double*) malloc (64);
  _Complex double* pd_dst = (_Complex double*) malloc (64);
  _Complex double* pd_exp = (_Complex double*) malloc (64);
  _Complex float* ps_src = (_Complex float*) malloc (64);
  _Complex float* ps_dst = (_Complex float*) malloc (64);
  _Complex float* ps_exp = (_Complex float*) malloc (64);
  _Complex long long* epi64_src = (_Complex long long*) malloc (64);
  _Complex long long* epi64_dst = (_Complex long long*) malloc (64);
  _Complex long long* epi64_exp = (_Complex long long*) malloc (64);
  _Complex int* epi32_src = (_Complex int*) malloc (64);
  _Complex int* epi32_dst = (_Complex int*) malloc (64);
  _Complex int* epi32_exp = (_Complex int*) malloc (64);
  _Complex short* epi16_src = (_Complex short*) malloc (64);
  _Complex short* epi16_dst = (_Complex short*) malloc (64);
  _Complex short* epi16_exp = (_Complex short*) malloc (64);
  _Complex char* epi8_src = (_Complex char*) malloc (64);
  _Complex char* epi8_dst = (_Complex char*) malloc (64);
  _Complex char* epi8_exp = (_Complex char*) malloc (64);
  char* p = (char* ) malloc (64);
  char* q = (char* ) malloc (64);

  __builtin_memset (pd_dst, 0, 64);
  __builtin_memset (ps_dst, 0, 64);
  __builtin_memset (epi64_dst, 0, 64);
  __builtin_memset (epi32_dst, 0, 64);
  __builtin_memset (epi16_dst, 0, 64);
  __builtin_memset (epi8_dst, 0, 64);

  for (int i = 0; i != 64; i++)
    p[i] = i;

  __builtin_memcpy (pd_src, p, 64);
  __builtin_memcpy (ps_src, p, 64);
  __builtin_memcpy (epi64_src, p, 64);
  __builtin_memcpy (epi32_src, p, 64);
  __builtin_memcpy (epi16_src, p, 64);
  __builtin_memcpy (epi8_src, p, 64);


  for (int i = 0; i != 16; i++)
    {
      q[i] = i + 48;
      q[i + 16] = i + 32;
      q[i + 32] = i + 16;
      q[i + 48] = i;
    }
 
  __builtin_memcpy (pd_exp, q, 64);
  __builtin_memcpy (epi64_exp, q, 64);

   for (int i = 0; i != 8; i++)
    {
      q[i] = i + 56;
      q[i + 8] = i + 48;
      q[i + 16] = i + 40;
      q[i + 24] = i + 32;
      q[i + 32] = i + 24;
      q[i + 40] = i + 16;
      q[i + 48] = i + 8;
      q[i + 56] = i;
    }

  __builtin_memcpy (ps_exp, q, 64);
  __builtin_memcpy (epi32_exp, q, 64);

  for (int i = 0; i != 4; i++)
    {
      q[i] = i + 60;
      q[i + 4] = i + 56;
      q[i + 8] = i + 52;
      q[i + 12] = i + 48;
      q[i + 16] = i + 44;
      q[i + 20] = i + 40;
      q[i + 24] = i + 36;
      q[i + 28] = i + 32;
      q[i + 32] = i + 28;
      q[i + 36] = i + 24;
      q[i + 40] = i + 20;
      q[i + 44] = i + 16;
      q[i + 48] = i + 12;
      q[i + 52] = i + 8;
      q[i + 56] = i + 4;
      q[i + 60] = i;
    }

  __builtin_memcpy (epi16_exp, q, 64);

  for (int i = 0; i != 2; i++)
    {
      q[i] = i + 62;
      q[i + 2] = i + 60;
      q[i + 4] = i + 58;
      q[i + 6] = i + 56;
      q[i + 8] = i + 54;
      q[i + 10] = i + 52;
      q[i + 12] = i + 50;
      q[i + 14] = i + 48;
      q[i + 16] = i + 46;
      q[i + 18] = i + 44;
      q[i + 20] = i + 42;
      q[i + 22] = i + 40;
      q[i + 24] = i + 38;
      q[i + 26] = i + 36;
      q[i + 28] = i + 34;
      q[i + 30] = i + 32;
      q[i + 32] = i + 30;
      q[i + 34] = i + 28;
      q[i + 36] = i + 26;
      q[i + 38] = i + 24;
      q[i + 40] = i + 22;
      q[i + 42] = i + 20;
      q[i + 44] = i + 18;
      q[i + 46] = i + 16;
      q[i + 48] = i + 14;
      q[i + 50] = i + 12;
      q[i + 52] = i + 10;
      q[i + 54] = i + 8;
      q[i + 56] = i + 6;
      q[i + 58] = i + 4;
      q[i + 60] = i + 2;
      q[i + 62] = i;
    }
  __builtin_memcpy (epi8_exp, q, 64);

  foo_pd (pd_dst, pd_src);
  foo_ps (ps_dst, ps_src);
  foo_epi64 (epi64_dst, epi64_src);
  foo_epi32 (epi32_dst, epi32_src);
  foo_epi16 (epi16_dst, epi16_src);
  foo_epi8 (epi8_dst, epi8_src);

  if (__builtin_memcmp (pd_dst, pd_exp, 64) != 0)
    __builtin_abort ();
  if (__builtin_memcmp (ps_dst, ps_exp, 64) != 0)
    __builtin_abort ();
  if (__builtin_memcmp (epi64_dst, epi64_exp, 64) != 0)
    __builtin_abort ();
  if (__builtin_memcmp (epi32_dst, epi32_exp, 64) != 0)
    __builtin_abort ();
  if (__builtin_memcmp (epi16_dst, epi16_exp, 64) != 0)
    __builtin_abort ();
  if (__builtin_memcmp (epi8_dst, epi8_exp, 64) != 0)
    __builtin_abort ();

  return;
}
