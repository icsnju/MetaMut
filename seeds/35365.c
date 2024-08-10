/* Test for scanf formats.  Formats using C90 features, including cases
   where C90 specifies some aspect of the format to be ignored or where
   the behavior is undefined.
*/
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do compile { target { *-*-mingw* } } } */
/* { dg-options "-std=iso9899:1990 -pedantic -Wformat" } */

#define USE_SYSTEM_FORMATS
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
foo (int *ip, unsigned int *uip, short int *hp, unsigned short int *uhp,
     long int *lp, unsigned long int *ulp, float *fp, double *dp,
     long double *ldp, char *s, signed char *ss, unsigned char *us,
     void **pp, int *n, llong *llp, ullong *ullp, wchar_t *ls,
     const int *cip, const int *cn, const char *cs, const void **ppc,
     void *const *pcp, short int *hn, long int *ln, void *p, char **sp,
     volatile void *ppv)
{
  /* See ISO/IEC 9899:1990 (E) subclause 7.9.6.2 (pages 134-138).  */
  /* Basic sanity checks for the different components of a format.  */
  scanf ("%d", ip);
  scanf ("%*d");
  scanf ("%3d", ip);
  scanf ("%hd", hp);
  scanf ("%3ld", lp);
  scanf ("%*3d");
  scanf ("%d %ld", ip, lp);
  /* Valid and invalid %% constructions.  */
  scanf ("%%");
  scanf ("%*%"); /* { dg-warning "format" "bogus %%" } */
  scanf ("%*%\n"); /* { dg-warning "format" "bogus %%" } */
  scanf ("%4%"); /* { dg-warning "format" "bogus %%" } */
  scanf ("%4%\n"); /* { dg-warning "format" "bogus %%" } */
  scanf ("%h%"); /* { dg-warning "format" "bogus %%" } */
  scanf ("%h%\n"); /* { dg-warning "format" "bogus %%" } */
  /* Valid, invalid and silly assignment-suppression constructions.  */
  scanf ("%*d%*i%*o%*u%*x%*X%*e%*E%*f%*g%*G%*s%*[abc]%*c%*p");
  scanf ("%*2d%*8s%*3c");
  scanf ("%*n", n); /* { dg-warning "suppress" "suppression of %n" } */
  scanf ("%*hd"); /* { dg-warning "together" "suppression with length" } */
  /* Valid, invalid and silly width constructions.  */
  scanf ("%2d%3i%4o%5u%6x%7X%8e%9E%10f%11g%12G%13s%14[abc]%15c%16p",
	 ip, ip, uip, uip, uip, uip, fp, fp, fp, fp, fp, s, s, s, pp);
  scanf ("%0d", ip); /* { dg-warning "width" "warning for zero width" } */
  scanf ("%3n", n); /* { dg-warning "width" "width with %n" } */
  /* Valid and invalid %h, %l, %L constructions.  */
  scanf ("%hd%hi%ho%hu%hx%hX%hn", hp, hp, uhp, uhp, uhp, uhp, hn);
  scanf ("%he", fp); /* { dg-warning "length" "bad use of %h" } */
  scanf ("%hE", fp); /* { dg-warning "length" "bad use of %h" } */
  scanf ("%hf", fp); /* { dg-warning "length" "bad use of %h" } */
  scanf ("%hg", fp); /* { dg-warning "length" "bad use of %h" } */
  scanf ("%hG", fp); /* { dg-warning "length" "bad use of %h" } */
  scanf ("%hs", hp);
  scanf ("%h[ac]", s); /* { dg-warning "length" "bad use of %h" } */
  scanf ("%hc", hp);
  scanf ("%hp", pp); /* { dg-warning "length" "bad use of %h" } */
  scanf ("%h"); /* { dg-warning "conversion lacks type" "bare %h" } */
  scanf ("%h."); /* { dg-warning "conversion" "bogus %h" } */
  scanf ("%ld%li%lo%lu%lx%lX%ln", lp, lp, ulp, ulp, ulp, ulp, ln);
  scanf ("%le%lE%lf%lg%lG", dp, dp, dp, dp, dp);
  scanf ("%lp", pp); /* { dg-warning "length" "bad use of %l" } */
  /* These next three formats were added in C94.  */
  scanf ("%ls", ls); /* { dg-warning "length|C" "bad use of %l" } */
  scanf ("%l[ac]", ls); /* { dg-warning "length|C" "bad use of %l" } */
  scanf ("%lc", ls); /* { dg-warning "length|C" "bad use of %l" } */
  scanf ("%Ld", llp); /* { dg-warning "unknown|format" "%L is unsupported" } */
  scanf ("%Li", llp); /* { dg-warning "unknown|format" "%L is unsupported" } */
  scanf ("%Lo", ullp); /* { dg-warning "unknown|format" "%L is unsupported" } */
  scanf ("%Lu", ullp); /* { dg-warning "unknown|format" "%L is unsupported" } */
  scanf ("%Lx", ullp); /* { dg-warning "unknown|format" "%L is unsupported" } */
  scanf ("%LX", ullp); /* { dg-warning "unknown|format" "%L is unsupported" } */
  scanf ("%Ls", s); /* { dg-warning "unknown|format" "%L is unsupported" } */
  scanf ("%L[ac]", s); /* { dg-warning "unknown|format" "%L is unsupported" } */
  scanf ("%Lc", s); /* { dg-warning "unknown|format" "%L is unsupported" } */
  scanf ("%Lp", pp); /* { dg-warning "unknown|format" "%L is unsupported" } */
  scanf ("%Ln", n); /* { dg-warning "unknown|format" "%L is unsupported" } */
  /* Valid uses of each bare conversion.  */
  scanf ("%d%i%o%u%x%X%e%E%f%g%G%s%[abc]%c%p%n%%", ip, ip, uip, uip, uip,
	 uip, fp, fp, fp, fp, fp, s, s, s, pp, n);
  /* Allow other character pointers with %s, %c, %[].  */
  scanf ("%2s%3s%4c%5c%6[abc]%7[abc]", ss, us, ss, us, ss, us);
  /* Further tests for %[].  */
  scanf ("%[%d]%d", s, ip);
  scanf ("%[^%d]%d", s, ip);
  scanf ("%[]%d]%d", s, ip);
  scanf ("%[^]%d]%d", s, ip);
  scanf ("%[%d]%d", s, ip);
  scanf ("%[]abcd", s); /* { dg-warning "no closing" "incomplete scanset" } */
  /* Various tests of bad argument types.  Some of these are only pedantic
     warnings.
  */
  scanf ("%d", lp); /* { dg-warning "format" "bad argument types" } */
  scanf ("%d", uip); /* { dg-warning "format" "bad argument types" } */
  scanf ("%d", pp); /* { dg-warning "format" "bad argument types" } */
  scanf ("%p", ppc); /* { dg-warning "format" "bad argument types" } */
  scanf ("%p", ppv); /* { dg-warning "format" "bad argument types" } */
  scanf ("%s", n); /* { dg-warning "format" "bad argument types" } */
  scanf ("%s", p); /* { dg-warning "format" "bad argument types" } */
  scanf ("%p", p); /* { dg-warning "format" "bad argument types" } */
  scanf ("%p", sp); /* { dg-warning "format" "bad argument types" } */
  /* Tests for writing into constant values.  */
  scanf ("%d", cip); /* { dg-warning "constant" "%d writing into const" } */
  scanf ("%n", cn); /* { dg-warning "constant" "%n writing into const" } */
  scanf ("%s", cs); /* { dg-warning "constant" "%s writing into const" } */
  scanf ("%p", pcp); /* { dg-warning "constant" "%p writing into const" } */
  /* Wrong number of arguments.  */
  scanf ("%d%d", ip); /* { dg-warning "matching" "wrong number of args" } */
  scanf ("%d", ip, ip); /* { dg-warning "arguments" "wrong number of args" } */
  /* Miscellaneous bogus constructions.  */
  scanf (""); /* { dg-warning "zero-length" "warning for empty format" } */
  scanf ("\0"); /* { dg-warning "embedded" "warning for embedded NUL" } */
  scanf ("%d\0", ip); /* { dg-warning "embedded" "warning for embedded NUL" } */
  scanf ("%d\0%d", ip, ip); /* { dg-warning "embedded|too many" "warning for embedded NUL" } */
  scanf (NULL); /* { dg-warning "null" "null format string warning" } */
  scanf ("%"); /* { dg-warning "trailing" "trailing % warning" } */
  scanf ("%d", (int *)0); /* { dg-warning "null" "writing into NULL" } */
}
