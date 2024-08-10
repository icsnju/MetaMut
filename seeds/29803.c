/* { dg-do run { target avx512fp16 } } */
/* { dg-options "-O2 -mavx512fp16 -mavx512vl" } */

#include <string.h>
#include <stdlib.h>
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
/* { dg-do compile} */
/* { dg-options "-O2 -mavx512fp16 -mavx512vl" } */

#include <immintrin.h>
__m128h
__attribute__ ((noinline, noclone))
vadd128 (__m128h a, __m128h b)
{
  return a + b;
}

__m256h
__attribute__ ((noinline, noclone))
vadd256 (__m256h a, __m256h b)
{
  return a + b;
}

__m128h
__attribute__ ((noinline, noclone))
vsub128 (__m128h a, __m128h b)
{
  return a - b;
}

__m256h
__attribute__ ((noinline, noclone))
vsub256 (__m256h a, __m256h b)
{
  return a - b;
}

__m128h
__attribute__ ((noinline, noclone))
vmul128 (__m128h a, __m128h b)
{
  return a * b;
}

__m256h
__attribute__ ((noinline, noclone))
vmul256 (__m256h a, __m256h b)
{
  return a * b;
}

__m128h
__attribute__ ((noinline, noclone))
vdiv128 (__m128h a, __m128h b)
{
  return a / b;
}

__m256h
__attribute__ ((noinline, noclone))
vdiv256 (__m256h a, __m256h b)
{
  return a / b;
}

/* { dg-final { scan-assembler-times "vaddph\[ \\t\]+\[^\n\r\]*%xmm\[01\]" 1 } } */
/* { dg-final { scan-assembler-times "vaddph\[ \\t\]+\[^\n\r\]*%ymm\[01\]" 1 } } */
/* { dg-final { scan-assembler-times "vsubph\[ \\t\]+\[^\n\r\]*%xmm\[01\]" 1 } } */
/* { dg-final { scan-assembler-times "vsubph\[ \\t\]+\[^\n\r\]*%ymm\[01\]" 1 } } */
/* { dg-final { scan-assembler-times "vmulph\[ \\t\]+\[^\n\r\]*%xmm\[01\]" 1 } } */
/* { dg-final { scan-assembler-times "vmulph\[ \\t\]+\[^\n\r\]*%ymm\[01\]" 1 } } */
/* { dg-final { scan-assembler-times "vdivph\[ \\t\]+\[^\n\r\]*%xmm\[01\]" 1 } } */
/* { dg-final { scan-assembler-times "vdivph\[ \\t\]+\[^\n\r\]*%ymm\[01\]" 1 } } */

/* Get random float16 between -50.x to 50.x.  */
_Float16
get_float16_noround()
{
  return ((int) (100.0 * rand ()/ (RAND_MAX + 1.0)) - 50)
    + 0.1f * (int) (10 * rand() / (RAND_MAX + 1.0));
}

static void
do_test (void)
{
  _Float16 x[16];
  _Float16 y[16];
  _Float16 res_add[16];
  _Float16 res_sub[16];
  _Float16 res_mul[16];
  _Float16 res_div[16];
  for (int i = 0 ; i != 16; i++)
    {
      x[i] = get_float16_noround ();
      y[i] = get_float16_noround ();
      if (y[i] == 0)
	y[i] = 1.0f;
      res_add[i] = x[i] + y[i];
      res_sub[i] = x[i] - y[i];
      res_mul[i] = x[i] * y[i];
      res_div[i] = x[i] / y[i];

    }

  union128h u128 = { x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7] };
  union128h u128_1 = { y[0], y[1], y[2], y[3], y[4], y[5], y[6], y[7] };
  union256h u256 = { x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7],
      x[8], x[9], x[10], x[11], x[12], x[13], x[14], x[15] };
  union256h u256_1 = { y[0], y[1], y[2], y[3], y[4], y[5], y[6], y[7],
      y[8], y[9], y[10], y[11], y[12], y[13], y[14], y[15]};

  __m128h v128;
  __m256h v256;
  union128h a128;
  union256h a256;

  memset (&v128, -1, sizeof (v128));
  v128 = vadd128 (u128.x, u128_1.x);
  a128.x = v128;
  if (check_union128h (a128, res_add))
    abort ();
  memset (&v128, -1, sizeof (v128));
  v128 = vsub128 (u128.x, u128_1.x);
  a128.x = v128;
  if (check_union128h (a128, res_sub))
    abort ();
  memset (&v128, -1, sizeof (v128));
  v128 = vmul128 (u128.x, u128_1.x);
  a128.x = v128;
  if (check_union128h (a128, res_mul))
    abort ();
  memset (&v128, -1, sizeof (v128));
  v128 = vdiv128 (u128.x, u128_1.x);
  a128.x = v128;
  if (check_union128h (a128, res_div))
    abort ();

  memset (&v256, -1, sizeof (v256));
  v256 = vadd256 (u256.x, u256_1.x);
  a256.x = v256;
  if (check_union256h (a256, res_add))
    abort ();
  memset (&v256, -1, sizeof (v256));
  v256 = vsub256 (u256.x, u256_1.x);
  a256.x = v256;
  if (check_union256h (a256, res_sub))
    abort ();
  memset (&v256, -1, sizeof (v256));
  v256 = vmul256 (u256.x, u256_1.x);
  a256.x = v256;
  if (check_union256h (a256, res_mul))
    abort ();
  memset (&v256, -1, sizeof (v256));
  v256 = vdiv256 (u256.x, u256_1.x);
  a256.x = v256;
  if (check_union256h (a256, res_div))
    abort ();
}
