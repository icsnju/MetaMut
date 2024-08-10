/* Test for scanf formats for Decimal Floating Point types.  */

/* { dg-do compile } */
/* { dg-require-effective-target dfp } */
/* { dg-options "-Wformat" } */
/* { dg-skip-if "No scanf/printf dfp support" { *-*-mingw* } } */

/* Format checking tests: common header.  */
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */

/* DONT_GNU_PROTOTYPE */
#if defined (_WIN32) && !defined (__CYGWIN__)
#if !defined (USE_SYSTEM_FORMATS)
#define gnu_attr_printf	gnu_printf
#define gnu_attr___printf__ __gnu_printf__
#define gnu_attr_scanf	gnu_scanf
#define gnu_attr___scanf__ __gnu_scanf__
#define gnu_attr_strftime gnu_strftime
#define gnu_attr___strftime__ __gnu_strftime__
#endif
#endif

#ifndef gnu_attr_printf
#define gnu_attr_printf	printf
#define gnu_attr___printf__ __printf__
#define gnu_attr_scanf	scanf
#define gnu_attr___scanf__ __scanf__
#define gnu_attr_strftime strftime
#define gnu_attr___strftime__ __strftime__
#endif

#if !defined (USE_SYSTEM_FORMATS)
#define USE_PRINTF(FMTPOS, WILDARG) __attribute__((format(gnu_printf, FMTPOS, WILDARG))) __attribute__((nonnull (FMTPOS)))
#define USE_SCANF(FMTPOS, WILDARG) __attribute__((format(gnu_scanf, FMTPOS, WILDARG))) __attribute__((nonnull (FMTPOS)))
#define USE_STRFTIME(FMTPOS) __attribute__((__format__(gnu_strftime, FMTPOS, 0))) __attribute__((nonnull (FMTPOS)))
#else
#define USE_PRINTF(FMTPOS, WILDARG)
#define USE_SCANF(FMTPOS, WILDARG)
#define USE_STRFTIME(FMTPOS)
#endif

#include <stdarg.h>
#include <stddef.h>

#ifndef _WINT_T
#ifndef __WINT_TYPE__
#define __WINT_TYPE__ unsigned int
#endif
typedef __WINT_TYPE__ wint_t;
#endif

#ifdef _WIN64
/* Kludges to get types corresponding to size_t and ptrdiff_t.  */
#define unsigned signed
typedef signed int signed_size_t __attribute__ ((mode (DI)));
/* We also use this type to approximate ssize_t.  */
typedef signed int ssize_t __attribute__ ((mode (DI)));
#undef unsigned
#define signed /* Type might or might not have explicit 'signed'.  */
typedef unsigned int unsigned_ptrdiff_t __attribute__ ((mode (DI)));
#undef signed

__extension__ typedef int llong  __attribute__ ((mode (DI)));
__extension__ typedef unsigned int ullong  __attribute__ ((mode (DI)));
#else
/* Kludges to get types corresponding to size_t and ptrdiff_t.  */
#define unsigned signed
typedef __SIZE_TYPE__ signed_size_t;
/* We also use this type to approximate ssize_t.  */
typedef __SIZE_TYPE__ ssize_t;
#undef unsigned
#define signed /* Type might or might not have explicit 'signed'.  */
typedef unsigned __PTRDIFF_TYPE__ unsigned_ptrdiff_t;
#undef signed

__extension__ typedef long long int llong;
__extension__ typedef unsigned long long int ullong;
#endif

/* %q formats want a "quad"; GCC considers this to be a long long.  */
typedef llong quad_t;
typedef ullong u_quad_t;

__extension__ typedef __INTMAX_TYPE__ intmax_t;
__extension__ typedef __UINTMAX_TYPE__ uintmax_t;

__extension__ typedef __INT_LEAST8_TYPE__ int_least8_t;
__extension__ typedef __INT_LEAST16_TYPE__ int_least16_t;
__extension__ typedef __INT_LEAST32_TYPE__ int_least32_t;
__extension__ typedef __INT_LEAST64_TYPE__ int_least64_t;
__extension__ typedef __UINT_LEAST8_TYPE__ uint_least8_t;
__extension__ typedef __UINT_LEAST16_TYPE__ uint_least16_t;
__extension__ typedef __UINT_LEAST32_TYPE__ uint_least32_t;
__extension__ typedef __UINT_LEAST64_TYPE__ uint_least64_t;

