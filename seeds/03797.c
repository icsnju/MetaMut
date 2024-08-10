/* Test for strftime formats.  Formats using C99 features.  */
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1999 -pedantic -Wformat -Wformat-y2k" } */

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
foo (char *s, size_t m, const struct tm *tp)
{
  /* See ISO/IEC 9899:1990 (E) subclause 7.12.3.5 (pages 174-175).  */
  /* Formats which are Y2K-compliant (no 2-digit years).  */
  strftime (s, m, "%a%A%b%B%C%d%e%F%G%h%H%I%j%m%M%p%R%S%t%T%u%U%V%w%W%X%Y%z%Z%%", tp);
  strftime (s, m, "%EC%EX%EY%Od%Oe%OH%OI%Om%OM%OS%Ou%OU%OV%Ow%OW", tp);
  /* Formats with 2-digit years.  */
  strftime (s, m, "%D", tp); /* { dg-warning "only last 2" "2-digit year" } */
  strftime (s, m, "%g", tp); /* { dg-warning "only last 2" "2-digit year" } */
  strftime (s, m, "%y", tp); /* { dg-warning "only last 2" "2-digit year" } */
  strftime (s, m, "%Oy", tp); /* { dg-warning "only last 2" "2-digit year" } */
  /* Formats with 2-digit years in some locales.  */
  strftime (s, m, "%c", tp); /* { dg-warning "some locales" "2-digit year" } */
  strftime (s, m, "%Ec", tp); /* { dg-warning "some locales" "2-digit year" } */
  strftime (s, m, "%x", tp); /* { dg-warning "some locales" "2-digit year" } */
  strftime (s, m, "%Ex", tp); /* { dg-warning "some locales" "2-digit year" } */
  /* %Ey is explicitly an era offset not a 2-digit year; but in some
     locales the E modifier may be ignored.
  */
  strftime (s, m, "%Ey", tp); /* { dg-warning "some locales" "2-digit year" } */
  /* Bad uses of %E and %O.  */
  strftime (s, m, "%EEY", tp); /* { dg-warning "multiple|repeated" "multiple %E/%O" } */
  strftime (s, m, "%EOy", tp); /* { dg-warning "multiple|together" "multiple %E/%O" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%OEy", tp); /* { dg-warning "multiple|together" "multiple %E/%O" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%OOV", tp); /* { dg-warning "multiple|repeated" "multiple %E/%O" } */
  strftime (s, m, "%Ea", tp); /* { dg-warning "flag|modifier" "bad %Ea" } */
  strftime (s, m, "%EA", tp); /* { dg-warning "flag|modifier" "bad %EA" } */
  strftime (s, m, "%Eb", tp); /* { dg-warning "flag|modifier" "bad %Eb" } */
  strftime (s, m, "%EB", tp); /* { dg-warning "flag|modifier" "bad %EB" } */
  strftime (s, m, "%Ed", tp); /* { dg-warning "flag|modifier" "bad %Ed" } */
  strftime (s, m, "%ED", tp); /* { dg-warning "flag|modifier" "bad %ED" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%Ee", tp); /* { dg-warning "flag|modifier" "bad %Ee" } */
  strftime (s, m, "%EF", tp); /* { dg-warning "flag|modifier" "bad %EF" } */
  strftime (s, m, "%Eg", tp); /* { dg-warning "flag|modifier" "bad %Eg" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%EG", tp); /* { dg-warning "flag|modifier" "bad %EG" } */
  strftime (s, m, "%Eh", tp); /* { dg-warning "flag|modifier" "bad %Eh" } */
  strftime (s, m, "%EH", tp); /* { dg-warning "flag|modifier" "bad %EH" } */
  strftime (s, m, "%EI", tp); /* { dg-warning "flag|modifier" "bad %EI" } */
  strftime (s, m, "%Ej", tp); /* { dg-warning "flag|modifier" "bad %Ej" } */
  strftime (s, m, "%Em", tp); /* { dg-warning "flag|modifier" "bad %Em" } */
  strftime (s, m, "%EM", tp); /* { dg-warning "flag|modifier" "bad %EM" } */
  strftime (s, m, "%En", tp); /* { dg-warning "flag|modifier" "bad %En" } */
  strftime (s, m, "%Ep", tp); /* { dg-warning "flag|modifier" "bad %Ep" } */
  strftime (s, m, "%Er", tp); /* { dg-warning "flag|modifier" "bad %Er" } */
  strftime (s, m, "%ER", tp); /* { dg-warning "flag|modifier" "bad %ER" } */
  strftime (s, m, "%ES", tp); /* { dg-warning "flag|modifier" "bad %ES" } */
  strftime (s, m, "%Et", tp); /* { dg-warning "flag|modifier" "bad %Et" } */
  strftime (s, m, "%ET", tp); /* { dg-warning "flag|modifier" "bad %ET" } */
  strftime (s, m, "%Eu", tp); /* { dg-warning "flag|modifier" "bad %Eu" } */
  strftime (s, m, "%EU", tp); /* { dg-warning "flag|modifier" "bad %EU" } */
  strftime (s, m, "%EV", tp); /* { dg-warning "flag|modifier" "bad %EV" } */
  strftime (s, m, "%Ew", tp); /* { dg-warning "flag|modifier" "bad %Ew" } */
  strftime (s, m, "%EW", tp); /* { dg-warning "flag|modifier" "bad %EW" } */
  strftime (s, m, "%Ez", tp); /* { dg-warning "flag|modifier" "bad %Ez" } */
  strftime (s, m, "%EZ", tp); /* { dg-warning "flag|modifier" "bad %EZ" } */
  strftime (s, m, "%E%", tp); /* { dg-warning "flag|modifier" "bad %E%" } */
  strftime (s, m, "%Oa", tp); /* { dg-warning "flag|modifier" "bad %Oa" } */
  strftime (s, m, "%OA", tp); /* { dg-warning "flag|modifier" "bad %OA" } */
  strftime (s, m, "%Ob", tp); /* { dg-warning "flag|modifier" "bad %Ob" } */
  strftime (s, m, "%OB", tp); /* { dg-warning "flag|modifier" "bad %OB" } */
  strftime (s, m, "%Oc", tp); /* { dg-warning "flag|modifier" "bad %Oc" } */
  /* { dg-warning "in some locales" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%OC", tp); /* { dg-warning "flag|modifier|C" "bad %OC" } */
  strftime (s, m, "%OD", tp); /* { dg-warning "flag|modifier" "bad %OD" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%OF", tp); /* { dg-warning "flag|modifier" "bad %OF" } */
  strftime (s, m, "%Og", tp); /* { dg-warning "flag|modifier|C" "bad %Og" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%OG", tp); /* { dg-warning "flag|modifier|C" "bad %OG" } */
  strftime (s, m, "%Oh", tp); /* { dg-warning "flag|modifier" "bad %Oh" } */
  strftime (s, m, "%Oj", tp); /* { dg-warning "flag|modifier|C" "bad %Oj" } */
  strftime (s, m, "%On", tp); /* { dg-warning "flag|modifier" "bad %On" } */
  strftime (s, m, "%Op", tp); /* { dg-warning "flag|modifier" "bad %Op" } */
  strftime (s, m, "%Or", tp); /* { dg-warning "flag|modifier" "bad %Or" } */
  strftime (s, m, "%OR", tp); /* { dg-warning "flag|modifier" "bad %OR" } */
  strftime (s, m, "%Ot", tp); /* { dg-warning "flag|modifier" "bad %Ot" } */
  strftime (s, m, "%OT", tp); /* { dg-warning "flag|modifier" "bad %OT" } */
  strftime (s, m, "%Ox", tp); /* { dg-warning "flag|modifier" "bad %Ox" } */
  /* { dg-warning "in some locales" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%OX", tp); /* { dg-warning "flag|modifier" "bad %OX" } */
  strftime (s, m, "%OY", tp); /* { dg-warning "flag|modifier|C" "bad %OY" } */
  strftime (s, m, "%Oz", tp); /* { dg-warning "flag|modifier|C" "bad %Oz" } */
  strftime (s, m, "%OZ", tp); /* { dg-warning "flag|modifier" "bad %OZ" } */
  strftime (s, m, "%O%", tp); /* { dg-warning "flag|modifier" "bad %O%" } */
}
