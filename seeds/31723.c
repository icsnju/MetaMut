/* { dg-do run } */
/* { dg-options "-O2 -msse2" } */
/* { dg-require-effective-target sse2 } */

#include <stdarg.h>
#include <assert.h>

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

struct m128
{
  __m128 v;
};

struct m128 n1 = { { -283.3, -23.3, 213.4, 1119.03 } };
__m128d n2 = { -93.83, 893.318 };
__m128i n3 = { 893, -3180 } ;
int n4 = 324;
double n5 = 103.3;
__m128i n6 = { -123, 2 };
__m128d n7 = { -91.387, -8193.518 };
struct m128 n8 = { { -123.3, 2.3, 3.4, -10.03 } };
__m128i n9 = { 1233, -100 };
int n10 = 407;
double n11 = 304.9;
__m128i n12 = { 233, -110 };
__m128i n13 = { -393, -180 };
__m128d n14 = { 73.0, 63.18 };
struct m128 n15 = { { -183.3, 22.3, 13.4, -19.03 } };

struct m128 e1;
__m128d e2;
__m128i e3;
int e4;
double e5;
__m128i e6;
__m128d e7;
struct m128 e8;
__m128i e9;
int e10;
double e11;
__m128i e12;
__m128i e13;
__m128d e14;
struct m128 e15;

static void
__attribute__((noinline))
test (struct m128 a1, __m128d a2, __m128i a3, ...)
{
  va_list va_arglist;

  e1 = a1;
  e2 = a2;
  e3 = a3;
  va_start (va_arglist, a3);
  e4 = va_arg (va_arglist, int);
  e5 = va_arg (va_arglist, double);
  e6 = va_arg (va_arglist, __m128i);
  e7 = va_arg (va_arglist, __m128d);
  e8 = va_arg (va_arglist, struct m128);
  e9 = va_arg (va_arglist, __m128i);
  e10 = va_arg (va_arglist, int);
  e11 = va_arg (va_arglist, double);
  e12 = va_arg (va_arglist, __m128i);
  e13 = va_arg (va_arglist, __m128i);
  e14 = va_arg (va_arglist, __m128d);
  e15 = va_arg (va_arglist, struct m128);
  va_end (va_arglist);
}

static void
sse2_test (void)
{
  test (n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12, n13, n14, n15);
  assert (__builtin_memcmp (&e1, &n1, sizeof (e1)) == 0);
  assert (__builtin_memcmp (&e2, &n2, sizeof (e2)) == 0);
  assert (__builtin_memcmp (&e3, &n3, sizeof (e3)) == 0);
  assert (n4 == e4);
  assert (n5 == e5);
  assert (__builtin_memcmp (&e6, &n6, sizeof (e6)) == 0);
  assert (__builtin_memcmp (&e7, &n7, sizeof (e7)) == 0);
  assert (__builtin_memcmp (&e8, &n8, sizeof (e8)) == 0);
  assert (__builtin_memcmp (&e9, &n9, sizeof (e9)) == 0);
  assert (n10 == e10);
  assert (n11 == e11);
  assert (__builtin_memcmp (&e12, &n12, sizeof (e12)) == 0);
  assert (__builtin_memcmp (&e13, &n13, sizeof (e13)) == 0);
  assert (__builtin_memcmp (&e14, &n14, sizeof (e14)) == 0);
  assert (__builtin_memcmp (&e15, &n15, sizeof (e15)) == 0);
}