__extension__ typedef __INT_FAST8_TYPE__ int_fast8_t;
__extension__ typedef __INT_FAST16_TYPE__ int_fast16_t;
__extension__ typedef __INT_FAST32_TYPE__ int_fast32_t;
__extension__ typedef __INT_FAST64_TYPE__ int_fast64_t;
__extension__ typedef __UINT_FAST8_TYPE__ uint_fast8_t;
__extension__ typedef __UINT_FAST16_TYPE__ uint_fast16_t;
__extension__ typedef __UINT_FAST32_TYPE__ uint_fast32_t;
__extension__ typedef __UINT_FAST64_TYPE__ uint_fast64_t;

#if __STDC_VERSION__ < 199901L && !defined(restrict)
#define restrict /* "restrict" not in old C standard.  */
#endif

/* This may not be correct in the particular case, but allows the
   prototypes to be declared, and we don't try to link.
*/
typedef struct _FILE FILE;
extern FILE *stdin;
extern FILE *stdout;

extern int fprintf (FILE *restrict, const char *restrict, ...);
extern int printf (const char *restrict, ...);
extern int fprintf_unlocked (FILE *restrict, const char *restrict, ...);
extern int printf_unlocked (const char *restrict, ...);
extern int sprintf (char *restrict, const char *restrict, ...);
extern int vfprintf (FILE *restrict, const char *restrict, va_list);
extern int vprintf (const char *restrict, va_list);
extern int vsprintf (char *restrict, const char *restrict, va_list);
extern int snprintf (char *restrict, size_t, const char *restrict, ...);
extern int vsnprintf (char *restrict, size_t, const char *restrict, va_list);

extern int fscanf (FILE *restrict, const char *restrict, ...);
extern int scanf (const char *restrict, ...);
extern int sscanf (const char *restrict, const char *restrict, ...);
extern int vfscanf (FILE *restrict, const char *restrict, va_list);
extern int vscanf (const char *restrict, va_list);
extern int vsscanf (const char *restrict, const char *restrict, va_list);

extern char *gettext (const char *);
extern char *dgettext (const char *, const char *);
extern char *dcgettext (const char *, const char *, int);

struct tm;

extern size_t strftime (char *restrict, size_t, const char *restrict,
			const struct tm *restrict);

extern ssize_t strfmon (char *restrict, size_t, const char *restrict, ...);

/* Mingw specific part.  */
#if !defined (USE_SYSTEM_FORMATS) && defined(_WIN32) && !defined(DONT_GNU_PROTOTYPE)

extern USE_PRINTF(2,3) int fprintf_gnu (FILE *restrict, const char *restrict, ...);
#undef fprintf
#define fprintf fprintf_gnu

extern USE_PRINTF(1,2) int printf_gnu (const char *restrict, ...);
#undef printf
#define printf printf_gnu

extern USE_PRINTF(2,3) int fprintf_unlocked_gnu (FILE *restrict, const char *restrict, ...);
#undef fprintf_unlocked
#define fprintf_unlocked fprintf_unlocked_gnu

extern USE_PRINTF(1,2)int printf_unlocked_gnu (const char *restrict, ...);
#undef printf_unlocked
#define printf_unlocked printf_unlocked_gnu

extern USE_PRINTF(2,3) int sprintf_gnu (char *restrict, const char *restrict, ...);
#undef sprintf
#define sprintf sprintf_gnu

extern USE_PRINTF(2,0) int vfprintf_gnu (FILE *restrict, const char *restrict, va_list);
#undef vsprintf
#define vsprintf vsprintf_gnu

extern USE_PRINTF(1,0) int vprintf_gnu (const char *restrict, va_list);
#undef vprintf
#define vprintf vprintf_gnu

extern USE_PRINTF(2,0) int vsprintf_gnu (char *restrict, const char *restrict, va_list);
#undef vsprintf
#define vsprintf vsprintf_gnu

extern USE_PRINTF(3,4) int snprintf_gnu (char *restrict, size_t, const char *restrict, ...);
#undef snprintf
#define snprintf snprintf_gnu

extern USE_PRINTF(3,0) int vsnprintf_gnu (char *restrict, size_t, const char *restrict, va_list);
#undef vsnprintf
#define vsnprintf vsnprintf_gnu

