/* Test for X/Open format extensions, as found in the
   Single Unix Specification and in Austin Group draft 7.
*/
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do compile } */
/* { dg-options "-std=gnu99 -Wformat" } */

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
foo (int i, unsigned int u, wint_t lc, wchar_t *ls, int *ip, double d,
     char *s, void *p, int *n, long int l, int i2, float *fp, long int *lp,
     va_list va)
{
  /* The conversion specifiers C and S, for both printf and scanf,
     are X/Open extensions.
  */
  printf ("%C", lc);
  printf ("%3C", lc);
  printf ("%.3C", lc); /* { dg-warning "precision" "precision with %C" } */
  printf ("%hC", lc); /* { dg-warning "length" "bad %hC" } */
  printf ("%hhC", lc); /* { dg-warning "length" "bad %hhC" } */
  printf ("%lC", lc); /* { dg-warning "length" "bad %lC" } */
  printf ("%llC", lc); /* { dg-warning "length" "bad %llC" } */
  printf ("%jC", lc); /* { dg-warning "length" "bad %jC" } */
  printf ("%zC", lc); /* { dg-warning "length" "bad %zC" } */
  printf ("%tC", lc); /* { dg-warning "length" "bad %tC" } */
  printf ("%LC", lc); /* { dg-warning "length" "bad %LC" } */
  printf ("%-C", lc);
  printf ("%+C", lc); /* { dg-warning "flag" "bad %+C" } */
  printf ("% C", lc); /* { dg-warning "flag" "bad % C" } */
  printf ("%#C", lc); /* { dg-warning "flag" "bad %#C" } */
  printf ("%0C", lc); /* { dg-warning "flag" "bad %0C" } */
  printf ("%'C", lc); /* { dg-warning "flag" "bad %'C" } */
  printf ("%S", ls);
  printf ("%3S", ls);
  printf ("%.3S", ls);
  printf ("%hS", ls); /* { dg-warning "length" "bad %hS" } */
  printf ("%hhS", ls); /* { dg-warning "length" "bad %hhS" } */
  printf ("%lS", ls); /* { dg-warning "length" "bad %lS" } */
  printf ("%llS", ls); /* { dg-warning "length" "bad %llS" } */
  printf ("%jS", ls); /* { dg-warning "length" "bad %jS" } */
  printf ("%zS", ls); /* { dg-warning "length" "bad %zS" } */
  printf ("%tS", ls); /* { dg-warning "length" "bad %tS" } */
  printf ("%LS", ls); /* { dg-warning "length" "bad %LS" } */
  printf ("%-S", ls);
  printf ("%+S", ls); /* { dg-warning "flag" "bad %+S" } */
  printf ("% S", ls); /* { dg-warning "flag" "bad % S" } */
  printf ("%#S", ls); /* { dg-warning "flag" "bad %#S" } */
  printf ("%0S", ls); /* { dg-warning "flag" "bad %0S" } */
  printf ("%'S", ls); /* { dg-warning "flag" "bad %'S" } */
  scanf ("%C", ls);
  scanf ("%S", ls);
  scanf ("%*C%*S");
  scanf ("%2C%3S", ls, ls);
  scanf ("%hC", ls); /* { dg-warning "length" "bad %hC" } */
  scanf ("%hhC", ls); /* { dg-warning "length" "bad %hhC" } */
  scanf ("%lC", ls); /* { dg-warning "length" "bad %lC" } */
  scanf ("%llC", ls); /* { dg-warning "length" "bad %llC" } */
  scanf ("%jC", ls); /* { dg-warning "length" "bad %jC" } */
  scanf ("%zC", ls); /* { dg-warning "length" "bad %zC" } */
  scanf ("%tC", ls); /* { dg-warning "length" "bad %tC" } */
  scanf ("%LC", ls); /* { dg-warning "length" "bad %LC" } */
  scanf ("%hS", ls); /* { dg-warning "length" "bad %hS" } */
  scanf ("%hhS", ls); /* { dg-warning "length" "bad %hhS" } */
  scanf ("%lS", ls); /* { dg-warning "length" "bad %lS" } */
  scanf ("%llS", ls); /* { dg-warning "length" "bad %llS" } */
  scanf ("%jS", ls); /* { dg-warning "length" "bad %jS" } */
  scanf ("%zS", ls); /* { dg-warning "length" "bad %zS" } */
  scanf ("%tS", ls); /* { dg-warning "length" "bad %tS" } */
  scanf ("%LS", ls); /* { dg-warning "length" "bad %LS" } */
  /* In C99 mode (even with extensions), %aS is a floating point
     format followed by an S.
  */
  scanf ("%aS", fp);
  /* The printf flag character ' is an X/Open extension.  */
  printf ("%'d%'i%'u%'f%'F%'g%'G", i, i, u, d, d, d, d);
  printf ("%'o", u); /* { dg-warning "flag" "bad use of ' flag" } */
  printf ("%'x", u); /* { dg-warning "flag" "bad use of ' flag" } */
  printf ("%'X", u); /* { dg-warning "flag" "bad use of ' flag" } */
  printf ("%'e", d); /* { dg-warning "flag" "bad use of ' flag" } */
  printf ("%'E", d); /* { dg-warning "flag" "bad use of ' flag" } */
  printf ("%'a", d); /* { dg-warning "flag" "bad use of ' flag" } */
  printf ("%'A", d); /* { dg-warning "flag" "bad use of ' flag" } */
  printf ("%'c", i); /* { dg-warning "flag" "bad use of ' flag" } */
  printf ("%'s", s); /* { dg-warning "flag" "bad use of ' flag" } */
  printf ("%'p", p); /* { dg-warning "flag" "bad use of ' flag" } */
  printf ("%'n", n); /* { dg-warning "flag" "bad use of ' flag" } */
  /* The use of operand number $ formats is an X/Open extension.  */
  scanf ("%1$d", ip);
  printf ("%1$d", i);
  printf ("%1$d", l); /* { dg-warning "arg 2|argument 2" "mismatched args with $ format" } */
  printf ("%3$*2$.*1$ld", i2, i, l);
  printf ("%4$ld%7$ld%5$d%6$d%3$d%1$d%2$d", i, i, i, l, i, i, l);
  scanf ("%4$ld%7$ld%5$d%6$d%3$d%1$d%2$d", ip, ip, ip, lp, ip, ip, lp);
  printf ("%1$d%d", i, i); /* { dg-warning "missing" "mixing $ and non-$ formats" } */
  printf ("%%%1$d%%%2$d", i, i);
  printf ("%d%2$d", i); /* { dg-warning "used after format" "mixing $ and non-$ formats" } */
  printf ("%1$*d", i, i); /* { dg-warning "missing" "mixing $ and non-$ formats" } */
  printf ("%*1$d", i); /* { dg-warning "missing" "mixing $ and non-$ formats" } */
  scanf ("%1$d%d", ip, ip); /* { dg-warning "missing" "mixing $ and non-$ formats" } */
  scanf ("%*f%%%1$d%%%2$d", ip, ip);
  printf ("%2$d", i); /* { dg-warning "operand" "$ number too large" } */
  printf ("%0$d", i); /* { dg-warning "operand" "$ number too small" } */
  printf ("%3$d%1$d", i, i, i); /* { dg-warning "before used" "unused $ operand" } */
  printf ("%2$d%1$d", i, i, i); /* { dg-warning "unused" "unused $ operand" } */
  vprintf ("%3$d%1$d", va); /* { dg-warning "before used" "unused $ operand" } */
  /* With scanf formats, gaps in the used arguments are allowed only if the
     arguments are all pointers.  In such a case, should only give the lesser
     warning about unused arguments rather than the more serious one about
     argument gaps.  */
  scanf ("%3$d%1$d", ip, ip, ip); /* { dg-bogus "before used" "unused $ scanf pointer operand" } */
  /* { dg-warning "unused" "unused $ scanf pointer operand" { target *-*-* } .-1 } */
  /* If there are non-pointer arguments unused at the end, this is also OK.  */
  scanf ("%3$d%1$d", ip, ip, ip, i); /* { dg-bogus "before used" "unused $ scanf pointer operand" } */
  /* { dg-warning "unused" "unused $ scanf pointer operand" { target *-*-* } .-1 } */
  scanf ("%3$d%1$d", ip, i, ip); /* { dg-warning "before used" "unused $ scanf non-pointer operand" } */
  /* Can't check the arguments in the vscanf case, so should suppose the
     lesser problem.  */
  vscanf ("%3$d%1$d", va); /* { dg-bogus "before used" "unused $ scanf pointer operand" } */
  /* { dg-warning "unused" "unused $ scanf pointer operand" { target *-*-* } .-1 } */
  scanf ("%2$*d%1$d", ip, ip); /* { dg-warning "operand" "operand number with suppression" } */
  printf ("%1$d%1$d", i);
  scanf ("%1$d%1$d", ip); /* { dg-warning "more than once" "multiple use of scanf argument" } */
}
