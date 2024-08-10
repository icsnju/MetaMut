/* { dg-do run { target avx512fp16 } } */
/* { dg-options "-O2 -mavx512fp16 -mavx512vl" } */

static void vec_op_test (void);
#define DO_TEST vec_op_test
#define AVX512FP16
#define AVX512VL
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
/* { dg-do compile } */
/* { dg-options "-O2 -mavx512fp16 -mavx512vl" } */

/* { dg-final { scan-assembler-times "vaddph" 2 } } */
/* { dg-final { scan-assembler-times "vsubph" 2 } } */
/* { dg-final { scan-assembler-times "vmulph" 2 } } */
/* { dg-final { scan-assembler-times "vdivph" 2 } } */

#define DO_PRAGMA(X) _Pragma(#X)

#define VEC_OP_VV(size, op, name)       \
void \
__attribute__ ((noinline, noclone, optimize("tree-slp-vectorize"))) \
vecop_v##size##hf##name (_Float16 * restrict dst,  \
 _Float16 * restrict src1, _Float16 * restrict src2)   \
{ \
    int i;  \
    DO_PRAGMA (GCC unroll size)   \
    for (i = 0; i < size; i++)  \
      dst[i] = src1[i] op src2[i];  \
}

VEC_OP_VV(4, +, add)
VEC_OP_VV(2, +, add)
VEC_OP_VV(4, -, sub)
VEC_OP_VV(2, -, sub)
VEC_OP_VV(4, *, mul)
VEC_OP_VV(2, *, mul)
VEC_OP_VV(4, /, div)
VEC_OP_VV(2, /, div)

_Float16 a[4], b[4], fexp[4], fref[4];

#define EMULATE_VEC_OP_VV(size, op, name) \
void \
__attribute__ ((noinline, noclone)) \
scalar_vecop_v##size##hf##name ( \
  _Float16 * restrict dst, _Float16 * restrict src1,  \
  _Float16 * restrict src2)  \
{ \
  int i;  \
  for (i = 0; i < size; i++)  \
    dst[i] = src1[i] op src2[i];  \
}

EMULATE_VEC_OP_VV (4, +, add)
EMULATE_VEC_OP_VV (2, +, add)
EMULATE_VEC_OP_VV (4, -, sub)
EMULATE_VEC_OP_VV (2, -, sub)
EMULATE_VEC_OP_VV (4, *, mul)
EMULATE_VEC_OP_VV (2, *, mul)
EMULATE_VEC_OP_VV (4, /, div)
EMULATE_VEC_OP_VV (2, /, div)

void init()
{
  int i;
  for (i = 0; i < 4; i++)
    {
      a[i] = i + 0.5; 
      b[i] = i * 1.5;
      fexp[i] = fref[i] = 2.75 * i;
    }
}

int check_cond(void *a, void *b, int size)
{
  int i;
  unsigned short *pa = (unsigned short *)a,
		 *pb = (unsigned short *)b;
  for (i = 0; i < size; i++)
    if (pa[i] != pb[i])
      return 0;
  return 1;
}

#define TEST_VEC_OP_VV(size, name)	\
{ \
  init ();  \
  scalar_vecop_v##size##hf##name (a, b, fexp);  \
  vecop_v##size##hf##name (a, b, fref);  \
  if (!check_cond ((void *)fexp, (void *)fref, size)) \
    abort();  \
}

static void vec_op_test()
{
  TEST_VEC_OP_VV (4, add)
  TEST_VEC_OP_VV (2, add)
  TEST_VEC_OP_VV (4, sub)
  TEST_VEC_OP_VV (2, sub)
  TEST_VEC_OP_VV (4, mul)
  TEST_VEC_OP_VV (2, mul)
  TEST_VEC_OP_VV (4, div)
  TEST_VEC_OP_VV (2, div)
}
