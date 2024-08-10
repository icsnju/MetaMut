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

#include <emmintrin.h>
#include <string.h>

#define SHIFT (4)

typedef union {
  __m128i v;
  unsigned int s[4];
  unsigned short int t[8];
  unsigned long long u[2];
  unsigned char c[16];
}vecInLong;

void sse2_tests (void) __attribute__((noinline));
void dump128_16 (char *, char *, vecInLong);
void dump128_32 (char *, char *, vecInLong);
void dump128_64 (char *, char *, vecInLong);
void dump128_128 (char *, char *, vecInLong);
int check (const char *, const char *[]);

char buf[8000];
char comparison[8000];
static int errors = 0;

vecInLong a128, b128, c128, d128, e128, f128;
__m128i m128_16, m128_32, s128, m128_64, m128_128;
__m64 m64_16, s64, m64_32, m64_64;

const char *reference_sse2[] = {
  "_mm_srai_epi16 0012 0012 0012 0012 0012 0012 0012 0012 \n",
  "_mm_sra_epi16 0012 0012 0012 0012 0012 0012 0012 0012 \n",
  "_mm_srai_epi32 00123456 00123456 00123456 00123456 \n",
  "_mm_sra_epi32 00123456 00123456 00123456 00123456 \n",
  "_mm_srli_epi16 0012 0012 0012 0012 0012 0012 0012 0012 \n",
  "_mm_srl_epi16 0012 0012 0012 0012 0012 0012 0012 0012 \n",
  "_mm_srli_epi32 00123456 00123456 00123456 00123456 \n",
  "_mm_srl_epi32 00123456 00123456 00123456 00123456 \n",
  "_mm_srli_epi64 00123456789abcde 00123456789abcde \n",
  "_mm_srl_epi64 00123456789abcde 00123456789abcde \n",
  "_mm_srli_si128 (byte shift)  00000000ffeeddccbbaa998877665544\n",
  "_mm_slli_epi16 1230 1230 1230 1230 1230 1230 1230 1230 \n",
  "_mm_sll_epi16 1230 1230 1230 1230 1230 1230 1230 1230 \n",
  "_mm_slli_epi32 12345670 12345670 12345670 12345670 \n",
  "_mm_sll_epi32 12345670 12345670 12345670 12345670 \n",
  "_mm_slli_epi64 123456789abcdef0 123456789abcdef0 \n",
  "_mm_sll_epi64 123456789abcdef0 123456789abcdef0 \n",
  "_mm_sll_si128 (byte shift) bbaa9988776655443322110000000000\n",
  "_mm_shuffle_epi32 ffeeddcc bbaa9988 77665544 33221100 \n",
  "_mm_shuffelo_epi16 7766 5544 3322 1100 9988 bbaa ddcc ffee \n",
  "_mm_shuffehi_epi16 1100 3322 5544 7766 ffee ddcc bbaa 9988 \n",
  ""
};

static void
sse2_test (void)
{
  a128.s[0] = 0x01234567;
  a128.s[1] = 0x01234567;
  a128.s[2] = 0x01234567;
  a128.s[3] = 0x01234567;

  m128_32 = a128.v;

  d128.u[0] = 0x0123456789abcdefULL;
  d128.u[1] = 0x0123456789abcdefULL;

  m128_64 = d128.v;

  /* This is the 128-bit constant 0x00112233445566778899aabbccddeeff,
     expressed as two little-endian 64-bit words.  */
  e128.u[0] = 0x7766554433221100ULL;
  e128.u[1] = 0xffeeddccbbaa9988ULL;

  f128.t[0] = 0x0123;
  f128.t[1] = 0x0123;
  f128.t[2] = 0x0123;
  f128.t[3] = 0x0123;
  f128.t[4] = 0x0123;
  f128.t[5] = 0x0123;
  f128.t[6] = 0x0123;
  f128.t[7] = 0x0123;

  m128_16 = f128.v;

  m128_128 = e128.v;

  b128.s[0] = SHIFT;
  b128.s[1] = 0;
  b128.s[2] = 0;
  b128.s[3] = 0;

  s128 = b128.v;

  sse2_tests();
  check (buf, reference_sse2);
#ifdef DEBUG
  printf ("sse2 testing:\n");
  printf (buf);
  printf ("\ncomparison:\n");
  printf (comparison);
#endif
  buf[0] = '\0';

  if (errors != 0)
    abort ();
}

