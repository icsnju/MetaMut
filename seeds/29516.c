/* PR target/87214 */
/* { dg-do run { target { avx512vl } } } */
/* { dg-options "-O2 -mavx512vl" } */

#define AVX512VL
#define AVX512F_LEN 512
#define AVX512F_LEN_HALF 256
#define avx512f_test test_512
/* This file is used to reduce a number of runtime tests for AVX512F
   and AVX512VL instructions.  Idea is to create one file per instruction -
   avx512f-insn-2.c - using defines from this file instead of intrinsic
   name, vector length etc.  Then dg-options are set with appropriate
   -Dwhatever options in that .c file producing tests for specific
   length.  */

#ifndef AVX512F_HELPER_INCLUDED
#define AVX512F_HELPER_INCLUDED

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

/* Macros expansion.  */
#define CONCAT(a,b,c) a ## b ## c
#define EVAL(a,b,c) CONCAT(a,b,c)

/* Value to be written into destination.
   We have one value for all types so it must be small enough
   to fit into signed char.  */
#ifndef DEFAULT_VALUE
#define DEFAULT_VALUE 117
#endif

#define MAKE_MASK_MERGE(NAME, TYPE)				      \
static void							      \
__attribute__((noinline, unused))				      \
merge_masking_##NAME (TYPE *arr, unsigned long long mask, int size)   \
{								      \
  int i;							      \
  for (i = 0; i < size; i++)					      \
    {								      \
      arr[i] = (mask & (1LL << i)) ? arr[i] : DEFAULT_VALUE;	      \
    }								      \
}

MAKE_MASK_MERGE(i_b, char)
MAKE_MASK_MERGE(i_w, short)
MAKE_MASK_MERGE(i_d, int)
MAKE_MASK_MERGE(i_q, long long)
MAKE_MASK_MERGE(, float)
MAKE_MASK_MERGE(d, double)
MAKE_MASK_MERGE(i_ub, unsigned char)
MAKE_MASK_MERGE(i_uw, unsigned short)
MAKE_MASK_MERGE(i_ud, unsigned int)
MAKE_MASK_MERGE(i_uq, unsigned long long)

#define MASK_MERGE(TYPE) merge_masking_##TYPE

#define MAKE_MASK_ZERO(NAME, TYPE)				      \
static void							      \
__attribute__((noinline, unused))				      \
zero_masking_##NAME (TYPE *arr, unsigned long long mask, int size)    \
{								      \
  int i;							      \
  for (i = 0; i < size; i++)					      \
    {								      \
      arr[i] = (mask & (1LL << i)) ? arr[i] : 0;		      \
    }								      \
}

MAKE_MASK_ZERO(i_b, char)
MAKE_MASK_ZERO(i_w, short)
MAKE_MASK_ZERO(i_d, int)
MAKE_MASK_ZERO(i_q, long long)
MAKE_MASK_ZERO(, float)
MAKE_MASK_ZERO(d, double)
MAKE_MASK_ZERO(i_ub, unsigned char)
MAKE_MASK_ZERO(i_uw, unsigned short)
MAKE_MASK_ZERO(i_ud, unsigned int)
MAKE_MASK_ZERO(i_uq, unsigned long long)


#define MASK_ZERO(TYPE) zero_masking_##TYPE


/* Unions used for testing (for example union512d, union256d etc.).  */
#define UNION_TYPE(SIZE, NAME) EVAL(union, SIZE, NAME)
/* Corresponding union check.  */
#define UNION_CHECK(SIZE, NAME) EVAL(check_union, SIZE, NAME)
/* Corresponding fp union check.  */
#define UNION_FP_CHECK(SIZE, NAME) EVAL(check_fp_union, SIZE, NAME)
/* Corresponding rough union check.  */
#define UNION_ROUGH_CHECK(SIZE, NAME) \
  EVAL(check_rough_union, SIZE, NAME)
/* Function which tests intrinsic for given length.  */
#define TEST EVAL(test_, AVX512F_LEN,)
/* Function which calculates result.  */
#define CALC EVAL(calc_, AVX512F_LEN,)

#ifndef AVX512VL
#define AVX512F_LEN 512
#define AVX512F_LEN_HALF 256
#endif

#endif /* AVX512F_HELPER_INCLUDED */

/* Intrinsic being tested. It has different deffinitions,
   depending on AVX512F_LEN, so it's outside include guards
   and in undefed away to silence warnings.  */
#if defined INTRINSIC
#undef INTRINSIC
#endif

#if AVX512F_LEN != 128
#define INTRINSIC(NAME) EVAL(_mm, AVX512F_LEN, NAME)
#else
#define INTRINSIC(NAME) _mm ## NAME
#endif

typedef long long int v4di __attribute__((vector_size (4 * sizeof (long long int))));
typedef double v4df __attribute__((vector_size (4 * sizeof (double))));
typedef long long int v8di __attribute__((vector_size (8 * sizeof (long long int))));
typedef double v8df __attribute__((vector_size (8 * sizeof (double))));
typedef int v8si __attribute__((vector_size (8 * sizeof (int))));
typedef float v8sf __attribute__((vector_size (8 * sizeof (float))));
typedef int v16si __attribute__((vector_size (16 * sizeof (int))));
typedef float v16sf __attribute__((vector_size (16 * sizeof (float))));

__attribute__((noipa)) void
f1 (v4di *p)
{
  p[0] = __builtin_shuffle (p[1], p[2], (v4di) { 2, 3, 5, 6 });
}

