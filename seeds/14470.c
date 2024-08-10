/* PR target/63594 */
/* { dg-do run { target sse2 } } */
/* { dg-options "-O2 -msse2 -mno-mmx -Wno-psabi" } */

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

/* PR target/63594 */
/* { dg-do run } */
/* { dg-options "-O2 -Wno-psabi" } */
/* { dg-additional-options "-mno-mmx" { target i?86-*-* x86_64-*-* } } */
/* { dg-prune-output "non-standard ABI extension" } */

#define C1 c
#define C2 C1, C1
#define C4 C2, C2
#define C8 C4, C4
#define C16 C8, C8
#define C32 C16, C16
#define C64 C32, C32
#define C_(n) n
#define C(n) C_(C##n)

#define T(t,s) \
typedef t v##t##s __attribute__ ((__vector_size__ (s * sizeof (t))));	\
__attribute__((noinline, noclone)) v##t##s				\
test1##t##s (t c)							\
{									\
  v##t##s v = { C(s) };							\
  return v;								\
}									\
									\
__attribute__((noinline, noclone)) v##t##s				\
test2##t##s (t *p)							\
{									\
  t c = *p;								\
  v##t##s v = { C(s) };							\
  return v;								\
}									\
									\
void									\
test3##t##s (void)							\
{									\
  t c = 17;								\
  int i;								\
  v##t##s a = test1##t##s (c);						\
  for (i = 0; i < s; i++)						\
    if (a[i] != 17)							\
      __builtin_abort ();						\
  v##t##s b = test2##t##s (&c);						\
  for (i = 0; i < s; i++)						\
    if (a[i] != 17)							\
      __builtin_abort ();						\
}

typedef long long llong;

#define TESTS \
T(char, 64) \
T(char, 32) \
T(char, 16) \
T(char, 8) \
T(char, 4) \
T(char, 2) \
T(char, 1) \
T(short, 32) \
T(short, 16) \
T(short, 8) \
T(short, 4) \
T(short, 2) \
T(short, 1) \
T(int, 16) \
T(int, 8) \
T(int, 4) \
T(int, 2) \
T(int, 1) \
T(float, 16) \
T(float, 8) \
T(float, 4) \
T(float, 2) \
T(float, 1) \
T(llong, 8) \
T(llong, 4) \
T(llong, 2) \
T(llong, 1) \
T(double, 8) \
T(double, 4) \
T(double, 2) \
T(double, 1)

TESTS

int
main ()
{
#undef T
#define T(t,s) test3##t##s ();
  TESTS
  return 0;
}
