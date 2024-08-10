/* Test for format extensions beyond the C standard and X/Open standard.
   Test for strftime formats.
*/
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do compile } */
/* { dg-options "-std=gnu99 -Wformat -Wformat-y2k" } */

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
  /* GCC accepts the "-", "_" and "0" flags to control padding on numeric
     formats.  It also accepts width on these formats.
  */
  /* Basic tests of parts on their own.  */
  strftime (s, m, "%5C%-C%_C%0C", tp);
  /* Correct usages.  */
  strftime (s, m, "%-5C%_5C%05C%-5d%_5d%05d%-5e%_5e%05e%-5G%_5G%05G", tp);
  strftime (s, m, "%-5H%_5H%05H%-5I%_5I%05I%-5j%_5j%05j%-5m%_5m%05m", tp);
  strftime (s, m, "%-5M%_5M%05M%-5S%_5S%05S%-5u%_5u%05u%-5U%_5U%05U", tp);
  strftime (s, m, "%-5V%_5V%05V%-5w%_5w%05w%-5W%_5W%05W%-5Y%_5Y%05Y", tp);
  /* Correct usages with GNU extension conversion characters.  */
  strftime (s, m, "%-5k%_5k%05k%-5l%_5l%05l%-20s%_20s%020s", tp);
  /* Correct usages with Y2K problems.  */
  strftime (s, m, "%-5g%_5g%05g%-5y%_5y%05y", tp); /* { dg-warning "only last 2" "2-digit year" } */
  /* Incorrect usages.  */
  strftime (s, m, "%5a", tp); /* { dg-warning "width" "bad %a" } */
  strftime (s, m, "%-a", tp); /* { dg-warning "flag" "bad %a" } */
  strftime (s, m, "%_a", tp); /* { dg-warning "flag" "bad %a" } */
  strftime (s, m, "%0a", tp); /* { dg-warning "flag" "bad %a" } */
  strftime (s, m, "%5A", tp); /* { dg-warning "width" "bad %A" } */
  strftime (s, m, "%-A", tp); /* { dg-warning "flag" "bad %A" } */
  strftime (s, m, "%_A", tp); /* { dg-warning "flag" "bad %A" } */
  strftime (s, m, "%0A", tp); /* { dg-warning "flag" "bad %A" } */
  strftime (s, m, "%5b", tp); /* { dg-warning "width" "bad %b" } */
  strftime (s, m, "%-b", tp); /* { dg-warning "flag" "bad %b" } */
  strftime (s, m, "%_b", tp); /* { dg-warning "flag" "bad %b" } */
  strftime (s, m, "%0b", tp); /* { dg-warning "flag" "bad %b" } */
  strftime (s, m, "%5B", tp); /* { dg-warning "width" "bad %B" } */
  strftime (s, m, "%-B", tp); /* { dg-warning "flag" "bad %B" } */
  strftime (s, m, "%_B", tp); /* { dg-warning "flag" "bad %B" } */
  strftime (s, m, "%0B", tp); /* { dg-warning "flag" "bad %B" } */
  strftime (s, m, "%5F", tp); /* { dg-warning "width" "bad %F" } */
  strftime (s, m, "%-F", tp); /* { dg-warning "flag" "bad %F" } */
  strftime (s, m, "%_F", tp); /* { dg-warning "flag" "bad %F" } */
  strftime (s, m, "%0F", tp); /* { dg-warning "flag" "bad %F" } */
  strftime (s, m, "%5h", tp); /* { dg-warning "width" "bad %h" } */
  strftime (s, m, "%-h", tp); /* { dg-warning "flag" "bad %h" } */
  strftime (s, m, "%_h", tp); /* { dg-warning "flag" "bad %h" } */
  strftime (s, m, "%0h", tp); /* { dg-warning "flag" "bad %h" } */
  strftime (s, m, "%5n", tp); /* { dg-warning "width" "bad %n" } */
  strftime (s, m, "%-n", tp); /* { dg-warning "flag" "bad %n" } */
  strftime (s, m, "%_n", tp); /* { dg-warning "flag" "bad %n" } */
  strftime (s, m, "%0n", tp); /* { dg-warning "flag" "bad %n" } */
  strftime (s, m, "%5p", tp); /* { dg-warning "width" "bad %p" } */
  strftime (s, m, "%-p", tp); /* { dg-warning "flag" "bad %p" } */
  strftime (s, m, "%_p", tp); /* { dg-warning "flag" "bad %p" } */
  strftime (s, m, "%0p", tp); /* { dg-warning "flag" "bad %p" } */
  strftime (s, m, "%5r", tp); /* { dg-warning "width" "bad %r" } */
  strftime (s, m, "%-r", tp); /* { dg-warning "flag" "bad %r" } */
  strftime (s, m, "%_r", tp); /* { dg-warning "flag" "bad %r" } */
  strftime (s, m, "%0r", tp); /* { dg-warning "flag" "bad %r" } */
  strftime (s, m, "%5R", tp); /* { dg-warning "width" "bad %R" } */
  strftime (s, m, "%-R", tp); /* { dg-warning "flag" "bad %R" } */
  strftime (s, m, "%_R", tp); /* { dg-warning "flag" "bad %R" } */
  strftime (s, m, "%0R", tp); /* { dg-warning "flag" "bad %R" } */
  strftime (s, m, "%5t", tp); /* { dg-warning "width" "bad %t" } */
  strftime (s, m, "%-t", tp); /* { dg-warning "flag" "bad %t" } */
  strftime (s, m, "%_t", tp); /* { dg-warning "flag" "bad %t" } */
  strftime (s, m, "%0t", tp); /* { dg-warning "flag" "bad %t" } */
  strftime (s, m, "%5T", tp); /* { dg-warning "width" "bad %T" } */
  strftime (s, m, "%-T", tp); /* { dg-warning "flag" "bad %T" } */
  strftime (s, m, "%_T", tp); /* { dg-warning "flag" "bad %T" } */
  strftime (s, m, "%0T", tp); /* { dg-warning "flag" "bad %T" } */
  strftime (s, m, "%5X", tp); /* { dg-warning "width" "bad %X" } */
  strftime (s, m, "%-X", tp); /* { dg-warning "flag" "bad %X" } */
  strftime (s, m, "%_X", tp); /* { dg-warning "flag" "bad %X" } */
  strftime (s, m, "%0X", tp); /* { dg-warning "flag" "bad %X" } */
  strftime (s, m, "%5z", tp); /* { dg-warning "width" "bad %z" } */
  strftime (s, m, "%-z", tp); /* { dg-warning "flag" "bad %z" } */
  strftime (s, m, "%_z", tp); /* { dg-warning "flag" "bad %z" } */
  strftime (s, m, "%0z", tp); /* { dg-warning "flag" "bad %z" } */
  strftime (s, m, "%5Z", tp); /* { dg-warning "width" "bad %Z" } */
  strftime (s, m, "%-Z", tp); /* { dg-warning "flag" "bad %Z" } */
  strftime (s, m, "%_Z", tp); /* { dg-warning "flag" "bad %Z" } */
  strftime (s, m, "%0Z", tp); /* { dg-warning "flag" "bad %Z" } */

  /* Incorrect usages with Y2K problems.  */
  strftime (s, m, "%5c", tp); /* { dg-warning "width" "bad %c" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%-c", tp); /* { dg-warning "flag" "bad %c" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%_c", tp); /* { dg-warning "flag" "bad %c" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%0c", tp); /* { dg-warning "flag" "bad %c" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%5D", tp); /* { dg-warning "width" "bad %D" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%-D", tp); /* { dg-warning "flag" "bad %D" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%_D", tp); /* { dg-warning "flag" "bad %D" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%0D", tp); /* { dg-warning "flag" "bad %D" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%5x", tp); /* { dg-warning "width" "bad %x" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%-x", tp); /* { dg-warning "flag" "bad %x" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%_x", tp); /* { dg-warning "flag" "bad %x" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%0x", tp); /* { dg-warning "flag" "bad %x" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */

  /* Incorrect usages with GNU extension conversion characters.  */
  strftime (s, m, "%5P", tp); /* { dg-warning "width" "bad %P" } */
  strftime (s, m, "%-P", tp); /* { dg-warning "flag" "bad %P" } */
  strftime (s, m, "%_P", tp); /* { dg-warning "flag" "bad %P" } */
  strftime (s, m, "%0P", tp); /* { dg-warning "flag" "bad %P" } */
  /* The "^" and "#" flags control the case of the output.
     ^ (uppercase) makes sense on aAbBhZ; # (change case) makes sense
     on the same and on p.
  */
  strftime (s, m, "%^a%#a%^A%#A%^b%#b%^B%#B%^h%#h%^Z%#Z%#p", tp);
  /* Bad usages.  */
  strftime (s, m, "%^C", tp); /* { dg-warning "flag" "bad %C" } */
  strftime (s, m, "%#C", tp); /* { dg-warning "flag" "bad %C" } */
  strftime (s, m, "%^d", tp); /* { dg-warning "flag" "bad %d" } */
  strftime (s, m, "%#d", tp); /* { dg-warning "flag" "bad %d" } */
  strftime (s, m, "%^e", tp); /* { dg-warning "flag" "bad %e" } */
  strftime (s, m, "%#e", tp); /* { dg-warning "flag" "bad %e" } */
  strftime (s, m, "%^F", tp); /* { dg-warning "flag" "bad %F" } */
  strftime (s, m, "%#F", tp); /* { dg-warning "flag" "bad %F" } */
  strftime (s, m, "%^G", tp); /* { dg-warning "flag" "bad %G" } */
  strftime (s, m, "%#G", tp); /* { dg-warning "flag" "bad %G" } */
  strftime (s, m, "%^H", tp); /* { dg-warning "flag" "bad %H" } */
  strftime (s, m, "%#H", tp); /* { dg-warning "flag" "bad %H" } */
  strftime (s, m, "%^I", tp); /* { dg-warning "flag" "bad %I" } */
  strftime (s, m, "%#I", tp); /* { dg-warning "flag" "bad %I" } */
  strftime (s, m, "%^j", tp); /* { dg-warning "flag" "bad %j" } */
  strftime (s, m, "%#j", tp); /* { dg-warning "flag" "bad %j" } */
  strftime (s, m, "%^m", tp); /* { dg-warning "flag" "bad %m" } */
  strftime (s, m, "%#m", tp); /* { dg-warning "flag" "bad %m" } */
  strftime (s, m, "%^M", tp); /* { dg-warning "flag" "bad %M" } */
  strftime (s, m, "%#M", tp); /* { dg-warning "flag" "bad %M" } */
  strftime (s, m, "%^n", tp); /* { dg-warning "flag" "bad %n" } */
  strftime (s, m, "%#n", tp); /* { dg-warning "flag" "bad %n" } */
  strftime (s, m, "%^p", tp); /* { dg-warning "flag" "bad %p" } */
  strftime (s, m, "%^r", tp); /* { dg-warning "flag" "bad %r" } */
  strftime (s, m, "%#r", tp); /* { dg-warning "flag" "bad %r" } */
  strftime (s, m, "%^R", tp); /* { dg-warning "flag" "bad %R" } */
  strftime (s, m, "%#R", tp); /* { dg-warning "flag" "bad %R" } */
  strftime (s, m, "%^S", tp); /* { dg-warning "flag" "bad %S" } */
  strftime (s, m, "%#S", tp); /* { dg-warning "flag" "bad %S" } */
  strftime (s, m, "%^t", tp); /* { dg-warning "flag" "bad %t" } */
  strftime (s, m, "%#t", tp); /* { dg-warning "flag" "bad %t" } */
  strftime (s, m, "%^T", tp); /* { dg-warning "flag" "bad %T" } */
  strftime (s, m, "%#T", tp); /* { dg-warning "flag" "bad %T" } */
  strftime (s, m, "%^u", tp); /* { dg-warning "flag" "bad %u" } */
  strftime (s, m, "%#u", tp); /* { dg-warning "flag" "bad %u" } */
  strftime (s, m, "%^U", tp); /* { dg-warning "flag" "bad %U" } */
  strftime (s, m, "%#U", tp); /* { dg-warning "flag" "bad %U" } */
  strftime (s, m, "%^V", tp); /* { dg-warning "flag" "bad %V" } */
  strftime (s, m, "%#V", tp); /* { dg-warning "flag" "bad %V" } */
  strftime (s, m, "%^w", tp); /* { dg-warning "flag" "bad %w" } */
  strftime (s, m, "%#w", tp); /* { dg-warning "flag" "bad %w" } */
  strftime (s, m, "%^W", tp); /* { dg-warning "flag" "bad %W" } */
  strftime (s, m, "%#W", tp); /* { dg-warning "flag" "bad %W" } */
  strftime (s, m, "%^X", tp); /* { dg-warning "flag" "bad %X" } */
  strftime (s, m, "%#X", tp); /* { dg-warning "flag" "bad %X" } */
  strftime (s, m, "%^Y", tp); /* { dg-warning "flag" "bad %Y" } */
  strftime (s, m, "%#Y", tp); /* { dg-warning "flag" "bad %Y" } */
  strftime (s, m, "%^z", tp); /* { dg-warning "flag" "bad %z" } */
  strftime (s, m, "%#z", tp); /* { dg-warning "flag" "bad %z" } */
  strftime (s, m, "%^P", tp); /* { dg-warning "flag" "bad %P" } */
  strftime (s, m, "%#P", tp); /* { dg-warning "flag" "bad %P" } */
  strftime (s, m, "%^k", tp); /* { dg-warning "flag" "bad %k" } */
  strftime (s, m, "%#k", tp); /* { dg-warning "flag" "bad %k" } */
  strftime (s, m, "%^l", tp); /* { dg-warning "flag" "bad %l" } */
  strftime (s, m, "%#l", tp); /* { dg-warning "flag" "bad %l" } */
  strftime (s, m, "%^s", tp); /* { dg-warning "flag" "bad %s" } */
  strftime (s, m, "%#s", tp); /* { dg-warning "flag" "bad %s" } */

  /* Bad usages with Y2K problems.  */
  strftime (s, m, "%^c", tp); /* { dg-warning "flag" "bad %c" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%#c", tp); /* { dg-warning "flag" "bad %c" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%^D", tp); /* { dg-warning "flag" "bad %D" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%#D", tp); /* { dg-warning "flag" "bad %D" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%^g", tp); /* { dg-warning "flag" "bad %g" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%#g", tp); /* { dg-warning "flag" "bad %g" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%^x", tp); /* { dg-warning "flag" "bad %x" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%#x", tp); /* { dg-warning "flag" "bad %x" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%^y", tp); /* { dg-warning "flag" "bad %y" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  strftime (s, m, "%#y", tp); /* { dg-warning "flag" "bad %y" } */
  /* { dg-warning "only last 2" "2-digit year" { target *-*-* } .-1 } */
  
  /* GCC also accepts the glibc format extensions %P, %k, %l, %s.  */
  strftime (s, m, "%P%k%l%s", tp);
  /* GCC also accepts the glibc extension of the "O" modifier on some
     more formats.  The cases where it is rejected altogether are
     covered in c99-strftime-1.c, except for the extension %P.
  */
  strftime (s, m, "%OC%Og%OG%Oj%OY%Oz%Ok%Ol%Os", tp); /* { dg-warning "only last 2" "2-digit year" } */
  strftime (s, m, "%OP", tp); /* { dg-warning "flag|modifier" "bad %OP" } */
  /* The "-", "_" and "0" flags are mutually exclusive.  */
  strftime (s, m, "%-_5C", tp); /* { dg-warning "flag" "bad %-_" } */
  strftime (s, m, "%-05C", tp); /* { dg-warning "flag" "bad %-0" } */
  strftime (s, m, "%_05C", tp); /* { dg-warning "flag" "bad %_0" } */
  /* The "#" and "^" flags are mutually exclusive.  */
  strftime (s, m, "%^#a", tp); /* { dg-warning "flag" "bad %^#" } */
}