extern USE_SCANF(2,3) int fscanf_gnu (FILE *restrict, const char *restrict, ...);
#undef fscanf
#define fscanf fscanf_gnu

extern USE_SCANF(1,2) int scanf_gnu (const char *restrict, ...);
#undef scanf
#define scanf scanf_gnu

extern USE_SCANF(2,3) int sscanf_gnu (const char *restrict, const char *restrict, ...);
#undef sscanf
#define sscanf sscanf_gnu

extern USE_SCANF(2,0) int vfscanf_gnu (FILE *restrict, const char *restrict, va_list);
#undef vfscanf
#define vfscanf vfscanf_gnu

extern USE_SCANF(1,0) int vscanf_gnu (const char *restrict, va_list);
#undef vscanf
#define vscanf vscanf_gnu

extern USE_SCANF(2,0) int vsscanf_gnu (const char *restrict, const char *restrict, va_list);
#undef vsscanf
#define vsscanf vsscanf_gnu

extern USE_STRFTIME(3) size_t strftime_gnu (char *restrict, size_t, const char *restrict,
			const struct tm *restrict);
#undef strftime
#define strftime strftime_gnu

#endif

void
voo (_Decimal32 *x, _Decimal64 *y, _Decimal128 *z, int *i, unsigned int *j,
     double *d, char **p)
{
  /* See ISO/IEC DTR 24732 subclause 9.3 (currently Working Draft 5 from
      2005-03-06).  */
  /* Formatted input/output specifiers.  */

  /* Check lack of warnings for valid usage.  */

  scanf ("%Ha", x);
  scanf ("%HA", x);
  scanf ("%Hf", x);
  scanf ("%HF", x);
  scanf ("%He", x);
  scanf ("%HE", x);
  scanf ("%Hg", x);
  scanf ("%HG", x);

  scanf ("%Da", y);
  scanf ("%DA", y);
  scanf ("%Df", y);
  scanf ("%DF", y);
  scanf ("%De", y);
  scanf ("%DE", y);
  scanf ("%Dg", y);
  scanf ("%DG", y);

  scanf ("%DDa", z);
  scanf ("%DDA", z);
  scanf ("%DDf", z);
  scanf ("%DDF", z);
  scanf ("%DDe", z);
  scanf ("%DDE", z);
  scanf ("%DDg", z);
  scanf ("%DDG", z);

  scanf ("%DG%DDE%HF%DDe%He%HE%DF%DDF%De%DDG%HG%Df%Hg%DE%DDf%Dg%DDg%Hf\n",
           y, z, x, z, x, x, y, z, y, z, x, y, x, y, z, y, z, x);

  /* Check warnings for type mismatches.  */

  scanf ("%Ha", y);	/* { dg-warning "expects argument" "bad use of %H" } */
  scanf ("%HA", y);	/* { dg-warning "expects argument" "bad use of %H" } */
  scanf ("%Hf", y);	/* { dg-warning "expects argument" "bad use of %H" } */
  scanf ("%HF", y);	/* { dg-warning "expects argument" "bad use of %H" } */
  scanf ("%He", y);	/* { dg-warning "expects argument" "bad use of %H" } */
  scanf ("%HE", y);	/* { dg-warning "expects argument" "bad use of %H" } */
  scanf ("%Hg", y);	/* { dg-warning "expects argument" "bad use of %H" } */
  scanf ("%HG", y);	/* { dg-warning "expects argument" "bad use of %H" } */
  scanf ("%Ha", z);	/* { dg-warning "expects argument" "bad use of %H" } */
  scanf ("%HA", z);	/* { dg-warning "expects argument" "bad use of %H" } */
  scanf ("%Hf", z);	/* { dg-warning "expects argument" "bad use of %H" } */
  scanf ("%HF", z);	/* { dg-warning "expects argument" "bad use of %H" } */
  scanf ("%He", z);	/* { dg-warning "expects argument" "bad use of %H" } */
  scanf ("%HE", z);	/* { dg-warning "expects argument" "bad use of %H" } */
  scanf ("%Hg", z);	/* { dg-warning "expects argument" "bad use of %H" } */
  scanf ("%HG", z);	/* { dg-warning "expects argument" "bad use of %H" } */

  scanf ("%Da", x);	/* { dg-warning "expects argument" "bad use of %D" } */
  scanf ("%DA", x);	/* { dg-warning "expects argument" "bad use of %D" } */
  scanf ("%Df", x);	/* { dg-warning "expects argument" "bad use of %D" } */
  scanf ("%DF", x);	/* { dg-warning "expects argument" "bad use of %D" } */
  scanf ("%De", x);	/* { dg-warning "expects argument" "bad use of %D" } */
  scanf ("%DE", x);	/* { dg-warning "expects argument" "bad use of %D" } */
  scanf ("%Dg", x);	/* { dg-warning "expects argument" "bad use of %D" } */
  scanf ("%DG", x);	/* { dg-warning "expects argument" "bad use of %D" } */
  scanf ("%Da", z);	/* { dg-warning "expects argument" "bad use of %D" } */
  scanf ("%DA", z);	/* { dg-warning "expects argument" "bad use of %D" } */
  scanf ("%Df", z);	/* { dg-warning "expects argument" "bad use of %D" } */
  scanf ("%DF", z);	/* { dg-warning "expects argument" "bad use of %D" } */
  scanf ("%De", z);	/* { dg-warning "expects argument" "bad use of %D" } */
  scanf ("%DE", z);	/* { dg-warning "expects argument" "bad use of %D" } */
  scanf ("%Dg", z);	/* { dg-warning "expects argument" "bad use of %D" } */
  scanf ("%DG", z);	/* { dg-warning "expects argument" "bad use of %D" } */

  scanf ("%DDa", x);	/* { dg-warning "expects argument" "bad use of %DD" } */
  scanf ("%DDA", x);	/* { dg-warning "expects argument" "bad use of %DD" } */
  scanf ("%DDf", x);	/* { dg-warning "expects argument" "bad use of %DD" } */
  scanf ("%DDF", x);	/* { dg-warning "expects argument" "bad use of %DD" } */
  scanf ("%DDe", x);	/* { dg-warning "expects argument" "bad use of %DD" } */
  scanf ("%DDE", x);	/* { dg-warning "expects argument" "bad use of %DD" } */
  scanf ("%DDg", x);	/* { dg-warning "expects argument" "bad use of %DD" } */
  scanf ("%DDG", x);	/* { dg-warning "expects argument" "bad use of %DD" } */
  scanf ("%DDa", y);	/* { dg-warning "expects argument" "bad use of %DD" } */
  scanf ("%DDA", y);	/* { dg-warning "expects argument" "bad use of %DD" } */
  scanf ("%DDf", y);	/* { dg-warning "expects argument" "bad use of %DD" } */
  scanf ("%DDF", y);	/* { dg-warning "expects argument" "bad use of %DD" } */
  scanf ("%DDe", y);	/* { dg-warning "expects argument" "bad use of %DD" } */
  scanf ("%DDE", y);	/* { dg-warning "expects argument" "bad use of %DD" } */
  scanf ("%DDg", y);	/* { dg-warning "expects argument" "bad use of %DD" } */
  scanf ("%DDG", y);	/* { dg-warning "expects argument" "bad use of %DD" } */

  /* Check for warnings for bad use of H, D, and DD length specifiers.  */

  scanf ("%Hd\n", i);	/* { dg-warning "length" "bad use of %H" } */
  scanf ("%Hi\n", i);	/* { dg-warning "length" "bad use of %H" } */
  scanf ("%Ho\n", j);	/* { dg-warning "length" "bad use of %H" } */
  scanf ("%Hu\n", j);	/* { dg-warning "length" "bad use of %H" } */
  scanf ("%Hx\n", j);	/* { dg-warning "length" "bad use of %H" } */
  scanf ("%HX\n", j);	/* { dg-warning "length" "bad use of %H" } */
  scanf ("%Ha\n", d);	/* { dg-warning "expects argument" "bad use of %H" } */
  scanf ("%HA\n", d);	/* { dg-warning "expects argument" "bad use of %H" } */
  scanf ("%Hc\n", i);	/* { dg-warning "length" "bad use of %H" } */
  scanf ("%Hs\n", p);	/* { dg-warning "length" "bad use of %H" } */
  scanf ("%Hp\n", p);	/* { dg-warning "length" "bad use of %H" } */
  scanf ("%Hn\n", p);	/* { dg-warning "length" "bad use of %H" } */
}
