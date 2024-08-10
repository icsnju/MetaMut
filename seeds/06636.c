/* { dg-do run } */
/* { dg-require-effective-target avx512fp16 } */
/* { dg-options "-mavx512fp16" } */

#include <stdarg.h>
#include <assert.h>

static void do_test (void);

#define DO_TEST do_test
#define AVX512FP16
#include <stdlib.h>
#include "cpuid.h"
#include <immintrin.h>
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

typedef union
{
  __m512i x;
  char a[64];
} union512i_b;

typedef union
{
  __m512i x;
  short a[32];
} union512i_w;

typedef union
{
  __m512i x;
  int a[16];
} union512i_d;

typedef union
{
  __m512i x;
  long long a[8];
} union512i_q;

typedef union
{
  __m512 x;
  float a[16];
} union512;

typedef union
{
  __m512d x;
  double a[8];
} union512d;

typedef union
{
  __m512i x;
  unsigned char a[64];
} union512i_ub;
          
typedef union
{
 __m512i x;
 unsigned short a[32];
 } union512i_uw;
                      
typedef union
{
 __m512i x;
 unsigned int a[16];
} union512i_ud;
                  
typedef union
{
 __m512i x;
 unsigned long long a[8];
} union512i_uq;

typedef union
{
  __m128h x;
  _Float16 a[8];
} union128h;

typedef union
{
  __m256h x;
  _Float16 a[16];
} union256h;

typedef union
{
  __m512h x;
  _Float16 a[32];
} union512h;

CHECK_EXP (union512i_b, char, "%d")
CHECK_EXP (union512i_w, short, "%d")
CHECK_EXP (union512i_d, int, "0x%x")
CHECK_EXP (union512i_q, long long, "0x%llx")
CHECK_EXP (union512, float, "%f")
CHECK_EXP (union512d, double, "%f")
CHECK_EXP (union512i_ub, unsigned char, "%d")
CHECK_EXP (union512i_uw, unsigned short, "%d")
CHECK_EXP (union512i_ud, unsigned int, "0x%x")
CHECK_EXP (union512i_uq, unsigned long long, "0x%llx")
     

CHECK_FP_EXP (union512, float, ESP_FLOAT, "%f")
CHECK_FP_EXP (union512d, double, ESP_DOUBLE, "%f")

#define CHECK_ROUGH_EXP(UNION_TYPE, VALUE_TYPE, FMT)		\
static int							\
__attribute__((noinline, unused))				\
check_rough_##UNION_TYPE (UNION_TYPE u, const VALUE_TYPE *v,	\
			  VALUE_TYPE eps)			\
{								\
  int i;							\
  int err = 0;							\
								\
  for (i = 0; i < ARRAY_SIZE (u.a); i++)			\
    {								\
      /* We can have have v[i] == 0 == u.a[i]  for some i,	\
         when we test zero-masking.  */				\
      if (v[i] == 0.0 && u.a[i] == 0.0)				\
	continue;						\
      if (v[i] == 0.0 && u.a[i] != 0.0)				\
	{							\
	  err++;						\
	  PRINTF ("%i: " FMT " != " FMT "\n",			\
		  i, v[i], u.a[i]);				\
	}							\
      VALUE_TYPE rel_err = (u.a[i] - v[i]) / v[i];		\
      if (((rel_err < 0) ? -rel_err : rel_err) > eps)		\
	{							\
	  err++;						\
	  PRINTF ("%i: " FMT " != " FMT "\n",			\
		  i, v[i], u.a[i]);				\
	}							\
    }								\
  return err;							\
}

CHECK_ROUGH_EXP (union512, float, "%f")
CHECK_ROUGH_EXP (union512d, double, "%f")
CHECK_ROUGH_EXP (union256, float, "%f")
CHECK_ROUGH_EXP (union256d, double, "%f")
CHECK_ROUGH_EXP (union128, float, "%f")
CHECK_ROUGH_EXP (union128d, double, "%f")

#ifdef AVX512FP16

CHECK_EXP (union128h, _Float16, "%f")
CHECK_EXP (union256h, _Float16, "%f")
CHECK_EXP (union512h, _Float16, "%f")

#ifndef ESP_FLOAT16
#define ESP_FLOAT16 0.27
#endif

CHECK_FP_EXP (union128h, _Float16, ESP_FLOAT16, "%f")
CHECK_FP_EXP (union256h, _Float16, ESP_FLOAT16, "%f")
CHECK_FP_EXP (union512h, _Float16, ESP_FLOAT16, "%f")

