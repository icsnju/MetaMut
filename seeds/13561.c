/* { dg-do run } */
/* { dg-options "-O2 -mprefer-vector-width=512 -mavx512vl -mavx512bw" } */
/* { dg-require-effective-target avx512bw } */
/* { dg-require-effective-target avx512vl } */

/* { dg-do compile } */
/* { dg-options "-mavx512bw -mavx512vl -O2 -mprefer-vector-width=512" } */
/* { dg-final { scan-assembler-times {vpsravw[\t ]*%xmm} 2 { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-times {vpsrlvw[\t ]*%ymm} 2 } } */
/* { dg-final { scan-assembler-times {vpsllvw[\t ]*%zmm} 2 } } */
/* { dg-final { scan-assembler-times {vpsllvq[\t ]*%xmm} 1 } } */
/* { dg-final { scan-assembler-times {vpsravq[\t ]*%ymm} 1 } } */
/* { dg-final { scan-assembler-times {vpsrlvq[\t ]*%zmm} 1 } } */

int n;

typedef char v8qi __attribute__((vector_size (8)));
typedef char v16qi __attribute__((vector_size (16)));
typedef char v32qi __attribute__((vector_size (32)));
typedef short v8hi __attribute__((vector_size (16)));
typedef short v16hi __attribute__((vector_size (32)));
typedef short v32hi __attribute__((vector_size (64)));
typedef long long v2di __attribute__((vector_size (16)));
typedef long long v4di __attribute__((vector_size (32)));
typedef long long v8di __attribute__((vector_size (64)));
typedef unsigned char v8uqi __attribute__((vector_size (8)));
typedef unsigned char v16uqi __attribute__((vector_size (16)));
typedef unsigned char v32uqi __attribute__((vector_size (32)));
typedef unsigned short v8uhi __attribute__((vector_size (16)));
typedef unsigned short v16uhi __attribute__((vector_size (32)));
typedef unsigned short v32uhi __attribute__((vector_size (64)));
typedef unsigned long long v2udi __attribute__((vector_size (16)));
typedef unsigned long long v4udi __attribute__((vector_size (32)));
typedef unsigned long long v8udi __attribute__((vector_size (64)));

#define FOO(TYPE, OP, NAME)		\
  __attribute__((noipa)) TYPE		\
  foo_##TYPE##_##NAME (TYPE a, TYPE b)	\
  {					\
    return a OP b;			\
  }					\

FOO (v8qi, <<, vashl);
FOO (v8qi, >>, vashr);
FOO (v8uqi, >>, vlshr);
FOO (v16qi, <<, vashl);
FOO (v16qi, >>, vashr);
FOO (v16uqi, >>, vlshr);
FOO (v32qi, <<, vashl);
FOO (v32qi, >>, vashr);
FOO (v32uqi, >>, vlshr);
FOO (v8hi, <<, vashl);
FOO (v8hi, >>, vashr);
FOO (v8uhi, >>, vlshr);
FOO (v16hi, <<, vashl);
FOO (v16hi, >>, vashr);
FOO (v16uhi, >>, vlshr);
FOO (v32hi, <<, vashl);
FOO (v32hi, >>, vashr);
FOO (v32uhi, >>, vlshr);
FOO (v2di, <<, vashl);
FOO (v2di, >>, vashr);
FOO (v2udi, >>, vlshr);
FOO (v4di, <<, vashl);
FOO (v4di, >>, vashr);
FOO (v4udi, >>, vlshr);
FOO (v8di, <<, vashl);
FOO (v8di, >>, vashr);
FOO (v8udi, >>, vlshr);
void test (void);
#define DO_TEST test
#define AVX512VL
#define AVX512BW
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


typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef long long int64;
typedef unsigned long long uint64;

#define F_EMULATE(TYPE, SIZE, OP, NAME)		\
  __attribute__((noipa, optimize("-fno-tree-vectorize"))) void	\
  emulate_##SIZE##_##TYPE##_##NAME (TYPE *a,	\
				    TYPE *b,	\
				    TYPE *c)	\
  {						\
    int i;					\
    for (i = 0; i < SIZE; i++)			\
      {						\
	a[i] = b[i] OP c[i];			\
      }						\
  }

