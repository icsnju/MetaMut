/* Test for printf formats.  Formats using C90 features, including cases
   where C90 specifies some aspect of the format to be ignored or where
   the behavior is undefined.
*/
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1990 -pedantic -Wformat" } */

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
foo (int i, int i1, int i2, unsigned int u, double d, char *s, void *p,
     int *n, short int *hn, long int l, unsigned long int ul,
     long int *ln, long double ld, wint_t lc, wchar_t *ls, llong ll,
     ullong ull, unsigned int *un, const int *cn, signed char *ss,
     unsigned char *us, const signed char *css, unsigned int u1,
     unsigned int u2)
{
  /* See ISO/IEC 9899:1990 (E) subclause 7.9.6.1 (pages 131-134).  */
  /* Basic sanity checks for the different components of a format.  */
  printf ("%d\n", i);
  printf ("%+d\n", i);
  printf ("%3d\n", i);
  printf ("%-3d\n", i);
  printf ("%.7d\n", i);
  printf ("%+9.4d\n", i);
  printf ("%.3ld\n", l);
  printf ("%*d\n", i1, i);
  printf ("%.*d\n", i2, i);
  printf ("%*.*ld\n", i1, i2, l);
  printf ("%d %lu\n", i, ul);
  /* GCC has objected to the next one in the past, but it is a valid way
     of specifying zero precision.
  */
  printf ("%.e\n", d); /* { dg-bogus "precision" "bogus precision warning" } */
  /* Bogus use of width.  */
  printf ("%5n\n", n); /* { dg-warning "14:width" "width with %n" } */
  /* Erroneous, ignored or pointless constructs with precision.  */
  /* Whether negative values for precision may be included in the format
     string is not entirely clear; presume not, following Clive Feather's
     proposed resolution to DR#220 against C99.  In any case, such a
     construct should be warned about.
  */
  printf ("%.-5d\n", i); /* { dg-warning "14:format|precision" "negative precision warning" } */
  /* { dg-warning "too many arguments for format" "too many arguments" { target *-*-* } .-1 } */
  printf ("%.-*d\n", i); /* { dg-warning "14:format" "broken %.-*d format" } */
  /* { dg-warning "too many arguments for format" "too many arguments" { target *-*-* } .-1 } */
  printf ("%.3c\n", i); /* { dg-warning "15:precision" "precision with %c" } */
  printf ("%.3p\n", p); /* { dg-warning "15:precision" "precision with %p" } */
  printf ("%.3n\n", n); /* { dg-warning "15:precision" "precision with %n" } */
  /* Valid and invalid %% constructions.  Some of the warning messages
     are non-optimal, but they do detect the errorneous nature of the
     format string.
  */
  printf ("%%");
  printf ("%.3%"); /* { dg-warning "14:type" "missing type" } */
  /* { dg-warning "15:trailing" "bogus %%" { target *-*-* } .-1 } */
  printf ("%-%"); /* { dg-warning "13:type" "missing type" } */
  /* { dg-warning "14:trailing" "bogus %%" { target *-*-* } .-1 } */
  printf ("%-%\n"); /* { dg-warning "13:format" "bogus %%" } */
  /* { dg-warning "15:format" "bogus %%" { target *-*-* } .-1 } */
  printf ("%5%\n"); /* { dg-warning "13:format" "bogus %%" } */
  /* { dg-warning "15:format" "bogus %%" { target *-*-* } .-1 } */
  printf ("%h%\n"); /* { dg-warning "13:format" "bogus %%" } */
  /* { dg-warning "15:format" "bogus %%" { target *-*-* } .-1 } */
  /* Valid and invalid %h, %l, %L constructions.  */
  printf ("%hd", i);
  printf ("%hi", i);
  /* Strictly, these parameters should be int or unsigned int according to
     what unsigned short promotes to.  However, GCC ignores sign
     differences in format checking here, and this is relied on to get the
     correct checking without print_char_table needing to know whether
     int and short are the same size.
  */
  printf ("%ho%hu%hx%hX", u, u, u, u);
  printf ("%hn", hn);
  printf (" %hf", d); /* { dg-warning "15:length" "bad use of %h" } */
  printf (" %he", d); /* { dg-warning "15:length" "bad use of %h" } */
  printf (" %hE", d); /* { dg-warning "15:length" "bad use of %h" } */
  printf (" %hg", d); /* { dg-warning "15:length" "bad use of %h" } */
  printf (" %hG", d); /* { dg-warning "15:length" "bad use of %h" } */
  printf (" %hc", i); /* { dg-warning "15:length" "bad use of %h" } */
  printf (" %hs", s); /* { dg-warning "15:length" "bad use of %h" } */
  printf (" %hp", p); /* { dg-warning "15:length" "bad use of %h" } */
  printf (" %h"); /* { dg-warning "14:conversion lacks type" "bare %h" } */
  printf (" %h."); /* { dg-warning "15:conversion" "bogus %h." } */
  printf (" %ld%li%lo%lu%lx%lX", l, l, ul, ul, ul, ul);
  printf (" %ln", ln);
  printf (" %lf", d); /* { dg-warning "15:length|C" "bad use of %l" } */
  printf (" %le", d); /* { dg-warning "15:length|C" "bad use of %l" } */
  printf (" %lE", d); /* { dg-warning "15:length|C" "bad use of %l" } */
  printf (" %lg", d); /* { dg-warning "15:length|C" "bad use of %l" } */
  printf (" %lG", d); /* { dg-warning "15:length|C" "bad use of %l" } */
  printf (" %lp", p); /* { dg-warning "15:length|C" "bad use of %l" } */
  /* These next two were added in C94, but should be objected to in C90.
     For the first one, GCC has wanted wchar_t instead of the correct C94
     and C99 wint_t.
  */
  printf ("%lc", lc); /* { dg-warning "14:length|C" "C90 bad use of %l" } */
  printf ("%ls", ls); /* { dg-warning "14:length|C" "C90 bad use of %l" } */
  /* These uses of %L are legitimate, though GCC has wrongly warned for
     them in the past.
  */
  printf ("%Le%LE%Lf%Lg%LG", ld, ld, ld, ld, ld);
  /* These next six are accepted by GCC as referring to long long,
     but -pedantic correctly warns.
  */
  printf ("%Ld", ll); /* { dg-warning "14:does not support" "bad use of %L" } */
  printf ("%Li", ll); /* { dg-warning "14:does not support" "bad use of %L" } */
  printf ("%Lo", ull); /* { dg-warning "14:does not support" "bad use of %L" } */
  printf ("%Lu", ull); /* { dg-warning "14:does not support" "bad use of %L" } */
  printf ("%Lx", ull); /* { dg-warning "14:does not support" "bad use of %L" } */
  printf ("%LX", ull); /* { dg-warning "14:does not support" "bad use of %L" } */
  printf ("%Lc", i); /* { dg-warning "14:length" "bad use of %L" } */
  printf ("%Ls", s); /* { dg-warning "14:length" "bad use of %L" } */
  printf ("%Lp", p); /* { dg-warning "14:length" "bad use of %L" } */
  printf ("%Ln", n); /* { dg-warning "14:length" "bad use of %L" } */
  /* Valid uses of each bare conversion.  */
  printf ("%d%i%o%u%x%X%f%e%E%g%G%c%s%p%n%%", i, i, u, u, u, u, d, d, d, d, d,
	  i, s, p, n);
  /* Uses of the - flag (valid on all non-%, non-n conversions).  */
  printf ("%-d%-i%-o%-u%-x%-X%-f%-e%-E%-g%-G%-c%-s%-p", i, i, u, u, u, u,
	  d, d, d, d, d, i, s, p);
  printf ("%-n", n); /* { dg-warning "14:flag" "bad use of %-n" } */
  /* Uses of the + flag (valid on signed conversions only).  */
  printf ("%+d%+i%+f%+e%+E%+g%+G\n", i, i, d, d, d, d, d);
  printf ("%+o", u); /* { dg-warning "14:flag" "bad use of + flag" } */
  printf ("%+u", u); /* { dg-warning "14:flag" "bad use of + flag" } */
  printf ("%+x", u); /* { dg-warning "14:flag" "bad use of + flag" } */
  printf ("%+X", u); /* { dg-warning "14:flag" "bad use of + flag" } */
  printf ("%+c", i); /* { dg-warning "14:flag" "bad use of + flag" } */
  printf ("%+s", s); /* { dg-warning "14:flag" "bad use of + flag" } */
  printf ("%+p", p); /* { dg-warning "14:flag" "bad use of + flag" } */
  printf ("%+n", n); /* { dg-warning "14:flag" "bad use of + flag" } */
  /* Uses of the space flag (valid on signed conversions only, and ignored
     with +).
  */
  printf ("% +d", i); /* { dg-warning "11:use of both|ignored" "use of space and + flags" } */
  printf ("%+ d", i); /* { dg-warning "11:use of both|ignored" "use of space and + flags" } */
  printf ("% d% i% f% e% E% g% G\n", i, i, d, d, d, d, d);
  printf ("% o", u); /* { dg-warning "14:flag" "bad use of space flag" } */
  printf ("% u", u); /* { dg-warning "14:flag" "bad use of space flag" } */
  printf ("% x", u); /* { dg-warning "14:flag" "bad use of space flag" } */
  printf ("% X", u); /* { dg-warning "14:flag" "bad use of space flag" } */
  printf ("% c", i); /* { dg-warning "14:flag" "bad use of space flag" } */
  printf ("% s", s); /* { dg-warning "14:flag" "bad use of space flag" } */
  printf ("% p", p); /* { dg-warning "14:flag" "bad use of space flag" } */
  printf ("% n", n); /* { dg-warning "14:flag" "bad use of space flag" } */
  /* Uses of the # flag.  */
  printf ("%#o%#x%#X%#e%#E%#f%#g%#G", u, u, u, d, d, d, d, d);
  printf ("%#d", i); /* { dg-warning "14:flag" "bad use of # flag" } */
  printf ("%#i", i); /* { dg-warning "14:flag" "bad use of # flag" } */
  printf ("%#u", u); /* { dg-warning "14:flag" "bad use of # flag" } */
  printf ("%#c", i); /* { dg-warning "14:flag" "bad use of # flag" } */
  printf ("%#s", s); /* { dg-warning "14:flag" "bad use of # flag" } */
  printf ("%#p", p); /* { dg-warning "14:flag" "bad use of # flag" } */
  printf ("%#n", n); /* { dg-warning "14:flag" "bad use of # flag" } */
  /* Uses of the 0 flag.  */
  printf ("%08d%08i%08o%08u%08x%08X%08e%08E%08f%08g%08G", i, i, u, u, u, u,
	  d, d, d, d, d);
  printf ("%0c", i); /* { dg-warning "14:flag" "bad use of 0 flag" } */
  printf ("%0s", s); /* { dg-warning "14:flag" "bad use of 0 flag" } */
  printf ("%0p", p); /* { dg-warning "14:flag" "bad use of 0 flag" } */
  printf ("%0n", n); /* { dg-warning "14:flag" "bad use of 0 flag" } */
  /* 0 flag ignored with precision for certain types, not others.  */
  printf ("%08.5d", i); /* { dg-warning "11:ignored" "0 flag ignored with precision" } */
  printf ("%08.5i", i); /* { dg-warning "11:ignored" "0 flag ignored with precision" } */
  printf ("%08.5o", u); /* { dg-warning "11:ignored" "0 flag ignored with precision" } */
  printf ("%08.5u", u); /* { dg-warning "11:ignored" "0 flag ignored with precision" } */
  printf ("%08.5x", u); /* { dg-warning "11:ignored" "0 flag ignored with precision" } */
  printf ("%08.5X", u); /* { dg-warning "11:ignored" "0 flag ignored with precision" } */
  printf ("%08.5f%08.5e%08.5E%08.5g%08.5G", d, d, d, d, d);
  /* 0 flag ignored with - flag.  */
  printf ("%-08d", i); /* { dg-warning "11:flags|ignored" "0 flag ignored with - flag" } */
  printf ("%-08i", i); /* { dg-warning "11:flags|ignored" "0 flag ignored with - flag" } */
  printf ("%-08o", u); /* { dg-warning "11:flags|ignored" "0 flag ignored with - flag" } */
  printf ("%-08u", u); /* { dg-warning "11:flags|ignored" "0 flag ignored with - flag" } */
  printf ("%-08x", u); /* { dg-warning "11:flags|ignored" "0 flag ignored with - flag" } */
  printf ("%-08X", u); /* { dg-warning "11:flags|ignored" "0 flag ignored with - flag" } */
  printf ("%-08e", d); /* { dg-warning "11:flags|ignored" "0 flag ignored with - flag" } */
  printf ("%-08E", d); /* { dg-warning "11:flags|ignored" "0 flag ignored with - flag" } */
  printf ("%-08f", d); /* { dg-warning "11:flags|ignored" "0 flag ignored with - flag" } */
  printf ("%-08g", d); /* { dg-warning "11:flags|ignored" "0 flag ignored with - flag" } */
  printf ("%-08G", d); /* { dg-warning "11:flags|ignored" "0 flag ignored with - flag" } */
  /* Various tests of bad argument types.  */
  printf ("%d", l); /* { dg-warning "13:format" "bad argument types" } */
  printf ("%*.*d", l, i2, i); /* { dg-warning "13:field" "bad * argument types" } */
  printf ("%*.*d", i1, l, i); /* { dg-warning "15:field" "bad * argument types" } */
  printf ("%ld", i); /* { dg-warning "14:format" "bad argument types" } */
  printf ("%s", n); /* { dg-warning "13:format" "bad argument types" } */
  printf ("%p", i); /* { dg-warning "13:format" "bad argument types" } */
  printf ("%n", p); /* { dg-warning "13:format" "bad argument types" } */
  /* With -pedantic, we want some further checks for pointer targets:
     %p should allow only pointers to void (possibly qualified) and
     to character types (possibly qualified), but not function pointers
     or pointers to other types.  (Whether, in fact, character types are
     allowed here is unclear; see thread on comp.std.c, July 2000 for
     discussion of the requirements of rules on identical representation,
     and of the application of the as if rule with the new va_arg
     allowances in C99 to printf.)  Likewise, we should warn if
     pointer targets differ in signedness, except in some circumstances
     for character pointers.  (In C99 we should consider warning for
     char * or unsigned char * being passed to %hhn, even if strictly
     legitimate by the standard.)
  */
  printf ("%p", foo); /* { dg-warning "13:format" "bad argument types" } */
  printf ("%n", un); /* { dg-warning "13:format" "bad argument types" } */
  printf ("%p", n); /* { dg-warning "13:format" "bad argument types" } */
  /* Allow character pointers with %p.  */
  printf ("%p%p%p%p", s, ss, us, css);
  /* %s allows any character type.  */
  printf ("%s%s%s%s", s, ss, us, css);
  /* Warning for void * arguments for %s is GCC's historical behavior,
     and seems useful to keep, even if some standard versions might be
     read to permit it.
  */
  printf ("%s", p); /* { dg-warning "13:format" "bad argument types" } */
  /* The historical behavior is to allow signed / unsigned types
     interchangeably as arguments.  For values representable in both types,
     such usage may be correct.  For now preserve the behavior of GCC
     in such cases.
  */
  printf ("%d", u);
  /* Also allow the same for width and precision arguments.  In the past,
     GCC has been inconsistent and allowed unsigned for width but not
     precision.
  */
  printf ("%*.*d", u1, u2, i);
  /* Wrong number of arguments.  */
  printf ("%d%d", i); /* { dg-warning "15:matching" "wrong number of args" } */
  printf ("%d", i, i); /* { dg-warning "11:arguments" "wrong number of args" } */
  /* Miscellaneous bogus constructions.  */
  printf (""); /* { dg-warning "11:zero-length" "warning for empty format" } */
  printf ("\0"); /* { dg-warning "12:embedded" "warning for embedded NUL" } */
  printf ("%d\0", i); /* { dg-warning "14:embedded" "warning for embedded NUL" } */
  printf ("%d\0%d", i, i); /* { dg-warning "embedded|too many" "warning for embedded NUL" } */
  printf (NULL); /* { dg-warning "3:null" "null format string warning" } */
  printf ("%"); /* { dg-warning "12:trailing" "trailing % warning" } */
  printf ("%++d", i); /* { dg-warning "14:repeated" "repeated flag warning" } */
  printf ("%n", cn); /* { dg-warning "3:constant" "%n with const" } */
  printf ((const char *)L"foo"); /* { dg-warning "25:wide" "wide string" } */
  printf ("%n", (int *)0); /* { dg-warning "3:null" "%n with NULL" } */
  printf ("%s", (char *)0); /* { dg-warning "12:'%s' directive argument is null" "%s with NULL" } */
  /* Test for correct column locations within strings with embedded
     escape sequences. */
  printf ("\\\a\n \"\t%5n\n", n); /* { dg-warning "25:width" "width with %n" } */
  printf ("\\a\\n%5n\n", n); /* { dg-warning "20:width" "width with %n" } */
}