CHECK_ROUGH_EXP (union128h, _Float16, "%f")
CHECK_ROUGH_EXP (union256h, _Float16, "%f")
CHECK_ROUGH_EXP (union512h, _Float16, "%f")
#endif
/* Check if the OS supports executing AVX512F instructions.  */

#define XCR_XFEATURE_ENABLED_MASK	0x0

#define XSTATE_FP	0x1
#define XSTATE_SSE	0x2
#define XSTATE_YMM	0x4
#define XSTATE_OPMASK	0x20
#define XSTATE_ZMM	0x40
#define XSTATE_HI_ZMM	0x80

static int
avx512f_os_support (void)
{
  unsigned int eax, edx;
  unsigned int ecx = XCR_XFEATURE_ENABLED_MASK;
  unsigned int mask = XSTATE_SSE | XSTATE_YMM | XSTATE_OPMASK
		      | XSTATE_ZMM | XSTATE_HI_ZMM;

  __asm__ ("xgetbv" : "=a" (eax), "=d" (edx) : "c" (ecx));

  return ((eax & mask) == mask);
}

#ifndef DO_TEST
#define DO_TEST do_test
#ifdef AVX512VL
static void test_256 (void);
static void test_128 (void);
#else
static void test_512 (void);
#endif

__attribute__ ((noinline))
static void
do_test (void)
{
#ifdef AVX512VL
  test_256 ();
  test_128 ();
#else
  test_512 ();
#endif
}
#endif

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

  /* Run AVX512 test only if host has ISA support.  */
  if (check_osxsave ()
      && (ebx & bit_AVX512F)
#ifdef AVX512VL
      && (ebx & bit_AVX512VL)
#endif
#ifdef AVX512ER
      && (ebx & bit_AVX512ER)
#endif
#ifdef AVX512CD
      && (ebx & bit_AVX512CD)
#endif
#ifdef AVX512DQ
      && (ebx & bit_AVX512DQ)
#endif
#ifdef AVX512BW
      && (ebx & bit_AVX512BW)
#endif
#ifdef AVX512IFMA
      && (ebx & bit_AVX512IFMA)
#endif
#ifdef AVX512VBMI
      && (ecx & bit_AVX512VBMI)
#endif
#ifdef AVX5124FMAPS
      && (edx & bit_AVX5124FMAPS)
#endif
#ifdef AVX5124VNNIW
      && (edx & bit_AVX5124VNNIW)
#endif
#ifdef AVX512VPOPCNTDQ
      && (ecx & bit_AVX512VPOPCNTDQ)
#endif
#ifdef AVX512BITALG
      && (ecx & bit_AVX512BITALG)
#endif
#ifdef GFNI
      && (ecx & bit_GFNI)
#endif
#ifdef AVX512VBMI2
      && (ecx & bit_AVX512VBMI2)
#endif
#ifdef AVX512VNNI
      && (ecx & bit_AVX512VNNI)
#endif
#ifdef AVX512FP16
      && (edx & bit_AVX512FP16)
#endif
#ifdef VAES
      && (ecx & bit_VAES)
#endif
#ifdef VPCLMULQDQ
      && (ecx & bit_VPCLMULQDQ)
#endif
#ifdef AVX512VP2INTERSECT
      && (edx & bit_AVX512VP2INTERSECT)
#endif
      && avx512f_os_support ())
    {
      DO_TEST ();
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

struct m256h
{
  __m256h  v;
};

__m128 n1 = { -283.3, -23.3, 213.4, 1119.03 };
struct m256h n2 = { { -93.83f16, 893.318f16, 3994.3f16, -39484.0f16, 213.4f16, 323.4f16, 42.5f16, -43.4f16,
		      234.4f16, 93.9f16, 34.5f16, -14.5f16, -34.9f16, -421.0f16, 234.5f16, 214.5f16 } };
__m128h n3 = { 11.5f16, -31.80f16, 242.3f16, 136.4f16, 42.8f16, -22.8f16, 343.8f16, 215.4f16 } ;
_Float16 n4 = 32.4f16;
double n5 = 103.3;
__m128h n6 = { -12.3f16, 2.0f16, 245.9f16, -432.1f16, 53.5f16, -13.4f16, 432.5f16, 482.4f16 };
__m128d n7 = { -91.387, -8193.518 };
struct m256h n8 = { { -93.83f16, 893.318f16, 3994.3f16, -39484.0f16, 213.4f16, 323.4f16, 42.5f16, -43.4f16,
		      234.4f16, 93.9f16, 34.5f16, -14.5f16, -34.9f16, -421.0f16, 234.5f16, 214.5f16 } };
__m128 n9 = { -123.3, 2.3, 3.4, -10.03 };
__m128h n10 = { 123.3f16, -100.0f16, 246.9f16, 13.4f16, -134.4f16, 35.4f16, 156.5f16, 953.1f16 };
_Float16 n11 = 40.7f16;
double n12 = 304.9;
__m128h n13 = { 23.3f16, -11.0f16, 24.5f16, -24.5f16, 535.4f16, 35.4f16, -13.4f16, 14.5f16 };
__m256h n14 = { -123.3f16, 23.9f16, 34.4f16, -100.3f16, 284.4f16, 352.5f16, 131.5f16, -13.2f16,
		131.4f16, 382.5f16, 38.5f16, 99.6f16, 423.2f16, -12.44f16, 43.2f16, -34.45f16 };
__m512h n15 = { -39.3f16, -180.9f16, 13.4f16, 35.4f16, -41.1f16, -14.4f16, 24.5f16, 53.54f16,
		 238.4f16, -134.8f16, 24.5f16, 35.6f16, -346.7f16, -43.4f16, -535.3f16, 324.7f16,
		 82.5f16, 21.4f16, 24.4f16, 53.4f16, 23.5f16, -24.4f16, -34.5f16, -32.5f16,
		 23.6f16, -13.4f16, 24.5f16, 35.5f16, -34.4f16, -24.5f16, -34.5f16, 13.5f16 };
__m128d n16 = { 73.0, 63.18 };
__m256 n17 = { -183.3, -22.3, 13.9, -119.3, 483.1, 122.3, -33.4, -9.37 };
__m128 n18 = { -183.3, 22.3, 13.4, -19.03 };

__m128 e1;
struct m256h e2;
__m128h e3;
_Float16 e4;
double e5;
__m128h e6;
__m128d e7;
struct m256h e8;
__m128 e9;
__m128h e10;
_Float16 e11;
double e12;
__m128h e13;
__m256h e14;
__m512h e15;
__m128d e16;
__m256 e17;
__m128 e18;

static void
__attribute__((noinline))
foo (va_list va_arglist)
{
  e4 = va_arg (va_arglist, _Float16);
  e5 = va_arg (va_arglist, double);
  e6 = va_arg (va_arglist, __m128h);
  e7 = va_arg (va_arglist, __m128d);
  e8 = va_arg (va_arglist, struct m256h);
  e9 = va_arg (va_arglist, __m128);
  e10 = va_arg (va_arglist, __m128h);
  e11 = va_arg (va_arglist, _Float16);
  e12 = va_arg (va_arglist, double);
  e13 = va_arg (va_arglist, __m128h);
  e14 = va_arg (va_arglist, __m256h);
  e15 = va_arg (va_arglist, __m512h);
  e16 = va_arg (va_arglist, __m128d);
  e17 = va_arg (va_arglist, __m256);
  e18 = va_arg (va_arglist, __m128);
  va_end (va_arglist);
}

static void
__attribute__((noinline))
test (__m128 a1, struct m256h a2, __m128h a3, ...)
{
  va_list va_arglist;

  e1 = a1;
  e2 = a2;
  e3 = a3;
  va_start (va_arglist, a3);
  foo (va_arglist);
  va_end (va_arglist);
}

static void
do_test (void)
{
  test (n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12,
	n13, n14, n15, n16, n17, n18);
  assert (__builtin_memcmp (&e1, &n1, sizeof (e1)) == 0);
  assert (__builtin_memcmp (&e2, &n2, sizeof (e2)) == 0);
  assert (__builtin_memcmp (&e3, &n3, sizeof (e3)) == 0);
  assert (n4 == e4);
  assert (n5 == e5);
  assert (__builtin_memcmp (&e6, &n6, sizeof (e6)) == 0);
  assert (__builtin_memcmp (&e7, &n7, sizeof (e7)) == 0);
  assert (__builtin_memcmp (&e8, &n8, sizeof (e8)) == 0);
  assert (__builtin_memcmp (&e9, &n9, sizeof (e9)) == 0);
  assert (__builtin_memcmp (&e10, &n10, sizeof (e10)) == 0);
  assert (n11 == e11);
  assert (n12 == e12);
  assert (__builtin_memcmp (&e13, &n13, sizeof (e13)) == 0);
  assert (__builtin_memcmp (&e14, &n14, sizeof (e14)) == 0);
  assert (__builtin_memcmp (&e15, &n15, sizeof (e15)) == 0);
  assert (__builtin_memcmp (&e16, &n16, sizeof (e16)) == 0);
  assert (__builtin_memcmp (&e17, &n17, sizeof (e17)) == 0);
  assert (__builtin_memcmp (&e18, &n18, sizeof (e18)) == 0);
}