void __attribute__((noinline))
sse2_tests (void)
{
  /* psraw */
  c128.v = _mm_srai_epi16 (m128_16, SHIFT);
  dump128_16 (buf, "_mm_srai_epi16", c128);
  c128.v = _mm_sra_epi16 (m128_16, s128);
  dump128_16 (buf, "_mm_sra_epi16", c128);

  /* psrad */
  c128.v = _mm_srai_epi32 (m128_32, SHIFT);
  dump128_32 (buf, "_mm_srai_epi32", c128);
  c128.v = _mm_sra_epi32 (m128_32, s128);
  dump128_32 (buf, "_mm_sra_epi32", c128);

  /* psrlw */
  c128.v = _mm_srli_epi16 (m128_16, SHIFT);
  dump128_16 (buf, "_mm_srli_epi16", c128);
  c128.v = _mm_srl_epi16 (m128_16, s128);
  dump128_16 (buf, "_mm_srl_epi16", c128);

  /* psrld */
  c128.v = _mm_srli_epi32 (m128_32, SHIFT);
  dump128_32 (buf, "_mm_srli_epi32", c128);
  c128.v = _mm_srl_epi32 (m128_32, s128);
  dump128_32 (buf, "_mm_srl_epi32", c128);

  /* psrlq */
  c128.v = _mm_srli_epi64 (m128_64, SHIFT);
  dump128_64 (buf, "_mm_srli_epi64", c128);
  c128.v = _mm_srl_epi64 (m128_64, s128);
  dump128_64 (buf, "_mm_srl_epi64", c128);

  /* psrldq */
  c128.v = _mm_srli_si128 (m128_128, SHIFT);
  dump128_128 (buf, "_mm_srli_si128 (byte shift) ", c128);

  /* psllw */
  c128.v = _mm_slli_epi16 (m128_16, SHIFT);
  dump128_16 (buf, "_mm_slli_epi16", c128);
  c128.v = _mm_sll_epi16 (m128_16, s128);
  dump128_16 (buf, "_mm_sll_epi16", c128);

  /* pslld */
  c128.v = _mm_slli_epi32 (m128_32, SHIFT);
  dump128_32 (buf, "_mm_slli_epi32", c128);
  c128.v = _mm_sll_epi32 (m128_32, s128);
  dump128_32 (buf, "_mm_sll_epi32", c128);

  /* psllq */
  c128.v = _mm_slli_epi64 (m128_64, SHIFT);
  dump128_64 (buf, "_mm_slli_epi64", c128);
  c128.v = _mm_sll_epi64 (m128_64, s128);
  dump128_64 (buf, "_mm_sll_epi64", c128);

  /* pslldq */
  c128.v = _mm_slli_si128 (m128_128, SHIFT);
  dump128_128 (buf, "_mm_sll_si128 (byte shift)", c128);

  /* Shuffle constant 0x1b == 0b_00_01_10_11, e.g. swap words: ABCD => DCBA.  */

  /* pshufd */
  c128.v = _mm_shuffle_epi32 (m128_128, 0x1b);
  dump128_32 (buf, "_mm_shuffle_epi32", c128);

  /* pshuflw */
  c128.v = _mm_shufflelo_epi16 (m128_128, 0x1b);
  dump128_16 (buf, "_mm_shuffelo_epi16", c128);

  /* pshufhw */
  c128.v = _mm_shufflehi_epi16 (m128_128, 0x1b);
  dump128_16 (buf, "_mm_shuffehi_epi16", c128);
}

void
dump128_16 (char *buf, char *name, vecInLong x)
{
  int i;
  char *p = buf + strlen (buf);

  sprintf (p, "%s ", name);
  p += strlen (p);

  for (i=0; i<8; i++)
    {
      sprintf (p, "%4.4x ", x.t[i]);
      p += strlen (p);
    }
  strcat (p, "\n");
}

void
dump128_32 (char *buf, char *name, vecInLong x)
{
  int i;
  char *p = buf + strlen (buf);

  sprintf (p, "%s ", name);
  p += strlen (p);

  for (i=0; i<4; i++)
    {
      sprintf (p, "%8.8x ", x.s[i]);
      p += strlen (p);
    }
  strcat (p, "\n");
}

void
dump128_64 (char *buf, char *name, vecInLong x)
{
  int i;
  char *p = buf + strlen (buf);

  sprintf (p, "%s ", name);
  p += strlen (p);

  for (i=0; i<2; i++)
    {
#if defined(_WIN32) && !defined(__CYGWIN__)
      sprintf (p, "%16.16I64x ", x.u[i]);
#else
      sprintf (p, "%16.16llx ", x.u[i]);
#endif
      p += strlen (p);
    }
  strcat (p, "\n");
}

void
dump128_128 (char *buf, char *name, vecInLong x)
{
  int i;
  char *p = buf + strlen (buf);

  sprintf (p, "%s ", name);
  p += strlen (p);

  for (i=15; i>=0; i--)
    {
      /* This is cheating; we don't have a 128-bit int format code.
	 Running the loop backwards to compensate for the
	 little-endian layout. */
      sprintf (p, "%2.2x", x.c[i]);
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
