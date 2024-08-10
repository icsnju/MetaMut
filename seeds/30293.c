/* PR target/71559 */
/* { dg-do run { target sse2 } } */
/* { dg-options "-O2 -ftree-vectorize -msse2" } */

#ifndef PR71559_TEST
#include <stdlib.h>
#include "cpuid.h"
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
/* Check if the OS supports executing SSE instructions.  */

static int
sse_os_support (void)
{
  /* All currently supported OSes do.  */
  return 1;
}

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
  unsigned int eax, ebx, ecx, edx;
 
  if (!__get_cpuid (1, &eax, &ebx, &ecx, &edx))
    return 0;

  /* Run SSE2 test only if host has SSE2 support.  */
  if ((edx & bit_SSE2) && sse_os_support ())
    do_test ();

  return 0;
}
#define PR71559_TEST sse2_test
#endif

#define N 16
float a[N] = { 5.0f, -3.0f, 1.0f, __builtin_nanf (""), 9.0f, 7.0f, -3.0f, -9.0f,
               -3.0f, -5.0f, -9.0f, __builtin_nanf (""), 0.5f, -0.5f, 0.0f, 0.0f };
float b[N] = { -5.0f, 3.0f, 1.0f, 7.0f, 8.0f, 8.0f, -3.0f, __builtin_nanf (""),
               -4.0f, -4.0f, -9.0f, __builtin_nanf (""), 0.0f, 0.0f, 0.0f, __builtin_nanf ("") };
int c[N], d[N];

#define FN(name, op) \
void					\
name (void)				\
{					\
  int i;				\
  for (i = 0; i < N; i++)		\
    c[i] = (op || d[i] > 37) ? 5 : 32;	\
}
FN (eq, a[i] == b[i])
FN (ne, a[i] != b[i])
FN (gt, a[i] > b[i])
FN (ge, a[i] >= b[i])
FN (lt, a[i] < b[i])
FN (le, a[i] <= b[i])
FN (unle, !__builtin_isgreater (a[i], b[i]))
FN (unlt, !__builtin_isgreaterequal (a[i], b[i]))
FN (unge, !__builtin_isless (a[i], b[i]))
FN (ungt, !__builtin_islessequal (a[i], b[i]))
FN (uneq, !__builtin_islessgreater (a[i], b[i]))
FN (ordered, !__builtin_isunordered (a[i], b[i]))
FN (unordered, __builtin_isunordered (a[i], b[i]))

#define TEST(name, GT, LT, EQ, UO) \
  name ();				\
  for (i = 0; i < N; i++)		\
    {					\
      int v;				\
      switch (i % 4)			\
	{				\
	case 0: v = GT ? 5 : 32; break;	\
	case 1: v = LT ? 5 : 32; break;	\
	case 2: v = EQ ? 5 : 32; break;	\
	case 3: v = UO ? 5 : 32; break;	\
	}				\
      if (c[i] != v)			\
	__builtin_abort ();		\
    }

void
PR71559_TEST (void)
{
  int i;
  asm volatile ("" : : "g" (a), "g" (b), "g" (c), "g" (d) : "memory");
  TEST (eq, 0, 0, 1, 0)
  TEST (ne, 1, 1, 0, 1)
  TEST (gt, 1, 0, 0, 0)
  TEST (ge, 1, 0, 1, 0)
  TEST (lt, 0, 1, 0, 0)
  TEST (le, 0, 1, 1, 0)
  TEST (unle, 0, 1, 1, 1)
  TEST (unlt, 0, 1, 0, 1)
  TEST (unge, 1, 0, 1, 1)
  TEST (ungt, 1, 0, 0, 1)
  TEST (uneq, 0, 0, 1, 1)
  TEST (ordered, 1, 1, 1, 0)
  TEST (unordered, 0, 0, 0, 1)
}
