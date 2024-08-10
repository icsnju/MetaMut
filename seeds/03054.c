/* PR target/85918 */
/* { dg-do run } */
/* { dg-require-effective-target avx512dq } */
/* { dg-require-effective-target avx512vl } */
/* { dg-options "-O3 -mavx512dq -mavx512vl -mprefer-vector-width=512 -fno-vect-cost-model" } */

#define AVX512DQ
#define AVX512VL
#define DO_TEST avx512dqvl_test

static void avx512dqvl_test (void);

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

#define N 16

long long ll[N] __attribute__((aligned (64)));
unsigned long long ull[N] __attribute__((aligned (64)));
float f[N] __attribute__((aligned (64)));
double d[N] __attribute__((aligned (64)));

__attribute__((noipa)) void
ll2d1 (void)
{
  int i;

  for (i = 0; i < 4; i++)
    d[i] = ll[i];
}

__attribute__((noipa)) void
ull2d1 (void)
{
  int i;

  for (i = 0; i < 4; i++)
    d[i] = ull[i];
}

__attribute__((noipa)) void
d2ll1 (void)
{
  int i;

  for (i = 0; i < 4; i++)
    ll[i] = d[i];
}

__attribute__((noipa)) void
d2ull1 (void)
{
  int i;

  for (i = 0; i < 4; i++)
    ull[i] = d[i];
}

__attribute__((noipa)) void
ll2f1 (void)
{
  int i;

  for (i = 0; i < 4; i++)
    f[i] = ll[i];
}

__attribute__((noipa)) void
ull2f1 (void)
{
  int i;

  for (i = 0; i < 4; i++)
    f[i] = ull[i];
}

__attribute__((noipa)) void
f2ll1 (void)
{
  int i;

  for (i = 0; i < 4; i++)
    ll[i] = f[i];
}

__attribute__((noipa)) void
f2ull1 (void)
{
  int i;

  for (i = 0; i < 4; i++)
    ull[i] = f[i];
}

__attribute__((noipa)) void
ll2d2 (void)
{
  int i;

  for (i = 0; i < 8; i++)
    d[i] = ll[i];
}

__attribute__((noipa)) void
ull2d2 (void)
{
  int i;

  for (i = 0; i < 8; i++)
    d[i] = ull[i];
}

__attribute__((noipa)) void
d2ll2 (void)
{
  int i;

  for (i = 0; i < 8; i++)
    ll[i] = d[i];
}

__attribute__((noipa)) void
d2ull2 (void)
{
  int i;

  for (i = 0; i < 8; i++)
    ull[i] = d[i];
}

__attribute__((noipa)) void
ll2f2 (void)
{
  int i;

  for (i = 0; i < 8; i++)
    f[i] = ll[i];
}

__attribute__((noipa)) void
ull2f2 (void)
{
  int i;

  for (i = 0; i < 8; i++)
    f[i] = ull[i];
}

__attribute__((noipa)) void
f2ll2 (void)
{
  int i;

  for (i = 0; i < 8; i++)
    ll[i] = f[i];
}

__attribute__((noipa)) void
f2ull2 (void)
{
  int i;

  for (i = 0; i < 8; i++)
    ull[i] = f[i];
}

__attribute__((noipa)) void
ll2d3 (void)
{
  int i;

  for (i = 0; i < 16; i++)
    d[i] = ll[i];
}

__attribute__((noipa)) void
ull2d3 (void)
{
  int i;

  for (i = 0; i < 16; i++)
    d[i] = ull[i];
}

__attribute__((noipa)) void
d2ll3 (void)
{
  int i;

  for (i = 0; i < 16; i++)
    ll[i] = d[i];
}

__attribute__((noipa)) void
d2ull3 (void)
{
  int i;

  for (i = 0; i < 16; i++)
    ull[i] = d[i];
}

__attribute__((noipa)) void
ll2f3 (void)
{
  int i;

  for (i = 0; i < 16; i++)
    f[i] = ll[i];
}

__attribute__((noipa)) void
ull2f3 (void)
{
  int i;

  for (i = 0; i < 16; i++)
    f[i] = ull[i];
}

__attribute__((noipa)) void
f2ll3 (void)
{
  int i;

  for (i = 0; i < 16; i++)
    ll[i] = f[i];
}

__attribute__((noipa)) void
f2ull3 (void)
{
  int i;

  for (i = 0; i < 16; i++)
    ull[i] = f[i];
}

unsigned long long ullt[] = {
  13835058055282163712ULL, 9223653511831486464ULL, 9218868437227405312ULL,
  1ULL, 9305281255077576704ULL, 1191936ULL, 18446462598732840960ULL, 0ULL,
  9223372036854775808ULL, 4611686018427387904ULL, 2305843009213693952ULL,
  9ULL, 9223653511831486464ULL, 0ULL, 65536ULL, 131071ULL
};
float uft[] = {
  13835058055282163712.0f, 9223653511831486464.0f, 9218868437227405312.0f,
  1.0f, 9305281255077576704.0f, 1191936.0f, 18446462598732840960.0f, 0.0f,
  9223372036854775808.0f, 4611686018427387904.0f, 2305843009213693952.0f,
  9.0f, 9223653511831486464.0f, 0.0f, 65536.0f, 131071.0f
};
long long llt[] = {
  9223090561878065152LL, -9223372036854775807LL - 1, -9223090561878065152LL,
  -4LL, -8074672656898588672LL, 8074672656898588672LL, 29LL, -15LL,
  7574773098260463616LL, -7579276697887834112LL, -8615667562136469504LL,
  148LL, -255LL, 9151595917793558528LL, -9218868437227405312LL, 9LL
};
float ft[] = {
  9223090561878065152.0f, -9223372036854775808.0f, -9223090561878065152.0f,
  -4.0f, -8074672656898588672.0f, 8074672656898588672.0f, 29.0f, -15.0f,
  7574773098260463616.0f, -7579276697887834112.0f, -8615667562136469504.0f,
  148.0f, -255.0f, 9151595917793558528.0f, -9218868437227405312.0f, 9.0f
};

