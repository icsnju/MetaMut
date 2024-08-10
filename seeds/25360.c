/* { dg-do run } */
/* { dg-options "-O2 -fno-strict-aliasing -msse2" } */
/* { dg-additional-options "-mno-mmx" { target { ! ia32 } } } */

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
/* Routine to check correctness of the results */

__attribute__((unused))
static int
saturate_b (int i)
{
  if (i > 127)
    i = 127;
  else if (i < -128)
    i = -128;
  return i;
}

__attribute__((unused))
static int
saturate_w (int i)
{
  if (i > 32767)
    i = 32767;
  else if (i < -32768)
    i = -32768;
  return i;
}

__attribute__((unused))
static int
saturate_ub (int i)
{
  if (i > 255)
    i = 255;
  else if (i < 0)
    i = 0;
  return i;
}

__attribute__((unused))
static int
saturate_uw (int i)
{
  if (i > 65535)
    i = 65535;
  else if (i < 0)
    i = 0;
  return i;
}

static long long MMXops[] =
{
  0x3467512347612976LL, 0x000000000000000eLL,
  0x3467512347612976LL, 0x0000000000000014LL,
  0x3467512347612976LL, 0x000000000000003cLL,
  0x0000000000000000LL, 0xFFFFFFFFFFFFFFFFLL,
  0xFFFFFFFFFFFFFFFFLL, 0x0000000000000000LL,
  0x0000000000000001LL, 0x1000000000000000LL,
  0x1000000000000000LL, 0x0000000000000001LL,
  0xFF00FF00FF00FF00LL, 0x00FF00FF00FF00FFLL,
  0xFFFFFFFFFFFFFFFFLL, 0x0101010101010101LL,
  0x0101010101010101LL, 0xFFFFFFFFFFFFFFFFLL,
  0x0123456789ABCDEFLL, 0x0123456789ABCDEFLL,
  0x3467512347612976LL, 0x1839876340879234LL,
  0x0000000000000000LL, 0x0000000000000000LL,
  0xFFFFFFFFFFFFFFFFLL, 0xFFFFFFFFFFFFFFFFLL,
  0x7F7F7F7F7F7F7F7FLL, 0x7F7F7F7F7F7F7F7FLL,
  0x7F7F7F7F7F7F7F7FLL, 0x0101010101010101LL,
  0x7F7F7F7F7F7F7F7FLL, 0x4782082349761237LL,
  0x0000000000000000LL, 0x7F7F7F7F7F7F7F7FLL,
  0x8080808080808080LL, 0x8080808080808080LL,
  0x0101010101010101LL, 0x8080808080808080LL,
  0x8080808080808080LL, 0x0000000000000000LL,
  0x2372347120982458LL, 0x8080808080808080LL,
  0xFFFFFFFFFFFFFFFFLL, 0x8080808080808080LL,
  0x7F7F7F7F7F7F7F7FLL, 0xFFFFFFFFFFFFFFFFLL,
  0x8080808080808080LL, 0x7F7F7F7F7F7F7F7FLL,
  0xFFFFFFFFFFFFFFFFLL, 0x7F7F7F7F7F7F7F7FLL
};

#define MMX_num_ops (sizeof (MMXops) / sizeof (MMXops[0]))

__attribute__((noinline, noclone))
static void
test_pcmpgtd  (long long *ll1, long long *ll2, long long *r)
{
  __m64 t1 = *(__m64 *) ll1;
  __m64 t2 = *(__m64 *) ll2;
  *(__m64 *) r = _m_pcmpgtd (t1, t2);
}

/* Routine to manually compute the results */
static void
compute_correct_result (long long *dst_p, long long *src_p,
			long long *res_p)
{
  int *dst = (int *) dst_p;
  int *src = (int *) src_p;
  int *res = (int *) res_p;
  int i;
  for (i = 0; i < 2; i++)
    res[i] = dst[i] > src[i] ? -1 : 0;
}

static void
sse2_test (void)
{
  int i;
  long long r, ck;
  int fail = 0;

  /* Run the MMX tests */
  for (i = 0; i < MMX_num_ops; i += 2)
    {
      test_pcmpgtd (&MMXops[i], &MMXops[i + 1], &r);
      compute_correct_result (&MMXops[i], &MMXops[i + 1], &ck);
      if (ck != r)
	fail++;
    }

  if (fail != 0)
    abort ();
}
