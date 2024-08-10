/* Test for format extensions beyond the C standard and X/Open standard.
   Test for printf formats.
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
foo (quad_t q, u_quad_t uq, quad_t *qn, size_t z, size_t *zn, long long int ll,
     unsigned long long int ull, int i, unsigned int u, double d,
     char *s, void *p, wchar_t *ls, wint_t lc, int *n, long int l)
{
  /* As an extension, GCC allows the BSD length "q" for integer formats.
     This is largely obsoleted in C99 by %j, %ll and PRId64.
  */
  printf ("%qd%qi%qo%qu%qx%qX%qn", q, q, uq, uq, uq, uq, qn);
  printf ("%qf", d); /* { dg-warning "length" "bad use of %q" } */
  printf ("%qF", d); /* { dg-warning "length" "bad use of %q" } */
  printf ("%qe", d); /* { dg-warning "length" "bad use of %q" } */
  printf ("%qE", d); /* { dg-warning "length" "bad use of %q" } */
  printf ("%qg", d); /* { dg-warning "length" "bad use of %q" } */
  printf ("%qG", d); /* { dg-warning "length" "bad use of %q" } */
  printf ("%qa", d); /* { dg-warning "length" "bad use of %q" } */
  printf ("%qA", d); /* { dg-warning "length" "bad use of %q" } */
  printf ("%qc", i); /* { dg-warning "length" "bad use of %q" } */
  printf ("%qs", s); /* { dg-warning "length" "bad use of %q" } */
  printf ("%qp", p); /* { dg-warning "length" "bad use of %q" } */
  printf ("%qC", lc); /* { dg-warning "length" "bad use of %q" } */
  printf ("%qS", ls); /* { dg-warning "length" "bad use of %q" } */
  /* With a bad length GCC wants some argument, any argument,
     to devour with the format conversion, as a synchronisation heuristic.
     This may get improved later.
  */
  printf ("%qm", i); /* { dg-warning "length" "bad use of %q" } */
  /* As an extension, GCC allows the length "Z" as a synonym for "z".
     This was an extension predating C99 which should now be considered
     deprecated; use the standard "z" instead.
  */
  printf ("%Zd%Zi%Zo%Zu%Zx%ZX", z, z, z, z, z, z);
  printf ("%Zn", zn);
  printf ("%Zf", d); /* { dg-warning "length" "bad use of %Z" } */
  printf ("%ZF", d); /* { dg-warning "length" "bad use of %Z" } */
  printf ("%Ze", d); /* { dg-warning "length" "bad use of %Z" } */
  printf ("%ZE", d); /* { dg-warning "length" "bad use of %Z" } */
  printf ("%Zg", d); /* { dg-warning "length" "bad use of %Z" } */
  printf ("%ZG", d); /* { dg-warning "length" "bad use of %Z" } */
  printf ("%Za", d); /* { dg-warning "length" "bad use of %Z" } */
  printf ("%ZA", d); /* { dg-warning "length" "bad use of %Z" } */
  printf ("%Zc", i); /* { dg-warning "length" "bad use of %Z" } */
  printf ("%Zs", s); /* { dg-warning "length" "bad use of %Z" } */
  printf ("%Zp", p); /* { dg-warning "length" "bad use of %Z" } */
  printf ("%ZC", lc); /* { dg-warning "length" "bad use of %Z" } */
  printf ("%ZS", ls); /* { dg-warning "length" "bad use of %Z" } */
  printf ("%Zm", i); /* { dg-warning "length" "bad use of %Z" } */
  /* As an extension, GCC allows the length "L" on integer formats
     (but not %n) as a synonym for "ll".
     This should be considered deprecated.
  */
  printf ("%Ld%Li%Lo%Lu%Lx%LX", ll, ll, ull, ull, ull, ull);
  /* As an extension, derived from syslog, GCC allows the conversion
     specifier "m" for formatting strerror(errno).  This may be used
     with width, precision and the "-" flag, the same as %s.
  */
  printf ("%m%3m%.4m%5.6m");
  printf ("%*m", i);
  printf ("%.*m", i);
  printf ("%*.*m", i, i);
  printf ("%3.*m", i);
  printf ("%*.4m", i);
  printf ("%-m");
  printf ("%+m"); /* { dg-warning "flag" "bad %+m" } */
  printf ("% m"); /* { dg-warning "flag" "bad % m" } */
  printf ("%#m"); /* { dg-warning "flag" "bad %#m" } */
  printf ("%0m"); /* { dg-warning "flag" "bad %0m" } */
  printf ("%'m"); /* { dg-warning "flag" "bad %'m" } */
  printf ("%hm", i); /* { dg-warning "length" "bad %hm" } */
  printf ("%hhm", i); /* { dg-warning "length" "bad %hhm" } */
  printf ("%lm", i); /* { dg-warning "length" "bad %lm" } */
  printf ("%llm", i); /* { dg-warning "length" "bad %llm" } */
  printf ("%jm", i); /* { dg-warning "length" "bad %jm" } */
  printf ("%zm", i); /* { dg-warning "length" "bad %zm" } */
  printf ("%tm", i); /* { dg-warning "length" "bad %tm" } */
  printf ("%Lm", i); /* { dg-warning "length" "bad %Lm" } */
  printf ("%qm", i); /* { dg-warning "length" "bad %qm" } */
  printf ("%Zm", i); /* { dg-warning "length" "bad %Zm" } */
  /* It should be OK to mix %m formats with $ operand number formats.  */
  printf ("%2$ld%m%1$d", i, l);
  /* Likewise, %m formats with width and precision should not have an
     operand number for the %m itself.
  */
  printf ("%*2$.*1$m", i, i);
  printf ("%1$*2$.*1$m", i, i); /* { dg-warning "no argument" "printf %1\$m" } */
  /* As an extension, glibc includes the "I" flag for decimal
     formats, to output using the locale's digits (e.g. in Arabic).
     In GCC, we require this to be in the standard place for flags, though
     glibc allows it also after width or precision.
  */
  printf ("%Id%Ii%Iu", i, i, u);
  printf ("%Io", u); /* { dg-warning "flag" "bad use of I flag" } */
  printf ("%Ix", u); /* { dg-warning "flag" "bad use of I flag" } */
  printf ("%IX", u); /* { dg-warning "flag" "bad use of I flag" } */
  printf ("%In", n); /* { dg-warning "flag" "bad use of I flag" } */
  printf ("%If", d);
  printf ("%IF", d);
  printf ("%Ie", d);
  printf ("%IE", d);
  printf ("%Ig", d);
  printf ("%IG", d);
  printf ("%Ia", d); /* { dg-warning "flag" "bad use of I flag" } */
  printf ("%IA", d); /* { dg-warning "flag" "bad use of I flag" } */
  printf ("%Ic", i); /* { dg-warning "flag" "bad use of I flag" } */
  printf ("%Is", s); /* { dg-warning "flag" "bad use of I flag" } */
  printf ("%Ip", p); /* { dg-warning "flag" "bad use of I flag" } */
  printf ("%IC", lc); /* { dg-warning "flag" "bad use of I flag" } */
  printf ("%IS", ls); /* { dg-warning "flag" "bad use of I flag" } */
  printf ("%Im"); /* { dg-warning "flag" "bad use of I flag" } */

  /* As an extension, GCC does format checking on "unlocked"
     i.e. thread unsafe versions of these functions.  */
  fprintf_unlocked (stdout, "%d", i);
  fprintf_unlocked (stdout, "%ld", i); /* { dg-warning "format" "fprintf_unlocked" } */
  printf_unlocked ("%d", i);
  printf_unlocked ("%ld", i); /* { dg-warning "format" "printf_unlocked" } */
}