static void
avx512dqvl_test (void)
{
  int i;
  for (i = 0; i < 4; i++)
    {
      ll[i] = llt[i];
      ull[i] = ullt[i];
    }
  ll2d1 ();
  for (i = 0; i < 4; i++)
    if (d[i] != ft[i])
      abort ();
  ull2d1 ();
  for (i = 0; i < 4; i++)
    if (d[i] != uft[i])
      abort ();
    else
      d[i] = ft[i + 4];
  d2ll1 ();
  for (i = 0; i < 4; i++)
    if (ll[i] != llt[i + 4])
      abort ();
    else
      d[i] = uft[i + 4];
  d2ull1 ();
  for (i = 0; i < 4; i++)
    if (ull[i] != ullt[i + 4])
      abort ();
    else
      {
        ll[i] = llt[i + 8];
	ull[i] = ullt[i + 8];
      }
  ll2f1 ();
  for (i = 0; i < 4; i++)
    if (f[i] != ft[i + 8])
      abort ();
  ull2f1 ();
  for (i = 0; i < 4; i++)
    if (f[i] != uft[i + 8])
      abort ();
    else
      f[i] = ft[i + 12];
  f2ll1 ();
  for (i = 0; i < 4; i++)
    if (ll[i] != llt[i + 12])
      abort ();
    else
      f[i] = uft[i + 12];
  f2ull1 ();
  for (i = 0; i < 4; i++)
    if (ull[i] != ullt[i + 12])
      abort ();
  for (i = 0; i < 8; i++)
    {
      ll[i] = llt[i];
      ull[i] = ullt[i];
    }
  ll2d2 ();
  for (i = 0; i < 8; i++)
    if (d[i] != ft[i])
      abort ();
  ull2d2 ();
  for (i = 0; i < 8; i++)
    if (d[i] != uft[i])
      abort ();
    else
      {
        d[i] = ft[i];
        ll[i] = 1234567LL;
        ull[i] = 7654321ULL;
      }
  d2ll2 ();
  for (i = 0; i < 8; i++)
    if (ll[i] != llt[i])
      abort ();
    else
      d[i] = uft[i];
  d2ull2 ();
  for (i = 0; i < 8; i++)
    if (ull[i] != ullt[i])
      abort ();
    else
      {
        ll[i] = llt[i + 8];
	ull[i] = ullt[i + 8];
      }
  ll2f2 ();
  for (i = 0; i < 8; i++)
    if (f[i] != ft[i + 8])
      abort ();
  ull2f2 ();
  for (i = 0; i < 8; i++)
    if (f[i] != uft[i + 8])
      abort ();
    else
      {
	f[i] = ft[i + 8];
	ll[i] = 1234567LL;
	ull[i] = 7654321ULL;
      }
  f2ll2 ();
  for (i = 0; i < 8; i++)
    if (ll[i] != llt[i + 8])
      abort ();
    else
      f[i] = uft[i + 8];
  f2ull2 ();
  for (i = 0; i < 8; i++)
    if (ull[i] != ullt[i + 8])
      abort ();
  for (i = 0; i < 16; i++)
    {
      ll[i] = llt[i];
      ull[i] = ullt[i];
    }
  ll2d3 ();
  for (i = 0; i < 16; i++)
    if (d[i] != ft[i])
      abort ();
  ull2d3 ();
  for (i = 0; i < 16; i++)
    if (d[i] != uft[i])
      abort ();
    else
      {
        d[i] = ft[i];
        ll[i] = 1234567LL;
        ull[i] = 7654321ULL;
      }
  d2ll3 ();
  for (i = 0; i < 16; i++)
    if (ll[i] != llt[i])
      abort ();
    else
      d[i] = uft[i];
  d2ull3 ();
  for (i = 0; i < 16; i++)
    if (ull[i] != ullt[i])
      abort ();
    else
      {
        ll[i] = llt[i];
	ull[i] = ullt[i];
	f[i] = 3.0f;
	d[i] = 4.0;
      }
  ll2f3 ();
  for (i = 0; i < 16; i++)
    if (f[i] != ft[i])
      abort ();
  ull2f3 ();
  for (i = 0; i < 16; i++)
    if (f[i] != uft[i])
      abort ();
    else
      {
	f[i] = ft[i];
	ll[i] = 1234567LL;
	ull[i] = 7654321ULL;
      }
  f2ll3 ();
  for (i = 0; i < 16; i++)
    if (ll[i] != llt[i])
      abort ();
    else
      f[i] = uft[i];
  f2ull3 ();
  for (i = 0; i < 16; i++)
    if (ull[i] != ullt[i])
      abort ();
}
