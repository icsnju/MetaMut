/* PR tree-optimization/57233 */
/* { dg-do run { target sse2 } } */
/* { dg-options "-O2 -msse2" } */

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

int do_main (void);

static void
sse2_test (void)
{
  do_main ();
}

#undef main
#define main() do_main ()

/* PR tree-optimization/57233 */
/* { dg-do run { target { ilp32 || lp64 } } } */
/* { dg-options "-O2" } */
/* { dg-additional-options "-fno-common" { target hppa*-*-hpux* } } */

typedef unsigned V4 __attribute__((vector_size(4 * sizeof (int))));
typedef unsigned V8 __attribute__((vector_size(8 * sizeof (int))));
typedef unsigned V16 __attribute__((vector_size(16 * sizeof (int))));
V4 a, b, g;
V8 c, d, h;
V16 e, f, j;

__attribute__((noinline)) void
f1 (void)
{
  a = (a << 2) | (a >> 30);
}

__attribute__((noinline)) void
f2 (void)
{
  a = (a << 30) | (a >> 2);
}

__attribute__((noinline)) void
f3 (void)
{
  a = (a << b) | (a >> (32 - b));
}

__attribute__((noinline, noclone)) void
f4 (int x)
{
  a = (a << x) | (a >> (32 - x));
}

__attribute__((noinline)) void
f5 (void)
{
  c = (c << 2) | (c >> 30);
}

__attribute__((noinline)) void
f6 (void)
{
  c = (c << 30) | (c >> 2);
}

__attribute__((noinline)) void
f7 (void)
{
  c = (c << d) | (c >> (32 - d));
}

__attribute__((noinline, noclone)) void
f8 (int x)
{
  c = (c << x) | (c >> (32 - x));
}

__attribute__((noinline)) void
f9 (void)
{
  e = (e << 2) | (e >> 30);
}

__attribute__((noinline)) void
f10 (void)
{
  e = (e << 30) | (e >> 2);
}

__attribute__((noinline)) void
f11 (void)
{
  e = (e << f) | (e >> (32 - f));
}

__attribute__((noinline, noclone)) void
f12 (int x)
{
  e = (e << x) | (e >> (32 - x));
}

unsigned
r (void)
{
  static unsigned x = 0xdeadbeefU;
  static unsigned y = 0x12347654U;
  static unsigned z = 0x1a2b3c4dU;
  static unsigned w = 0x87654321U;
  unsigned t = x ^ (x << 11);
  x = y;
  y = z;
  z = w;
  w = w ^ (w >> 19) ^ t ^ (t >> 8);
  return w;
}

void
init (unsigned int *p, int count, int mod)
{
  int i;
  for (i = 0; i < count; i++)
    {
      unsigned int v = r ();
      if (mod)
	v = (v % 31) + 1;
      p[i] = v;
    }
}

void
check (unsigned int *p, unsigned int *q, int count, unsigned int *s, int ss)
{
  int i;
  for (i = 0; i < count; i++)
    {
      if (s)
	ss = s[i];
      if (p[i] != ((q[i] << ss) | (q[i] >> (32 - ss))))
	__builtin_abort ();
    }
}

int
main ()
{
  init ((unsigned int *) &a, 4, 0);
  init ((unsigned int *) &b, 4, 1);
  init ((unsigned int *) &c, 8, 0);
  init ((unsigned int *) &d, 8, 1);
  init ((unsigned int *) &e, 16, 0);
  init ((unsigned int *) &f, 16, 1);
  g = a;
  h = c;
  j = e;
  f1 ();
  f5 ();
  f9 ();
  check ((unsigned int *) &a, (unsigned int *) &g, 4, 0, 2);
  check ((unsigned int *) &c, (unsigned int *) &h, 8, 0, 2);
  check ((unsigned int *) &e, (unsigned int *) &j, 16, 0, 2);
  g = a;
  h = c;
  j = e;
  f2 ();
  f6 ();
  f10 ();
  check ((unsigned int *) &a, (unsigned int *) &g, 4, 0, 30);
  check ((unsigned int *) &c, (unsigned int *) &h, 8, 0, 30);
  check ((unsigned int *) &e, (unsigned int *) &j, 16, 0, 30);
  g = a;
  h = c;
  j = e;
  f3 ();
  f7 ();
  f11 ();
  check ((unsigned int *) &a, (unsigned int *) &g, 4, (unsigned int *) &b, 0);
  check ((unsigned int *) &c, (unsigned int *) &h, 8, (unsigned int *) &d, 0);
  check ((unsigned int *) &e, (unsigned int *) &j, 16, (unsigned int *) &f, 0);
  g = a;
  h = c;
  j = e;
  f4 (5);
  f8 (5);
  f12 (5);
  check ((unsigned int *) &a, (unsigned int *) &g, 4, 0, 5);
  check ((unsigned int *) &c, (unsigned int *) &h, 8, 0, 5);
  check ((unsigned int *) &e, (unsigned int *) &j, 16, 0, 5);
  return 0;
}