F_EMULATE (int8,  8, <<, vashl);
F_EMULATE (int8,  8,  >>, vashr);
F_EMULATE (uint8,  8, >>, vlshr);
F_EMULATE (int8,  16, <<, vashl);
F_EMULATE (int8,  16, >>, vashr);
F_EMULATE (uint8,  16, >>, vlshr);
F_EMULATE (int8,  32, <<, vashl);
F_EMULATE (int8,  32, >>, vashr);
F_EMULATE (uint8,  32, >>, vlshr);
F_EMULATE (int16,  8, <<, vashl);
F_EMULATE (int16,  8, >>, vashr);
F_EMULATE (uint16, 8, >>, vlshr);
F_EMULATE (int16,  16, <<, vashl);
F_EMULATE (int16,  16, >>, vashr);
F_EMULATE (uint16, 16, >>, vlshr);
F_EMULATE (int16,  32, <<, vashl);
F_EMULATE (int16,  32, >>, vashr);
F_EMULATE (uint16, 32, >>, vlshr);
F_EMULATE (int64,  2, <<, vashl);
F_EMULATE (int64,  2, >>, vashr);
F_EMULATE (uint64,  2, >>, vlshr);
F_EMULATE (int64,  4, <<, vashl);
F_EMULATE (int64,  4, >>, vashr);
F_EMULATE (uint64,  4, >>, vlshr);
F_EMULATE (int64,  8, <<, vashl);
F_EMULATE (int64,  8, >>, vashr);
F_EMULATE (uint64,  8, >>, vlshr);

#define VSHIFT(VTYPE, NAME, src1, src2)	\
  foo_##VTYPE##_##NAME (src1, src2)

#define EMULATE(SIZE, TYPE, NAME, dst, src1, src2)	\
  emulate_##SIZE##_##TYPE##_##NAME (dst, src1, src2)

#define F_TEST_SHIFT(VTYPE, VTYPEU, TYPE, TYPEU, SIZE)    \
  __attribute__((noipa, optimize("-fno-tree-vectorize"))) void \
  test_##VTYPE ()\
  {\
    TYPE src1[SIZE], src2[SIZE], ref[SIZE];		\
    TYPEU usrc1[SIZE], usrc2[SIZE], uref[SIZE];			\
    VTYPE dst;	     \
    VTYPEU udst;     \
    int i;\
    for (i = 0; i < SIZE; i++)\
    {\
      dst[i] = ref[i] = -i; \
      src1[i] = -(i + SIZE);			\
      src2[i] = i % 8;			\
      udst[i] = uref[i] = i;			\
      usrc1[i] = (i + SIZE);			\
      usrc2[i] = (i % 8);			\
    }\
    EMULATE(SIZE, TYPE, vashl, ref, src1, src2);	\
    dst = VSHIFT(VTYPE, vashl, *((VTYPE* )&src1[0]), *((VTYPE*) &src2[0])); \
    for (i = 0; i < SIZE; i++)\
    {\
      if(dst[i] != ref[i]) __builtin_abort();\
    }\
    EMULATE(SIZE, TYPE, vashr, ref, src1, src2);	\
    dst = VSHIFT(VTYPE, vashr, *((VTYPE* )&src1[0]), *((VTYPE*) &src2[0])); \
    for (i = 0; i < SIZE; i++)\
    {\
      if(dst[i] != ref[i]) __builtin_abort();\
    }\
    EMULATE(SIZE, TYPEU, vlshr, uref, usrc1, usrc2);	\
    udst = VSHIFT(VTYPEU, vlshr, *((VTYPEU* )&usrc1[0]), *((VTYPEU*) &usrc2[0])); \
    for (i = 0; i < SIZE; i++)\
    {\
      if(udst[i] != uref[i]) __builtin_abort();\
    }\
  }

F_TEST_SHIFT (v8qi, v8uqi, int8, uint8, 8);
F_TEST_SHIFT (v16qi, v16uqi, int8, uint8, 16);
F_TEST_SHIFT (v32qi, v32uqi, int8, uint8, 32);
F_TEST_SHIFT (v8hi, v8uhi, int16, uint16, 8);
F_TEST_SHIFT (v16hi, v16uhi, int16, uint16, 16);
F_TEST_SHIFT (v32hi, v32uhi, int16, uint16, 32);
F_TEST_SHIFT (v2di, v2udi, int64, uint64, 2);
F_TEST_SHIFT (v4di, v4udi, int64, uint64, 4);
F_TEST_SHIFT (v8di, v8udi, int64, uint64, 8);


void
test (void)
{
  test_v8qi ();
  test_v16qi ();
  test_v32qi ();
  test_v8hi ();
  test_v16hi ();
  test_v32hi ();
  test_v2di ();
  test_v4di ();
  test_v8di ();
}