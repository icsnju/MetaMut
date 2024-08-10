/* Test for asm_fprintf formats.  */
/* Origin: Kaveh Ghazi <ghazi@caip.rutgers.edu> */
/* { dg-do compile } */
/* { dg-options "-Wformat" } */

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

/* Magic identifier must be set before the attribute is used.  */
typedef long long __gcc_host_wide_int__;

extern int asm_fprintf (const char *, ...) __attribute__ ((__format__ (__asm_fprintf__, 1, 2))) __attribute__ ((__nonnull__));

void
foo (int i, int i1, int i2, unsigned int u, double d, char *s, void *p,
     int *n, short int *hn, long int l, unsigned long int ul,
     long int *ln, long double ld, wint_t lc, wchar_t *ls, llong ll,
     ullong ull, unsigned int *un, const int *cn, signed char *ss,
     unsigned char *us, const signed char *css, unsigned int u1,
     unsigned int u2)
{
  /* Acceptable C90 specifiers, flags and modifiers.  */
  asm_fprintf ("%%");
  asm_fprintf ("%d%i%o%u%x%X%c%s%%", i, i, u, u, u, u, i, s);
  asm_fprintf ("%ld%li%lo%lu%lx%lX", l, l, ul, ul, ul, ul);
  asm_fprintf ("%lld%lli%llo%llu%llx%llX", ll, ll, ull, ull, ull, ull);
  asm_fprintf ("%-d%-i%-o%-u%-x%-X%-c%-s", i, i, u, u, u, u, i, s);
  asm_fprintf ("% d% i\n", i, i);
  asm_fprintf ("%#o%#x%#X", u, u, u);
  asm_fprintf ("%08d%08i%08o%08u%08x%08X", i, i, u, u, u, u);
  asm_fprintf ("%d\n", i);
  asm_fprintf ("%+d\n", i);
  asm_fprintf ("%3d\n", i);
  asm_fprintf ("%-3d\n", i);
  asm_fprintf ("%.7d\n", i);
  asm_fprintf ("%+9.4d\n", i);
  asm_fprintf ("%.3ld\n", l);
  asm_fprintf ("%d %lu\n", i, ul);

  /* Extensions provided in asm_fprintf.  */
  asm_fprintf ("%O%R%I%L%U%@");
  asm_fprintf ("%r", i);
  asm_fprintf ("%wd%wi%wo%wu%wx%wX", ll, ll, ull, ull, ull, ull);

  /* Standard specifiers not accepted in asm_fprintf.  */
  asm_fprintf ("%f\n", d); /* { dg-warning "18:format" "float" } */
  asm_fprintf ("%e\n", d); /* { dg-warning "18:format" "float" } */
  asm_fprintf ("%E\n", d); /* { dg-warning "18:format" "float" } */
  asm_fprintf ("%g\n", d); /* { dg-warning "18:format" "float" } */
  asm_fprintf ("%G\n", d); /* { dg-warning "18:format" "float" } */
  asm_fprintf ("%p\n", p); /* { dg-warning "18:format" "pointer" } */
  asm_fprintf ("%n\n", n); /* { dg-warning "18:format" "counter" } */
  asm_fprintf ("%hd\n", i); /* { dg-warning "18:format" "conversion" } */

  /* Various tests of bad argument types.  */
  asm_fprintf ("%d", l); /* { dg-warning "18:format" "bad argument types" } */
  asm_fprintf ("%wd", l); /* { dg-warning "19:format" "bad argument types" } */
  asm_fprintf ("%d", ll); /* { dg-warning "18:format" "bad argument types" } */
  asm_fprintf ("%*d\n", i1, i); /* { dg-warning "18:format" "bad * argument types" } */
  asm_fprintf ("%.*d\n", i2, i); /* { dg-warning "19:format" "bad * argument types" } */
  asm_fprintf ("%*.*ld\n", i1, i2, l); /* { dg-warning "18:format" "bad * argument types" } */
  asm_fprintf ("%ld", i); /* { dg-warning "19:format" "bad argument types" } */
  asm_fprintf ("%s", n); /* { dg-warning "18:format" "bad argument types" } */

  /* Wrong number of arguments.  */
  asm_fprintf ("%d%d", i); /* { dg-warning "20:matching" "wrong number of args" } */
  asm_fprintf ("%d", i, i); /* { dg-warning "16:arguments" "wrong number of args" } */
  /* Miscellaneous bogus constructions.  */
  asm_fprintf (""); /* { dg-warning "16:zero-length" "warning for empty format" } */
  asm_fprintf ("\0"); /* { dg-warning "17:embedded" "warning for embedded NUL" } */
  asm_fprintf ("%d\0", i); /* { dg-warning "19:embedded" "warning for embedded NUL" } */
  asm_fprintf ("%d\0%d", i, i); /* { dg-warning "19:embedded|too many" "warning for embedded NUL" } */
  asm_fprintf (NULL); /* { dg-warning "null" "null format string warning" } */
  asm_fprintf ("%"); /* { dg-warning "17:trailing" "trailing % warning" } */
  asm_fprintf ("%++d", i); /* { dg-warning "19:repeated" "repeated flag warning" } */
  asm_fprintf ((const char *)L"foo"); /* { dg-warning "30:wide" "wide string" } */
  asm_fprintf ("%s", (char *)0); /* { dg-warning "null" "%s with NULL" } */

  /* Make sure we still get warnings for regular printf.  */
  printf ("%d\n", ll); /* { dg-warning "13:format" "bad argument types" } */
}
/* { dg-warning "16:too many arguments for format" "too many arguments" { target *-*-* } 0 } */
