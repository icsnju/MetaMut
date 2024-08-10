/* Test for scanf formats.  Formats using C99 features, including cases
   where C99 specifies some aspect of the format to be ignored or where
   the behavior is undefined.
*/
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1999 -pedantic -Wformat" } */

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
     signed char *hhp, unsigned char *uhhp, long int *lp,
     unsigned long int *ulp, float *fp, double *dp, long double *ldp, char *s,
     void **pp, int *n, long long *llp, unsigned long long *ullp, wchar_t *ls,
     short int *hn, signed char *hhn, long int *ln, long long int *lln,
     intmax_t *jp, uintmax_t *ujp, intmax_t *jn, size_t *zp,
     signed_size_t *szp, signed_size_t *zn, ptrdiff_t *tp,
     unsigned_ptrdiff_t *utp, ptrdiff_t *tn)
{
  /* See ISO/IEC 9899:1999 (E) subclause 7.19.6.2 (pages 281-288).
     We do not repeat here most of the checks for correct C90 formats
     or completely broken formats.
  */
  /* Valid, invalid and silly assignment-suppression
     and width constructions.
  */
  scanf ("%*d%*i%*o%*u%*x%*X%*a%*A%*e%*E%*f%*F%*g%*G%*s%*[abc]%*c%*p");
  scanf ("%*2d%*8s%*3c");
  scanf ("%*n", n); /* { dg-warning "suppress" "suppression of %n" } */
  scanf ("%*hd"); /* { dg-warning "together" "suppression with length" } */
  scanf ("%2d%3i%4o%5u%6x%7X%8a%9A%10e%11E%12f%13F%14g%15G%16s%3[abc]%4c%5p",
	 ip, ip, uip, uip, uip, uip, fp, fp, fp, fp, fp, fp, fp, fp,
	 s, s, s, pp);
  scanf ("%0d", ip); /* { dg-warning "width" "warning for zero width" } */
  scanf ("%3n", n); /* { dg-warning "width" "width with %n" } */
  /* Valid and invalid %h, %hh, %l, %ll, %j, %z, %t, %L constructions.  */
  scanf ("%hd%hi%ho%hu%hx%hX%hn", hp, hp, uhp, uhp, uhp, uhp, hn);
  scanf ("%ha", fp); /* { dg-warning "length" "bad use of %h" } */
  scanf ("%hA", fp); /* { dg-warning "length" "bad use of %h" } */
  scanf ("%he", fp); /* { dg-warning "length" "bad use of %h" } */
  scanf ("%hE", fp); /* { dg-warning "length" "bad use of %h" } */
  scanf ("%hf", fp); /* { dg-warning "length" "bad use of %h" } */
  scanf ("%hF", fp); /* { dg-warning "length" "bad use of %h" } */
  scanf ("%hg", fp); /* { dg-warning "length" "bad use of %h" } */
  scanf ("%hG", fp); /* { dg-warning "length" "bad use of %h" } */
  scanf ("%hs", s); /* { dg-warning "length" "bad use of %h" } */
  scanf ("%h[ac]", s); /* { dg-warning "length" "bad use of %h" } */
  scanf ("%hc", s); /* { dg-warning "length" "bad use of %h" } */
  scanf ("%hp", pp); /* { dg-warning "length" "bad use of %h" } */
  scanf ("%hhd%hhi%hho%hhu%hhx%hhX%hhn", hhp, hhp, uhhp, uhhp, uhhp, uhhp,
	 hhn);
  scanf ("%hha", fp); /* { dg-warning "length" "bad use of %hh" } */
  scanf ("%hhA", fp); /* { dg-warning "length" "bad use of %hh" } */
  scanf ("%hhe", fp); /* { dg-warning "length" "bad use of %hh" } */
  scanf ("%hhE", fp); /* { dg-warning "length" "bad use of %hh" } */
  scanf ("%hhf", fp); /* { dg-warning "length" "bad use of %hh" } */
  scanf ("%hhF", fp); /* { dg-warning "length" "bad use of %hh" } */
  scanf ("%hhg", fp); /* { dg-warning "length" "bad use of %hh" } */
  scanf ("%hhG", fp); /* { dg-warning "length" "bad use of %hh" } */
  scanf ("%hhs", s); /* { dg-warning "length" "bad use of %hh" } */
  scanf ("%hh[ac]", s); /* { dg-warning "length" "bad use of %hh" } */
  scanf ("%hhc", s); /* { dg-warning "length" "bad use of %hh" } */
  scanf ("%hhp", pp); /* { dg-warning "length" "bad use of %hh" } */
  scanf ("%ld%li%lo%lu%lx%lX%ln", lp, lp, ulp, ulp, ulp, ulp, ln);
  scanf ("%la%lA%le%lE%lf%lF%lg%lG", dp, dp, dp, dp, dp, dp, dp, dp);
  scanf ("%lp", pp); /* { dg-warning "length" "bad use of %l" } */
  scanf ("%ls", ls);
  scanf ("%l[ac]", ls);
  scanf ("%lc", ls);
  scanf ("%lld%lli%llo%llu%llx%llX%lln", llp, llp, ullp, ullp, ullp, ullp,
	 lln);
  scanf ("%lla", fp); /* { dg-warning "length" "bad use of %ll" } */
  scanf ("%llA", fp); /* { dg-warning "length" "bad use of %ll" } */
  scanf ("%lle", fp); /* { dg-warning "length" "bad use of %ll" } */
  scanf ("%llE", fp); /* { dg-warning "length" "bad use of %ll" } */
  scanf ("%llf", fp); /* { dg-warning "length" "bad use of %ll" } */
  scanf ("%llF", fp); /* { dg-warning "length" "bad use of %ll" } */
  scanf ("%llg", fp); /* { dg-warning "length" "bad use of %ll" } */
  scanf ("%llG", fp); /* { dg-warning "length" "bad use of %ll" } */
  scanf ("%lls", s); /* { dg-warning "length" "bad use of %ll" } */
  scanf ("%ll[ac]", s); /* { dg-warning "length" "bad use of %ll" } */
  scanf ("%llc", s); /* { dg-warning "length" "bad use of %ll" } */
  scanf ("%llp", pp); /* { dg-warning "length" "bad use of %ll" } */
  scanf ("%jd%ji%jo%ju%jx%jX%jn", jp, jp, ujp, ujp, ujp, ujp, jn); /* { dg-bogus "length" "bogus %j warning" } */
  scanf ("%ja", fp); /* { dg-warning "length" "bad use of %j" } */
  scanf ("%jA", fp); /* { dg-warning "length" "bad use of %j" } */
  scanf ("%je", fp); /* { dg-warning "length" "bad use of %j" } */
  scanf ("%jE", fp); /* { dg-warning "length" "bad use of %j" } */
  scanf ("%jf", fp); /* { dg-warning "length" "bad use of %j" } */
  scanf ("%jF", fp); /* { dg-warning "length" "bad use of %j" } */
  scanf ("%jg", fp); /* { dg-warning "length" "bad use of %j" } */
  scanf ("%jG", fp); /* { dg-warning "length" "bad use of %j" } */
  scanf ("%js", s); /* { dg-warning "length" "bad use of %j" } */
  scanf ("%j[ac]", s); /* { dg-warning "length" "bad use of %j" } */
  scanf ("%jc", s); /* { dg-warning "length" "bad use of %j" } */
  scanf ("%jp", pp); /* { dg-warning "length" "bad use of %j" } */
  scanf ("%zd%zi%zo%zu%zx%zX%zn", szp, szp, zp, zp, zp, zp, zn);
  scanf ("%za", fp); /* { dg-warning "length" "bad use of %z" } */
  scanf ("%zA", fp); /* { dg-warning "length" "bad use of %z" } */
  scanf ("%ze", fp); /* { dg-warning "length" "bad use of %z" } */
  scanf ("%zE", fp); /* { dg-warning "length" "bad use of %z" } */
  scanf ("%zf", fp); /* { dg-warning "length" "bad use of %z" } */
  scanf ("%zF", fp); /* { dg-warning "length" "bad use of %z" } */
  scanf ("%zg", fp); /* { dg-warning "length" "bad use of %z" } */
  scanf ("%zG", fp); /* { dg-warning "length" "bad use of %z" } */
  scanf ("%zs", s); /* { dg-warning "length" "bad use of %z" } */
  scanf ("%z[ac]", s); /* { dg-warning "length" "bad use of %z" } */
  scanf ("%zc", s); /* { dg-warning "length" "bad use of %z" } */
  scanf ("%zp", pp); /* { dg-warning "length" "bad use of %z" } */
  scanf ("%td%ti%to%tu%tx%tX%tn", tp, tp, utp, utp, utp, utp, tn);
  scanf ("%ta", fp); /* { dg-warning "length" "bad use of %t" } */
  scanf ("%tA", fp); /* { dg-warning "length" "bad use of %t" } */
  scanf ("%te", fp); /* { dg-warning "length" "bad use of %t" } */
  scanf ("%tE", fp); /* { dg-warning "length" "bad use of %t" } */
  scanf ("%tf", fp); /* { dg-warning "length" "bad use of %t" } */
  scanf ("%tF", fp); /* { dg-warning "length" "bad use of %t" } */
  scanf ("%tg", fp); /* { dg-warning "length" "bad use of %t" } */
  scanf ("%tG", fp); /* { dg-warning "length" "bad use of %t" } */
  scanf ("%ts", s); /* { dg-warning "length" "bad use of %t" } */
  scanf ("%t[ac]", s); /* { dg-warning "length" "bad use of %t" } */
  scanf ("%tc", s); /* { dg-warning "length" "bad use of %t" } */
  scanf ("%tp", pp); /* { dg-warning "length" "bad use of %t" } */
  scanf ("%La%LA%Le%LE%Lf%LF%Lg%LG", ldp, ldp, ldp, ldp, ldp, ldp, ldp, ldp);
  scanf ("%Ld", llp); /* { dg-warning "does not support" "bad use of %L" } */
  scanf ("%Li", llp); /* { dg-warning "does not support" "bad use of %L" } */
  scanf ("%Lo", ullp); /* { dg-warning "does not support" "bad use of %L" } */
  scanf ("%Lu", ullp); /* { dg-warning "does not support" "bad use of %L" } */
  scanf ("%Lx", ullp); /* { dg-warning "does not support" "bad use of %L" } */
  scanf ("%LX", ullp); /* { dg-warning "does not support" "bad use of %L" } */
  scanf ("%Ls", s); /* { dg-warning "length" "bad use of %L" } */
  scanf ("%L[ac]", s); /* { dg-warning "length" "bad use of %L" } */
  scanf ("%Lc", s); /* { dg-warning "length" "bad use of %L" } */
  scanf ("%Lp", pp); /* { dg-warning "length" "bad use of %L" } */
  scanf ("%Ln", n); /* { dg-warning "length" "bad use of %L" } */
  /* Valid uses of each bare conversion.  */
  scanf ("%d%i%o%u%x%X%a%A%e%E%f%F%g%G%s%[abc]%c%p%n%%", ip, ip, uip, uip, uip,
         uip, fp, fp, fp, fp, fp, fp, fp, fp, s, s, s, pp, n);
  /* Assert that %as is not treated as an extension in C99 mode.  */
  scanf ("%as", fp);
  scanf ("%a[", fp);
  /* Tests for bad argument types: pointer target sign with %hh.  */
  scanf ("%hhd", uhhp); /* { dg-warning "format" "%hhd sign" } */
  scanf ("%hhu", hhp); /* { dg-warning "format" "%hhu sign" } */
}
