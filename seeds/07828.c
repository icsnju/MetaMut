/* Test for printf formats.  Formats using C99 features, including cases
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
foo (int i, unsigned int u, double d, char *s, void *p, int *n,
     long double ld, wint_t lc, wchar_t *ls, long long int ll,
     unsigned long long int ull, signed char *ss, unsigned char *us,
     long long int *lln, intmax_t j, uintmax_t uj, intmax_t *jn,
     size_t z, signed_size_t sz, signed_size_t *zn,
     ptrdiff_t t, ptrdiff_t *tn)
{
  /* See ISO/IEC 9899:1999 (E) subclause 7.19.6.1 (pages 273-281).
     We do not repeat here most of the checks for correct C90 formats
     or completely broken formats.
  */
  /* Valid and invalid %h, %hh, %l, %ll, %j, %z, %t, %L constructions.  */
  printf ("%hf", d); /* { dg-warning "length" "bad use of %h" } */
  printf ("%hF", d); /* { dg-warning "length" "bad use of %h" } */
  printf ("%he", d); /* { dg-warning "length" "bad use of %h" } */
  printf ("%hE", d); /* { dg-warning "length" "bad use of %h" } */
  printf ("%hg", d); /* { dg-warning "length" "bad use of %h" } */
  printf ("%hG", d); /* { dg-warning "length" "bad use of %h" } */
  printf ("%ha", d); /* { dg-warning "length" "bad use of %h" } */
  printf ("%hA", d); /* { dg-warning "length" "bad use of %h" } */
  printf ("%hc", i); /* { dg-warning "length" "bad use of %h" } */
  printf ("%hs", s); /* { dg-warning "length" "bad use of %h" } */
  printf ("%hp", p); /* { dg-warning "length" "bad use of %h" } */
  printf ("%hhd%hhi%hho%hhu%hhx%hhX", i, i, u, u, u, u);
  printf ("%hhn", ss);
  printf ("%hhf", d); /* { dg-warning "length" "bad use of %hh" } */
  printf ("%hhF", d); /* { dg-warning "length" "bad use of %hh" } */
  printf ("%hhe", d); /* { dg-warning "length" "bad use of %hh" } */
  printf ("%hhE", d); /* { dg-warning "length" "bad use of %hh" } */
  printf ("%hhg", d); /* { dg-warning "length" "bad use of %hh" } */
  printf ("%hhG", d); /* { dg-warning "length" "bad use of %hh" } */
  printf ("%hha", d); /* { dg-warning "length" "bad use of %hh" } */
  printf ("%hhA", d); /* { dg-warning "length" "bad use of %hh" } */
  printf ("%hhc", i); /* { dg-warning "length" "bad use of %hh" } */
  printf ("%hhs", s); /* { dg-warning "length" "bad use of %hh" } */
  printf ("%hhp", p); /* { dg-warning "length" "bad use of %hh" } */
  printf ("%lc", lc);
  printf ("%ls", ls);
  printf ("%lf%lF%le%lE%lg%lG%la%lA", d, d, d, d, d, d, d, d);
  printf ("%lp", p); /* { dg-warning "length|C" "bad use of %l" } */
  printf ("%lld%lli%llo%llu%llx%llX", ll, ll, ull, ull, ull, ull);
  printf ("%lln", lln);
  printf ("%llf", d); /* { dg-warning "length" "bad use of %ll" } */
  printf ("%llF", d); /* { dg-warning "length" "bad use of %ll" } */
  printf ("%lle", d); /* { dg-warning "length" "bad use of %ll" } */
  printf ("%llE", d); /* { dg-warning "length" "bad use of %ll" } */
  printf ("%llg", d); /* { dg-warning "length" "bad use of %ll" } */
  printf ("%llG", d); /* { dg-warning "length" "bad use of %ll" } */
  printf ("%lla", d); /* { dg-warning "length" "bad use of %ll" } */
  printf ("%llA", d); /* { dg-warning "length" "bad use of %ll" } */
  printf ("%llc", i); /* { dg-warning "length" "bad use of %ll" } */
  printf ("%lls", s); /* { dg-warning "length" "bad use of %ll" } */
  printf ("%llp", p); /* { dg-warning "length" "bad use of %ll" } */
  printf ("%jd%ji%jo%ju%jx%jX", j, j, uj, uj, uj, uj); /* { dg-bogus "length" "bogus %j warning" } */
  printf ("%jn", jn); /* { dg-bogus "length" "bogus %j warning" } */
  printf ("%jf", d); /* { dg-warning "length" "bad use of %j" } */
  printf ("%jF", d); /* { dg-warning "length" "bad use of %j" } */
  printf ("%je", d); /* { dg-warning "length" "bad use of %j" } */
  printf ("%jE", d); /* { dg-warning "length" "bad use of %j" } */
  printf ("%jg", d); /* { dg-warning "length" "bad use of %j" } */
  printf ("%jG", d); /* { dg-warning "length" "bad use of %j" } */
  printf ("%ja", d); /* { dg-warning "length" "bad use of %j" } */
  printf ("%jA", d); /* { dg-warning "length" "bad use of %j" } */
  printf ("%jc", i); /* { dg-warning "length" "bad use of %j" } */
  printf ("%js", s); /* { dg-warning "length" "bad use of %j" } */
  printf ("%jp", p); /* { dg-warning "length" "bad use of %j" } */
  printf ("%zd%zi%zo%zu%zx%zX", sz, sz, z, z, z, z);
  printf ("%zn", zn);
  printf ("%zf", d); /* { dg-warning "length" "bad use of %z" } */
  printf ("%zF", d); /* { dg-warning "length" "bad use of %z" } */
  printf ("%ze", d); /* { dg-warning "length" "bad use of %z" } */
  printf ("%zE", d); /* { dg-warning "length" "bad use of %z" } */
  printf ("%zg", d); /* { dg-warning "length" "bad use of %z" } */
  printf ("%zG", d); /* { dg-warning "length" "bad use of %z" } */
  printf ("%za", d); /* { dg-warning "length" "bad use of %z" } */
  printf ("%zA", d); /* { dg-warning "length" "bad use of %z" } */
  printf ("%zc", i); /* { dg-warning "length" "bad use of %z" } */
  printf ("%zs", s); /* { dg-warning "length" "bad use of %z" } */
  printf ("%zp", p); /* { dg-warning "length" "bad use of %z" } */
  printf ("%td%ti%to%tu%tx%tX", t, t, t, t, t, t);
  printf ("%tn", tn);
  printf ("%tf", d); /* { dg-warning "length" "bad use of %t" } */
  printf ("%tF", d); /* { dg-warning "length" "bad use of %t" } */
  printf ("%te", d); /* { dg-warning "length" "bad use of %t" } */
  printf ("%tE", d); /* { dg-warning "length" "bad use of %t" } */
  printf ("%tg", d); /* { dg-warning "length" "bad use of %t" } */
  printf ("%tG", d); /* { dg-warning "length" "bad use of %t" } */
  printf ("%ta", d); /* { dg-warning "length" "bad use of %t" } */
  printf ("%tA", d); /* { dg-warning "length" "bad use of %t" } */
  printf ("%tc", i); /* { dg-warning "length" "bad use of %t" } */
  printf ("%ts", s); /* { dg-warning "length" "bad use of %t" } */
  printf ("%tp", p); /* { dg-warning "length" "bad use of %t" } */
  printf ("%Le%LE%Lf%LF%Lg%LG%La%LA", ld, ld, ld, ld, ld, ld, ld, ld);
  /* These next six are accepted by GCC as referring to long long,
     but -pedantic correctly warns.
  */
  printf ("%Ld", ll); /* { dg-warning "does not support" "bad use of %L" } */
  printf ("%Li", ll); /* { dg-warning "does not support" "bad use of %L" } */
  printf ("%Lo", ull); /* { dg-warning "does not support" "bad use of %L" } */
  printf ("%Lu", ull); /* { dg-warning "does not support" "bad use of %L" } */
  printf ("%Lx", ull); /* { dg-warning "does not support" "bad use of %L" } */
  printf ("%LX", ull); /* { dg-warning "does not support" "bad use of %L" } */
  printf ("%Lc", i); /* { dg-warning "length" "bad use of %L" } */
  printf ("%Ls", s); /* { dg-warning "length" "bad use of %L" } */
  printf ("%Lp", p); /* { dg-warning "length" "bad use of %L" } */
  printf ("%Ln", n); /* { dg-warning "length" "bad use of %L" } */
  /* Valid uses of each bare conversion.  */
  printf ("%d%i%o%u%x%X%f%F%e%E%g%G%a%A%c%s%p%n%%", i, i, u, u, u, u,
	  d, d, d, d, d, d, d, d, i, s, p, n);
  /* Uses of the - flag (valid on all non-%, non-n conversions).  */
  printf ("%-d%-i%-o%-u%-x%-X%-f%-F%-e%-E%-g%-G%-a%-A%-c%-s%-p", i, i,
	  u, u, u, u, d, d, d, d, d, d, d, d, i, s, p);
  printf ("%-n", n); /* { dg-warning "flag" "bad use of %-n" } */
  /* Uses of the + flag (valid on signed conversions only).  */
  printf ("%+d%+i%+f%+F%+e%+E%+g%+G%+a%+A\n", i, i, d, d, d, d, d, d, d, d);
  printf ("%+o", u); /* { dg-warning "flag" "bad use of + flag" } */
  printf ("%+u", u); /* { dg-warning "flag" "bad use of + flag" } */
  printf ("%+x", u); /* { dg-warning "flag" "bad use of + flag" } */
  printf ("%+X", u); /* { dg-warning "flag" "bad use of + flag" } */
  printf ("%+c", i); /* { dg-warning "flag" "bad use of + flag" } */
  printf ("%+s", s); /* { dg-warning "flag" "bad use of + flag" } */
  printf ("%+p", p); /* { dg-warning "flag" "bad use of + flag" } */
  printf ("%+n", n); /* { dg-warning "flag" "bad use of + flag" } */
  /* Uses of the space flag (valid on signed conversions only, and ignored
     with +).
  */
  printf ("% +d", i); /* { dg-warning "use of both|ignored" "use of space and + flags" } */
  printf ("%+ d", i); /* { dg-warning "use of both|ignored" "use of space and + flags" } */
  printf ("% d% i% f% F% e% E% g% G% a% A\n", i, i, d, d, d, d, d, d, d, d);
  printf ("% o", u); /* { dg-warning "flag" "bad use of space flag" } */
  printf ("% u", u); /* { dg-warning "flag" "bad use of space flag" } */
  printf ("% x", u); /* { dg-warning "flag" "bad use of space flag" } */
  printf ("% X", u); /* { dg-warning "flag" "bad use of space flag" } */
  printf ("% c", i); /* { dg-warning "flag" "bad use of space flag" } */
  printf ("% s", s); /* { dg-warning "flag" "bad use of space flag" } */
  printf ("% p", p); /* { dg-warning "flag" "bad use of space flag" } */
  printf ("% n", n); /* { dg-warning "flag" "bad use of space flag" } */
  /* Uses of the # flag.  */
  printf ("%#o%#x%#X%#e%#E%#f%#F%#g%#G%#a%#A", u, u, u, d, d, d, d,
	  d, d, d, d);
  printf ("%#d", i); /* { dg-warning "flag" "bad use of # flag" } */
  printf ("%#i", i); /* { dg-warning "flag" "bad use of # flag" } */
  printf ("%#u", u); /* { dg-warning "flag" "bad use of # flag" } */
  printf ("%#c", i); /* { dg-warning "flag" "bad use of # flag" } */
  printf ("%#s", s); /* { dg-warning "flag" "bad use of # flag" } */
  printf ("%#p", p); /* { dg-warning "flag" "bad use of # flag" } */
  printf ("%#n", n); /* { dg-warning "flag" "bad use of # flag" } */
  /* Uses of the 0 flag.  */
  printf ("%08d%08i%08o%08u%08x%08X%08e%08E%08f%08F%08g%08G%08a%08A", i, i,
	  u, u, u, u, d, d, d, d, d, d, d, d);
  printf ("%0c", i); /* { dg-warning "flag" "bad use of 0 flag" } */
  printf ("%0s", s); /* { dg-warning "flag" "bad use of 0 flag" } */
  printf ("%0p", p); /* { dg-warning "flag" "bad use of 0 flag" } */
  printf ("%0n", n); /* { dg-warning "flag" "bad use of 0 flag" } */
  /* 0 flag ignored with precision for certain types, not others.  */
  printf ("%08.5d", i); /* { dg-warning "ignored" "0 flag ignored with precision" } */
  printf ("%08.5i", i); /* { dg-warning "ignored" "0 flag ignored with precision" } */
  printf ("%08.5o", u); /* { dg-warning "ignored" "0 flag ignored with precision" } */
  printf ("%08.5u", u); /* { dg-warning "ignored" "0 flag ignored with precision" } */
  printf ("%08.5x", u); /* { dg-warning "ignored" "0 flag ignored with precision" } */
  printf ("%08.5X", u); /* { dg-warning "ignored" "0 flag ignored with precision" } */
  printf ("%08.5f%08.5F%08.5e%08.5E%08.5g%08.5G%08.5a%08.5A",
	  d, d, d, d, d, d, d, d);
  /* 0 flag ignored with - flag.  */
  printf ("%-08d", i); /* { dg-warning "flags|ignored" "0 flag ignored with - flag" } */
  printf ("%-08i", i); /* { dg-warning "flags|ignored" "0 flag ignored with - flag" } */
  printf ("%-08o", u); /* { dg-warning "flags|ignored" "0 flag ignored with - flag" } */
  printf ("%-08u", u); /* { dg-warning "flags|ignored" "0 flag ignored with - flag" } */
  printf ("%-08x", u); /* { dg-warning "flags|ignored" "0 flag ignored with - flag" } */
  printf ("%-08X", u); /* { dg-warning "flags|ignored" "0 flag ignored with - flag" } */
  printf ("%-08e", d); /* { dg-warning "flags|ignored" "0 flag ignored with - flag" } */
  printf ("%-08E", d); /* { dg-warning "flags|ignored" "0 flag ignored with - flag" } */
  printf ("%-08f", d); /* { dg-warning "flags|ignored" "0 flag ignored with - flag" } */
  printf ("%-08F", d); /* { dg-warning "flags|ignored" "0 flag ignored with - flag" } */
  printf ("%-08g", d); /* { dg-warning "flags|ignored" "0 flag ignored with - flag" } */
  printf ("%-08G", d); /* { dg-warning "flags|ignored" "0 flag ignored with - flag" } */
  printf ("%-08a", d); /* { dg-warning "flags|ignored" "0 flag ignored with - flag" } */
  printf ("%-08A", d); /* { dg-warning "flags|ignored" "0 flag ignored with - flag" } */
  /* Various tests of bad argument types.  Mostly covered in c90-printf-1.c;
     here just test for pointer target sign with %hhn.  (Probably allowed
     by the standard, but a bad idea, so GCC should diagnose if what
     is used is not signed char *.)
  */
  printf ("%hhn", s); /* { dg-warning "format" "%hhn plain char" } */
  printf ("%hhn", us); /* { dg-warning "format" "%hhn unsigned char" } */
}
