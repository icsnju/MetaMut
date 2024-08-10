/* { dg-do run } */
/* { dg-options "-O2 -msse" } */
/* { dg-require-effective-target sse } */

#include <stdlib.h>
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
#include "cpuid.h"
/* Check if the OS supports executing SSE instructions.  */

static int
sse_os_support (void)
{
  /* All currently supported OSes do.  */
  return 1;
}

static void sse_test (void);

static void
__attribute__ ((noinline))
do_test (void)
{
  sse_test ();
}

int
main ()
{
  unsigned int eax, ebx, ecx, edx;
 
  if (!__get_cpuid (1, &eax, &ebx, &ecx, &edx))
    return 0;

  /* Run SSE test only if host has SSE support.  */
  if ((edx & bit_SSE) && sse_os_support ())
    do_test ();

  return 0;
}

#include <xmmintrin.h>
#include <string.h>

#define SHIFT (4)

typedef union {
  __m64 v;
  unsigned char c[8];
  unsigned short int s[4];
  unsigned long long t;
  unsigned int u[2];
}vecInWord;

void sse_tests (void) __attribute__((noinline));
void dump64_16 (char *, char *, vecInWord);
int check (const char *, const char *[]);

char buf[8000];
char comparison[8000];
static int errors = 0;

vecInWord c64, e64;
__m64 m64_64;

const char *reference_sse[] = {
  "_mm_shuffle_pi16 0123 4567 89ab cdef \n",
  ""
};

static void
sse_test (void)
{
  e64.t  = 0x0123456789abcdefULL;

  m64_64 = e64.v;

  sse_tests();
  check (buf, reference_sse);
#ifdef DEBUG
  printf ("sse testing:\n");
  printf (buf);
  printf ("\ncomparison:\n");
  printf (comparison);
#endif
  buf[0] = '\0';

  if (errors != 0)
    abort ();
}

void __attribute__((noinline))
sse_tests (void)
{
  /* pshufw */
  c64.v = _mm_shuffle_pi16 (m64_64, 0x1b);
  dump64_16 (buf, "_mm_shuffle_pi16", c64);
}

void
dump64_16 (char *buf, char *name, vecInWord x)
{
  int i;
  char *p = buf + strlen (buf);

  sprintf (p, "%s ", name);
  p += strlen (p);

  for (i=0; i<4; i++)
    {
      sprintf (p, "%4.4x ", x.s[i]);
      p += strlen (p);
    }
  strcat (p, "\n");
}

int
check (const char *input, const char *reference[])
{
  int broken, i, j, len;
  const char *p_input;
  char *p_comparison;
  int new_errors = 0;

  p_comparison = &comparison[0];
  p_input = input;

  for (i = 0; *reference[i] != '\0'; i++)
    {
      broken = 0;
      len = strlen (reference[i]);
      for (j = 0; j < len; j++)
	{
	  /* Ignore the terminating NUL characters at the end of every string in 'reference[]'.  */
	  if (!broken && *p_input != reference[i][j])
	    {
	      *p_comparison = '\0';
	      strcat (p_comparison, " >>> ");
	      p_comparison += strlen (p_comparison);
	      new_errors++;
	      broken = 1;
	    }
	  *p_comparison = *p_input;
	  p_comparison++;
	  p_input++;
	}
      if (broken)
	{
	  *p_comparison = '\0';
	  strcat (p_comparison, "expected:\n");
	  strcat (p_comparison, reference[i]);
	  p_comparison += strlen (p_comparison);
	}
    }
  *p_comparison = '\0';
  strcat (p_comparison, new_errors ? "failure\n\n" : "O.K.\n\n") ;
  errors += new_errors;
  return 0;
}
