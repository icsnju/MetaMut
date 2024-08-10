/* { dg-do run } */
/* { dg-options "-O3 -mpower8-vector" } */
/* { dg-require-effective-target p8vector_hw } */

#include <stdlib.h>

/* Define this to enable the combination of VSX vector double and
   SSE2 data types.  */
#define __VSX_SSE2__ 1

#include <stdio.h>

#define NO_WARN_X86_INTRINSICS 1

#ifdef __ALTIVEC__
#include <xmmintrin.h>

#ifdef __VSX_SSE2__
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

#if __VSX__
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

#endif
typedef union
{
  __m128  x;
  float a[4];
  int i[4];
} union128;

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(A) (sizeof (A) / sizeof ((A)[0]))
#endif

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

#ifdef __VSX_SSE2__
CHECK_EXP (union128i_b, char, "%d")
CHECK_EXP (union128i_ub, unsigned char, "%d")
CHECK_EXP (union128i_w, short, "%d")
CHECK_EXP (union128i_uw, unsigned short, "%d")
CHECK_EXP (union128i_d, int, "0x%x")
CHECK_EXP (union128i_ud, unsigned int, "0x%x")
#ifdef __VSX__
CHECK_EXP (union128i_q, long long, "0x%llx")
CHECK_EXP (union128i_uq, unsigned long long, "0x%llx")
CHECK_EXP (union128d, double, "%f")
#endif
#endif

CHECK_EXP (union128, float, "%f")

#define ESP_FLOAT 0.000001
#define ESP_DOUBLE 0.000001
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
#ifdef __VSX_SSE2__
CHECK_FP_EXP (union128d, double, ESP_DOUBLE, "%f")
#endif
#endif /* __ALTIVEC__ */

/* define DEBUG replace abort with printf on error.  */
//#define DEBUG 1

#define TEST sse2_test

static void sse2_test (void);

static void
__attribute__ ((noinline))
do_test (void)
{
  sse2_test ();
}

int
main ()
  {
    do_test ();
#ifdef DEBUG
    printf ("PASSED\n");
#endif
    return 0;
  }

#ifndef TEST
#define TEST sse2_test_mmx_1
#endif

#include <mmintrin.h>

#define N 4

unsigned long long a[N], b[N], result[N];

unsigned long long check_data[N] =
  { 0x101010101010100full,
    0x1010101010101010ull,
    0x1010101010101010ull,
    0x1010101010101010ull };

__m64
unsigned_add3 (const __m64 * a, const __m64 * b,
	       __m64 * result, unsigned int count)
{
  __m64 _a, _b, one, sum, carry, onesCarry;

  unsigned int i;

  carry = _mm_setzero_si64 ();

  one = _mm_cmpeq_pi8 (carry, carry);
  one = _mm_sub_si64 (carry, one);

  for (i = 0; i < count; i++)
    {
      _a = a[i];
      _b = b[i];

      sum = _mm_add_si64 (_a, _b);
      sum = _mm_add_si64 (sum, carry);

      result[i] = sum;

      onesCarry = _mm_and_si64 (_mm_xor_si64 (_a, _b), carry);
      onesCarry = _mm_or_si64 (_mm_and_si64 (_a, _b), onesCarry);
      onesCarry = _mm_and_si64 (onesCarry, one);

      _a = _mm_srli_si64 (_a, 1);
      _b = _mm_srli_si64 (_b, 1);

      carry = _mm_add_si64 (_mm_add_si64 (_a, _b), onesCarry);
      carry = _mm_srli_si64 (carry, 63);
    }

  return carry;
}

void __attribute__((noinline))
TEST (void)
{
  unsigned long long carry;
  int i;

  /* Really long numbers.  */
  a[3] = a[2] = a[1] = a[0] = 0xd3d3d3d3d3d3d3d3ull;
  b[3] = b[2] = b[1] = b[0] = 0x3c3c3c3c3c3c3c3cull;

  carry = (unsigned long long) unsigned_add3
    ((__m64 *)a, (__m64 *)b, (__m64 *)result, N);

  _mm_empty ();

  if (carry != 1)
    abort ();

  for (i = 0; i < N; i++)
    if (result [i] != check_data[i])
      abort ();
}
