/* Test for GCC diagnostic formats.  */
/* Origin: Kaveh Ghazi <ghazi@caip.rutgers.edu> */
/* { dg-do compile } */
/* { dg-options "-Wformat -Wno-format-diag" } */

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

#define ATTRIBUTE_DIAG(F) __attribute__ ((__format__ (F, 1, 2))) __attribute__ ((__nonnull__));

/* Magic identifiers must be set before the attribute is used.  */

typedef long long __gcc_host_wide_int__;

typedef struct location_s
{
  const char *file;
  int line;
} location_t;

union tree_node;
typedef union tree_node *tree;

extern int diag (const char *, ...) ATTRIBUTE_DIAG(__gcc_diag__);
extern int tdiag (const char *, ...) ATTRIBUTE_DIAG(__gcc_tdiag__);
extern int cdiag (const char *, ...) ATTRIBUTE_DIAG(__gcc_cdiag__);
extern int cxxdiag (const char *, ...) ATTRIBUTE_DIAG(__gcc_cxxdiag__);
extern int dump (const char *, ...) ATTRIBUTE_DIAG(__gcc_dump_printf__);

void
foo (int i, int i1, int i2, unsigned int u, double d, char *s, void *p,
     int *n, short int *hn, long int l, unsigned long int ul,
     long int *ln, long double ld, wint_t lc, wchar_t *ls, llong ll,
     ullong ull, unsigned int *un, const int *cn, signed char *ss,
     unsigned char *us, const signed char *css, unsigned int u1,
     unsigned int u2, location_t *loc, tree t1, union tree_node *t2,
     tree *t3, tree t4[], int *v, unsigned v_len)
{
  /* Acceptable C90 specifiers, flags and modifiers.  */
  diag ("%%");
  tdiag ("%%");
  cdiag ("%%");
  cxxdiag ("%%");
  dump ("%%");
  diag ("%d%i%o%u%x%c%s%p%%", i, i, u, u, u, i, s, p);
  tdiag ("%d%i%o%u%x%c%s%p%%", i, i, u, u, u, i, s, p);
  cdiag ("%d%i%o%u%x%c%s%p%%", i, i, u, u, u, i, s, p);
  cxxdiag ("%d%i%o%u%x%c%s%p%%", i, i, u, u, u, i, s, p);
  dump ("%d%i%o%u%x%c%s%p%%", i, i, u, u, u, i, s, p);
  diag ("%qd%qi%qo%qu%qx%qc%qs%qp%<%%%'%>", i, i, u, u, u, i, s, p);
  tdiag ("%qd%qi%qo%qu%qx%qc%qs%qp%<%%%'%>", i, i, u, u, u, i, s, p);
  cdiag ("%qd%qi%qo%qu%qx%qc%qs%qp%<%%%'%>", i, i, u, u, u, i, s, p);
  cxxdiag ("%qd%qi%qo%qu%qx%qc%qs%qp%<%%%'%>", i, i, u, u, u, i, s, p);
  dump ("%qd%qi%qo%qu%qx%qc%qs%qp%<%%%'%>", i, i, u, u, u, i, s, p);
  diag ("%ld%li%lo%lu%lx", l, l, ul, ul, ul);
  tdiag ("%ld%li%lo%lu%lx", l, l, ul, ul, ul);
  cdiag ("%ld%li%lo%lu%lx", l, l, ul, ul, ul);
  cxxdiag ("%ld%li%lo%lu%lx", l, l, ul, ul, ul);
  dump ("%ld%li%lo%lu%lx", l, l, ul, ul, ul);
  diag ("%lld%lli%llo%llu%llx", ll, ll, ull, ull, ull);
  tdiag ("%lld%lli%llo%llu%llx", ll, ll, ull, ull, ull);
  cdiag ("%lld%lli%llo%llu%llx", ll, ll, ull, ull, ull);
  cxxdiag ("%lld%lli%llo%llu%llx", ll, ll, ull, ull, ull);
  dump ("%lld%lli%llo%llu%llx", ll, ll, ull, ull, ull);
  diag ("%wd%wi%wo%wu%wx", ll, ll, ull, ull, ull);
  tdiag ("%wd%wi%wo%wu%wx", ll, ll, ull, ull, ull);
  cdiag ("%wd%wi%wo%wu%wx", ll, ll, ull, ull, ull);
  cxxdiag ("%wd%wi%wo%wu%wx", ll, ll, ull, ull, ull);
  dump ("%wd%wi%wo%wu%wx", ll, ll, ull, ull, ull);
  diag ("%.*s", i, s);
  tdiag ("%.*s", i, s);
  cdiag ("%.*s", i, s);
  cxxdiag ("%.*s", i, s);
  dump ("%.*s", i, s);

  /* Extensions provided in the diagnostic framework.  */
  diag ("%m");
  tdiag ("%m");
  cdiag ("%m");
  cxxdiag ("%m");
  dump ("%m");

  /* Quote directives to avoid "warning: conversion used unquoted." */
  tdiag ("%<%D%F%T%V%>", t1, t1, t1, t1);
  tdiag ("%<%+D%+F%+T%+V%>", t1, t1, t1, t1);
  tdiag ("%q+D%q+F%q+T%q+V", t1, t1, t1, t1);
  tdiag ("%<%D%D%D%D%>", t1, t2, *t3, t4[5]);
  cdiag ("%<%D%F%T%V%>", t1, t1, t1, t1);
  cdiag ("%<%+D%+F%+T%+V%>", t1, t1, t1, t1);
  cdiag ("%q+D%q+F%q+T%q+V", t1, t1, t1, t1);
  cdiag ("%<%D%D%D%D%>", t1, t2, *t3, t4[5]);
  cdiag ("%<%E%>", t1);
  cxxdiag ("%<%A%D%E%F%T%V%>", t1, t1, t1, t1, t1, t1);
  cxxdiag ("%<%D%D%D%D%>", t1, t2, *t3, t4[5]);
  cxxdiag ("%<%#A%#D%#E%#F%#T%#V%>", t1, t1, t1, t1, t1, t1);
  cxxdiag ("%<%+A%+D%+E%+F%+T%+V%>", t1, t1, t1, t1, t1, t1);
  cxxdiag ("%<%+#A%+#D%+#E%+#F%+#T%+#V%>", t1, t1, t1, t1, t1, t1);
  cxxdiag ("%C%L%O%P%Q", i, i, i, i, i);

  tdiag ("%v", i); /* { dg-warning "format" } */
  cdiag ("%v%qv%#v", i, i, i);
  cxxdiag ("%v", i); /* { dg-warning "format" } */

  tdiag ("%Z", v, v_len);
  cdiag ("%Z", v, v_len);
  cxxdiag ("%Z", v, v_len);
  dump ("%Z", v, v_len);

  /* Bad stuff with extensions.  */
  diag ("%m", i); /* { dg-warning "format" "extra arg" } */
  tdiag ("%m", i); /* { dg-warning "format" "extra arg" } */
  cdiag ("%m", i); /* { dg-warning "format" "extra arg" } */
  cxxdiag ("%m", i); /* { dg-warning "format" "extra arg" } */
  dump ("%m", i); /* { dg-warning "format" "extra arg" } */
  diag ("%#m"); /* { dg-warning "format" "bogus modifier" } */
  tdiag ("%#m"); /* { dg-warning "format" "bogus modifier" } */
  cdiag ("%#m"); /* { dg-warning "format" "bogus modifier" } */
  cxxdiag ("%#m"); /* { dg-warning "format" "bogus modifier" } */
  dump ("%#m"); /* { dg-warning "format" "bogus modifier" } */
  diag ("%+m"); /* { dg-warning "format" "bogus modifier" } */
  tdiag ("%+m"); /* { dg-warning "format" "bogus modifier" } */
  cdiag ("%+m"); /* { dg-warning "format" "bogus modifier" } */
  cxxdiag ("%+m"); /* { dg-warning "format" "bogus modifier" } */
  dump ("%+m"); /* { dg-warning "format" "bogus modifier" } */
  diag ("%D", t1); /* { dg-warning "format" "bogus tree" } */
  tdiag ("%A", t1); /* { dg-warning "format" "bogus tree" } */
  tdiag ("%E", t1);
  tdiag ("%#D", t1); /* { dg-warning "format" "bogus modifier" } */
  cdiag ("%A", t1); /* { dg-warning "format" "bogus tree" } */
  cdiag ("%#D", t1); /* { dg-warning "format" "bogus modifier" } */
  cdiag ("%<%+D%>", t1);
  cxxdiag ("%C"); /* { dg-warning "format" "missing arg" } */
  cxxdiag ("%C", l); /* { dg-warning "format" "wrong arg" } */
  cxxdiag ("%C", i, i); /* { dg-warning "format" "extra arg" } */
  cxxdiag ("%#C", i); /* { dg-warning "format" "bogus modifier" } */
  cxxdiag ("%+C", i); /* { dg-warning "format" "bogus modifier" } */
  tdiag ("%D"); /* { dg-warning "format" "missing arg" } */
  cdiag ("%D"); /* { dg-warning "format" "missing arg" } */
  cxxdiag ("%D"); /* { dg-warning "format" "missing arg" } */
  tdiag ("%D", i); /* { dg-warning "format" "wrong arg" } */
  cdiag ("%D", i); /* { dg-warning "format" "wrong arg" } */
  cxxdiag ("%D", i); /* { dg-warning "format" "wrong arg" } */
  tdiag ("%D", t1, t1); /* { dg-warning "format" "extra arg" } */
  cdiag ("%D", t1, t1); /* { dg-warning "format" "extra arg" } */
  cxxdiag ("%D", t1, t1); /* { dg-warning "format" "extra arg" } */

  tdiag ("%V", i); /* { dg-warning "format" "wrong arg" } */
  cdiag ("%V", i); /* { dg-warning "format" "wrong arg" } */
  cxxdiag ("%V", i); /* { dg-warning "format" "wrong arg" } */

  tdiag ("%v", t1); /* { dg-warning "format" "wrong arg" } */
  cdiag ("%v", t1); /* { dg-warning "format" "wrong arg" } */
  cxxdiag ("%v", t1); /* { dg-warning "format" "wrong arg" } */

  tdiag ("%Z"); /* { dg-warning "format" "missing arg" } */
  tdiag ("%Z", t1); /* { dg-warning "format" "wrong arg" } */

  /* Standard specifiers not accepted in the diagnostic framework.  */
  diag ("%X\n", u); /* { dg-warning "format" "HEX" } */
  diag ("%f\n", d); /* { dg-warning "format" "float" } */
  diag ("%e\n", d); /* { dg-warning "format" "float" } */
  diag ("%E\n", d); /* { dg-warning "format" "float" } */
  diag ("%g\n", d); /* { dg-warning "format" "float" } */
  diag ("%G\n", d); /* { dg-warning "format" "float" } */
  diag ("%n\n", n); /* { dg-warning "format" "counter" } */
  diag ("%hd\n", i); /* { dg-warning "format" "conversion" } */

  /* Various tests of bad argument types.  */
  diag ("%-d", i); /* { dg-warning "format" "bad flag" } */
  tdiag ("%-d", i); /* { dg-warning "format" "bad flag" } */
  cdiag ("%-d", i); /* { dg-warning "format" "bad flag" } */
  cxxdiag ("%-d", i); /* { dg-warning "format" "bad flag" } */
  diag ("% d", i); /* { dg-warning "format" "bad flag" } */
  tdiag ("% d", i); /* { dg-warning "format" "bad flag" } */
  cdiag ("% d", i); /* { dg-warning "format" "bad flag" } */
  cxxdiag ("% d", i); /* { dg-warning "format" "bad flag" } */
  diag ("%#o", u); /* { dg-warning "format" "bad flag" } */
  tdiag ("%#o", u); /* { dg-warning "format" "bad flag" } */
  cdiag ("%#o", u); /* { dg-warning "format" "bad flag" } */
  cxxdiag ("%#o", u); /* { dg-warning "format" "bad flag" } */
  diag ("%0d", i); /* { dg-warning "format" "bad flag" } */
  tdiag ("%0d", i); /* { dg-warning "format" "bad flag" } */
  cdiag ("%0d", i); /* { dg-warning "format" "bad flag" } */
  cxxdiag ("%0d", i); /* { dg-warning "format" "bad flag" } */
  diag ("%08d", i); /* { dg-warning "format" "bad flag" } */
  tdiag ("%08d", i); /* { dg-warning "format" "bad flag" } */
  cdiag ("%08d", i); /* { dg-warning "format" "bad flag" } */
  cxxdiag ("%08d", i); /* { dg-warning "format" "bad flag" } */
  diag ("%+d\n", i); /* { dg-warning "format" "bad flag" } */
  tdiag ("%+d\n", i); /* { dg-warning "format" "bad flag" } */
  cdiag ("%+d\n", i); /* { dg-warning "format" "bad flag" } */
  cxxdiag ("%+d\n", i); /* { dg-warning "format" "bad flag" } */
  diag ("%3d\n", i); /* { dg-warning "format" "bad flag" } */
  tdiag ("%3d\n", i); /* { dg-warning "format" "bad flag" } */
  cdiag ("%3d\n", i); /* { dg-warning "format" "bad flag" } */
  cxxdiag ("%3d\n", i); /* { dg-warning "format" "bad flag" } */
  diag ("%-3d\n", i); /* { dg-warning "format" "bad flag" } */
  tdiag ("%-3d\n", i); /* { dg-warning "format" "bad flag" } */
  cdiag ("%-3d\n", i); /* { dg-warning "format" "bad flag" } */
  cxxdiag ("%-3d\n", i); /* { dg-warning "format" "bad flag" } */
  diag ("%.7d\n", i); /* { dg-warning "format" "bad flag" } */
  tdiag ("%.7d\n", i); /* { dg-warning "format" "bad flag" } */
  cdiag ("%.7d\n", i); /* { dg-warning "format" "bad flag" } */
  cxxdiag ("%.7d\n", i); /* { dg-warning "format" "bad flag" } */
  diag ("%+9.4d\n", i); /* { dg-warning "format" "bad flag" } */
  tdiag ("%+9.4d\n", i); /* { dg-warning "format" "bad flag" } */
  cdiag ("%+9.4d\n", i); /* { dg-warning "format" "bad flag" } */
  cxxdiag ("%+9.4d\n", i); /* { dg-warning "format" "bad flag" } */
  diag ("%.3ld\n", l); /* { dg-warning "format" "bad flag" } */
  tdiag ("%.3ld\n", l); /* { dg-warning "format" "bad flag" } */
  cdiag ("%.3ld\n", l); /* { dg-warning "format" "bad flag" } */
  cxxdiag ("%.3ld\n", l); /* { dg-warning "format" "bad flag" } */
  diag ("%d %lu\n", i, ul);
  diag ("%d", l); /* { dg-warning "format" "bad argument types" } */
  diag ("%wd", l); /* { dg-warning "format" "bad argument types" } */
  diag ("%d", ll); /* { dg-warning "format" "bad argument types" } */
  diag ("%*s", i, s); /* { dg-warning "format" "bad * argument types" } */
  diag ("%*.*s", i, i, s); /* { dg-warning "format" "bad * argument types" } */
  diag ("%*d\n", i1, i); /* { dg-warning "format" "bad * argument types" } */
  diag ("%.*d\n", i2, i); /* { dg-warning "format" "bad * argument types" } */
  diag ("%*.*ld\n", i1, i2, l); /* { dg-warning "format" "bad * argument types" } */
  diag ("%ld", i); /* { dg-warning "format" "bad argument types" } */
  diag ("%s", n); /* { dg-warning "format" "bad argument types" } */

  /* Wrong number of arguments.  */
  diag ("%d%d", i); /* { dg-warning "matching" "wrong number of args" } */
  diag ("%d", i, i); /* { dg-warning "arguments" "wrong number of args" } */
  /* Miscellaneous bogus constructions.  */
  diag (""); /* { dg-warning "zero-length" "warning for empty format" } */
  diag ("\0"); /* { dg-warning "embedded" "warning for embedded NUL" } */
  diag ("%d\0", i); /* { dg-warning "embedded" "warning for embedded NUL" } */
  diag ("%d\0%d", i, i); /* { dg-warning "embedded|too many" "warning for embedded NUL" } */
  diag (NULL); /* { dg-warning "null" "null format string warning" } */
  diag ("%"); /* { dg-warning "trailing" "trailing % warning" } */
  diag ((const char *)L"foo"); /* { dg-warning "wide" "wide string" } */
  diag ("%s", (char *)0); /* { dg-warning "null" "%s with NULL" } */

  /* Make sure we still get warnings for regular printf.  */
  printf ("%d\n", ll); /* { dg-warning "format" "bad argument types" } */
}