__attribute__((noipa)) void
f2 (v4df *p)
{
  p[0] = __builtin_shuffle (p[1], p[2], (v4di) { 1, 2, 6, 7 });
}

__attribute__((noipa)) void
f3 (v8di *p)
{
  p[0] = __builtin_shuffle (p[1], p[2], (v8di) { 2, 3, 5, 6, 8, 9, 11, 12 });
}

__attribute__((noipa)) void
f4 (v8df *p)
{
  p[0] = __builtin_shuffle (p[1], p[2], (v8di) { 1, 2, 6, 7, 9, 10, 12, 13 });
}

__attribute__((noipa)) void
f5 (v8si *p)
{
  p[0] = __builtin_shuffle (p[1], p[2], (v8si) { 2, 3, 4, 5, 9, 10, 11, 12 });
}

__attribute__((noipa)) void
f6 (v8sf *p)
{
  p[0] = __builtin_shuffle (p[1], p[2], (v8si) { 1, 2, 3, 4, 12, 13, 14, 15 });
}

__attribute__((noipa)) void
f7 (v16si *p)
{
  p[0] = __builtin_shuffle (p[1], p[2], (v16si) { 0, 1, 2, 3, 1, 2, 3, 4, 16, 17, 18, 19, 25, 26, 27, 28 });
}

__attribute__((noipa)) void
f8 (v16sf *p)
{
  p[0] = __builtin_shuffle (p[1], p[2], (v16si) { 1, 2, 3, 4, 4, 5, 6, 7, 17, 18, 19, 20, 18, 19, 20, 21 });
}

static void
test_256 (void)
{
  v4di a[3] = { { 0, 0, 0, 0 }, { 10, 11, 12, 13 }, { 14, 15, 16, 17 } };
  f1 (a);
  if (a[0][0] != 12 || a[0][1] != 13 || a[0][2] != 15 || a[0][3] != 16)
    __builtin_abort ();
  v4df b[3] = { { 0.0, 0.0, 0.0, 0.0 }, { 10.0, 11.0, 12.0, 13.0 }, { 14.0, 15.0, 16.0, 17.0 } };
  f2 (b);
  if (b[0][0] != 11.0 || b[0][1] != 12.0 || b[0][2] != 16.0 || b[0][3] != 17.0)
    __builtin_abort ();
  v8di c[3] = { { 0, 0, 0, 0, 0, 0, 0, 0 }, { 10, 11, 12, 13, 14, 15, 16, 17 }, { 18, 19, 20, 21, 22, 23, 24, 25 } };
  f3 (c);
  if (c[0][0] != 12 || c[0][1] != 13 || c[0][2] != 15 || c[0][3] != 16
      || c[0][4] != 18 || c[0][5] != 19 || c[0][6] != 21 || c[0][7] != 22)
    __builtin_abort ();
  v8df d[3] = { { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
		{ 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0 },
		{ 18.0, 19.0, 20.0, 21.0, 22.0, 23.0, 24.0, 25.0 } };
  f4 (d);
  if (d[0][0] != 11.0 || d[0][1] != 12.0 || d[0][2] != 16.0 || d[0][3] != 17.0
      || d[0][4] != 19.0 || d[0][5] != 20.0 || d[0][6] != 22.0 || d[0][7] != 23.0)
    __builtin_abort ();
  v8si e[3] = { { 0, 0, 0, 0, 0, 0, 0, 0 }, { 10, 11, 12, 13, 14, 15, 16, 17 }, { 18, 19, 20, 21, 22, 23, 24, 25 } };
  f5 (e);
  if (e[0][0] != 12 || e[0][1] != 13 || e[0][2] != 14 || e[0][3] != 15
      || e[0][4] != 19 || e[0][5] != 20 || e[0][6] != 21 || e[0][7] != 22)
    __builtin_abort ();
  v8sf f[3] = { { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
		{ 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f },
		{ 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f } };
  f6 (f);
  if (f[0][0] != 11.0f || f[0][1] != 12.0f || f[0][2] != 13.0f || f[0][3] != 14.0f
      || f[0][4] != 22.0f || f[0][5] != 23.0f || f[0][6] != 24.0f || f[0][7] != 25.0f)
    __builtin_abort ();
  v16si g[3] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		 { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 },
		 { 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41 } };
  f7 (g);
  if (g[0][0] != 10 || g[0][1] != 11 || g[0][2] != 12 || g[0][3] != 13
      || g[0][4] != 11 || g[0][5] != 12 || g[0][6] != 13 || g[0][7] != 14
      || g[0][8] != 26 || g[0][9] != 27 || g[0][10] != 28 || g[0][11] != 29
      || g[0][12] != 35 || g[0][13] != 36 || g[0][14] != 37 || g[0][15] != 38)
    __builtin_abort ();
  v16sf h[3] = { { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		   0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
		 { 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f,
		   18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f },
		 { 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f,
		   34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f } };
  f8 (h);
  if (h[0][0] != 11.0f || h[0][1] != 12.0f || h[0][2] != 13.0f || h[0][3] != 14.0f
      || h[0][4] != 14.0f || h[0][5] != 15.0f || h[0][6] != 16.0f || h[0][7] != 17.0f
      || h[0][8] != 27.0f || h[0][9] != 28.0f || h[0][10] != 29.0f || h[0][11] != 30.0f
      || h[0][12] != 28.0f || h[0][13] != 29.0f || h[0][14] != 30.0f || h[0][15] != 31.0f)
    __builtin_abort ();
}

static void
test_128 (void)
{
}
