/* { dg-do run } */
/* { dg-options "-O2 -msse2" } */
/* { dg-require-effective-target sse2 } */

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

#ifdef DEBUG
#include <stdio.h>
#endif

#include <emmintrin.h>

static void
__attribute__((noinline))
check (__m128i x, unsigned short *v, int j)
{
  union
    {
      __m128i x;
      unsigned short i[8];
    } u;
  unsigned int i;

  u.x = x;
  
  for (i = 0; i < sizeof (u) / sizeof (v[0]); i++)
    if (i == j)
      {
	if (v[i] != u.i[i])
	  {
#ifdef DEBUG
	    printf ("%i: 0x%x != 0x%x\n", i, v[i], u.i[i]);
#endif
	    abort ();
	  }
      }
    else if (u.i[i] != 0)
      {
#ifdef DEBUG
	printf ("%i: 0x%x != 0\n", i, u.i[i]);
#endif
	abort ();
      }
}

static void
__attribute__((noinline))
test (unsigned short *v)
{
  __m128i x;

  x = _mm_set_epi16 (0, 0, 0, 0, 0, 0, 0, v[0]);
  check (x, v, 0);
  x = _mm_set_epi16 (0, 0, 0, 0, 0, 0, v[1], 0);
  check (x, v, 1);
  x = _mm_set_epi16 (0, 0, 0, 0, 0, v[2], 0, 0);
  check (x, v, 2);
  x = _mm_set_epi16 (0, 0, 0, 0, v[3], 0, 0, 0);
  check (x, v, 3);
  x = _mm_set_epi16 (0, 0, 0, v[4], 0, 0, 0, 0);
  check (x, v, 4);
  x = _mm_set_epi16 (0, 0, v[5], 0, 0, 0, 0, 0);
  check (x, v, 5);
  x = _mm_set_epi16 (0, v[6], 0, 0, 0, 0, 0, 0);
  check (x, v, 6);
  x = _mm_set_epi16 (v[7], 0, 0, 0, 0, 0, 0, 0);
  check (x, v, 7);
}

static void
sse2_test (void)
{
  unsigned short v[8]
    = { 0x7B5B, 0x5465, 0x7374, 0x5665,
	0x6374, 0x6F72, 0x5D53, 0x475D };
  test (v);
}
