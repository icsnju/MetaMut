/* { dg-options "-Wformat -fdiagnostics-show-caret" } */

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

/* Various format tests, some containing type mismatches.  Verify that for
   the type mismatch cases that we offer "good" suggestions.  Specifically,
   any suggestions should preserve flags characters, field width and precision,
   and, if possible, the conversion specifier character, whilst giving a
   corrected length modifier appropriate to the argument type.  */

/* Tests of "x" without a length modifier, with various param types.
   Suggestions should preserve the "x" for integer arguments.  */

void
test_x (char *d,
	int iexpr, unsigned int uiexpr,
	long lexpr, unsigned long ulexpr,
	long long llexpr, unsigned long long ullexpr,
	float fexpr, double dexpr, long double ldexpr,
	void *ptr)
{
  /* Integer arguments.  */

  sprintf (d, " %-8x ", iexpr);
  sprintf (d, " %-8x ", uiexpr);

  sprintf (d, " %-8x ", lexpr); /* { dg-warning "20: format '%x' expects argument of type 'unsigned int', but argument 3 has type 'long int'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8x ", lexpr);
                 ~~~^    ~~~~~
                    |    |
                    |    long int
                    unsigned int
                 %-8lx
   { dg-end-multiline-output "" } */
  sprintf (d, " %-8x ", ulexpr); /* { dg-warning "20: format '%x' expects argument of type 'unsigned int', but argument 3 has type 'long unsigned int'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8x ", ulexpr);
                 ~~~^    ~~~~~~
                    |    |
                    |    long unsigned int
                    unsigned int
                 %-8lx
   { dg-end-multiline-output "" } */

  sprintf (d, " %-8x ", llexpr); /* { dg-warning "20: format '%x' expects argument of type 'unsigned int', but argument 3 has type 'long long int'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8x ", llexpr);
                 ~~~^    ~~~~~~
                    |    |
                    |    long long int
                    unsigned int
                 %-8llx
   { dg-end-multiline-output "" } */
  sprintf (d, " %-8x ", ullexpr); /* { dg-warning "20: format '%x' expects argument of type 'unsigned int', but argument 3 has type 'long long unsigned int'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8x ", ullexpr);
                 ~~~^    ~~~~~~~
                    |    |
                    |    long long unsigned int
                    unsigned int
                 %-8llx
   { dg-end-multiline-output "" } */

  /* Floating-point arguments.  */

  sprintf (d, " %-8x ", fexpr); /* { dg-warning "20: format '%x' expects argument of type 'unsigned int', but argument 3 has type 'double'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8x ", fexpr);
                 ~~~^    ~~~~~
                    |    |
                    |    double
                    unsigned int
                 %-8f
   { dg-end-multiline-output "" } */
  sprintf (d, " %-8x ", dexpr); /* { dg-warning "20: format '%x' expects argument of type 'unsigned int', but argument 3 has type 'double'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8x ", dexpr);
                 ~~~^    ~~~~~
                    |    |
                    |    double
                    unsigned int
                 %-8f
   { dg-end-multiline-output "" } */
  sprintf (d, " %-8x ", ldexpr); /* { dg-warning "20: format '%x' expects argument of type 'unsigned int', but argument 3 has type 'long double'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8x ", ldexpr);
                 ~~~^    ~~~~~~
                    |    |
                    |    long double
                    unsigned int
                 %-8Lf
   { dg-end-multiline-output "" } */

  /* Pointer.  */
  sprintf (d, " %-8x ", ptr); /* { dg-warning "20: format '%x' expects argument of type 'unsigned int', but argument 3 has type 'void \\*'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8x ", ptr);
                 ~~~^    ~~~
                    |    |
                    |    void *
                    unsigned int
                 %-8p
   { dg-end-multiline-output "" } */

  /* Something unrecognized.  */
  struct s { int i; };
  struct s s;
  sprintf (d, " %-8x ", s); /* { dg-warning "20: format '%x' expects argument of type 'unsigned int', but argument 3 has type 'struct s'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8x ", s);
                 ~~~^    ~
                    |    |
                    |    struct s
                    unsigned int
   { dg-end-multiline-output "" } */
}

/* Tests of "x" with "l", with various param types.
   Suggestions should preserve the "x" for integer arguments.  */

void
test_lx (char *d,
	 int iexpr, unsigned int uiexpr,
	 long lexpr, unsigned long ulexpr,
	 long long llexpr, unsigned long long ullexpr,
	 float fexpr, double dexpr, long double ldexpr)
{
  /* Integer arguments.  */

  sprintf (d, " %-8lx ", iexpr); /* { dg-warning "21: format '%lx' expects argument of type 'long unsigned int', but argument 3 has type 'int'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8lx ", iexpr);
                 ~~~~^    ~~~~~
                     |    |
                     |    int
                     long unsigned int
                 %-8x
   { dg-end-multiline-output "" } */
  sprintf (d, " %-8lx ", uiexpr); /* { dg-warning "21: format '%lx' expects argument of type 'long unsigned int', but argument 3 has type 'unsigned int'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8lx ", uiexpr);
                 ~~~~^    ~~~~~~
                     |    |
                     |    unsigned int
                     long unsigned int
                 %-8x
   { dg-end-multiline-output "" } */

  sprintf (d, " %-8lx ", lexpr);
  sprintf (d, " %-8lx ", ulexpr);

  sprintf (d, " %-8lx ", llexpr); /* { dg-warning "21: format '%lx' expects argument of type 'long unsigned int', but argument 3 has type 'long long int'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8lx ", llexpr);
                 ~~~~^    ~~~~~~
                     |    |
                     |    long long int
                     long unsigned int
                 %-8llx
   { dg-end-multiline-output "" } */
  sprintf (d, " %-8lx ", ullexpr); /* { dg-warning "21: format '%lx' expects argument of type 'long unsigned int', but argument 3 has type 'long long unsigned int'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8lx ", ullexpr);
                 ~~~~^    ~~~~~~~
                     |    |
                     |    long long unsigned int
                     long unsigned int
                 %-8llx
   { dg-end-multiline-output "" } */

  /* Floating-point arguments.  */

  sprintf (d, " %-8lx ", fexpr); /* { dg-warning "21: format '%lx' expects argument of type 'long unsigned int', but argument 3 has type 'double'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8lx ", fexpr);
                 ~~~~^    ~~~~~
                     |    |
                     |    double
                     long unsigned int
                 %-8f
   { dg-end-multiline-output "" } */
  sprintf (d, " %-8lx ", dexpr); /* { dg-warning "21: format '%lx' expects argument of type 'long unsigned int', but argument 3 has type 'double'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8lx ", dexpr);
                 ~~~~^    ~~~~~
                     |    |
                     |    double
                     long unsigned int
                 %-8f
   { dg-end-multiline-output "" } */
  sprintf (d, " %-8lx ", ldexpr); /* { dg-warning "21: format '%lx' expects argument of type 'long unsigned int', but argument 3 has type 'long double'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8lx ", ldexpr);
                 ~~~~^    ~~~~~~
                     |    |
                     |    long double
                     long unsigned int
                 %-8Lf
   { dg-end-multiline-output "" } */
}

/* Tests of "o" without a length modifier, with various param types.
   Suggestions should preserve the "o" for integer arguments.  */

void
test_o (char *d,
	int iexpr, unsigned int uiexpr,
	long lexpr, unsigned long ulexpr,
	long long llexpr, unsigned long long ullexpr)
{
  /* Integer arguments.  */

  sprintf (d, " %-8o ", iexpr);
  sprintf (d, " %-8o ", uiexpr);

  sprintf (d, " %-8o ", lexpr); /* { dg-warning "20: format '%o' expects argument of type 'unsigned int', but argument 3 has type 'long int'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8o ", lexpr);
                 ~~~^    ~~~~~
                    |    |
                    |    long int
                    unsigned int
                 %-8lo
   { dg-end-multiline-output "" } */
  sprintf (d, " %-8o ", ulexpr); /* { dg-warning "20: format '%o' expects argument of type 'unsigned int', but argument 3 has type 'long unsigned int'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8o ", ulexpr);
                 ~~~^    ~~~~~~
                    |    |
                    |    long unsigned int
                    unsigned int
                 %-8lo
   { dg-end-multiline-output "" } */

  sprintf (d, " %-8o ", llexpr); /* { dg-warning "20: format '%o' expects argument of type 'unsigned int', but argument 3 has type 'long long int'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8o ", llexpr);
                 ~~~^    ~~~~~~
                    |    |
                    |    long long int
                    unsigned int
                 %-8llo
   { dg-end-multiline-output "" } */
  sprintf (d, " %-8o ", ullexpr); /* { dg-warning "20: format '%o' expects argument of type 'unsigned int', but argument 3 has type 'long long unsigned int'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8o ", ullexpr);
                 ~~~^    ~~~~~~~
                    |    |
                    |    long long unsigned int
                    unsigned int
                 %-8llo
   { dg-end-multiline-output "" } */
}

/* Tests of "o" with "l", with various param types.
   Suggestions should preserve the "o" for integer arguments.  */

void
test_lo (char *d,
	int iexpr, unsigned int uiexpr,
	long lexpr, unsigned long ulexpr,
	long long llexpr, unsigned long long ullexpr)
{
  /* Integer arguments.  */

  sprintf (d, " %-8lo ", iexpr); /* { dg-warning "21: format '%lo' expects argument of type 'long unsigned int', but argument 3 has type 'int'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8lo ", iexpr);
                 ~~~~^    ~~~~~
                     |    |
                     |    int
                     long unsigned int
                 %-8o
   { dg-end-multiline-output "" } */
  sprintf (d, " %-8lo ", uiexpr); /* { dg-warning "21: format '%lo' expects argument of type 'long unsigned int', but argument 3 has type 'unsigned int'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8lo ", uiexpr);
                 ~~~~^    ~~~~~~
                     |    |
                     |    unsigned int
                     long unsigned int
                 %-8o
   { dg-end-multiline-output "" } */

  sprintf (d, " %-8lo ", lexpr);
  sprintf (d, " %-8lo ", ulexpr);

  sprintf (d, " %-8lo ", llexpr); /* { dg-warning "21: format '%lo' expects argument of type 'long unsigned int', but argument 3 has type 'long long int'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8lo ", llexpr);
                 ~~~~^    ~~~~~~
                     |    |
                     |    long long int
                     long unsigned int
                 %-8llo
   { dg-end-multiline-output "" } */
  sprintf (d, " %-8lo ", ullexpr); /* { dg-warning "21: format '%lo' expects argument of type 'long unsigned int', but argument 3 has type 'long long unsigned int'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8lo ", ullexpr);
                 ~~~~^    ~~~~~~~
                     |    |
                     |    long long unsigned int
                     long unsigned int
                 %-8llo
   { dg-end-multiline-output "" } */
}

/* Tests of "e" without a length modifier, with various param types.
   Suggestions should preserve the "e" for float  arguments.  */

void
test_e (char *d, int iexpr, float fexpr, double dexpr, long double ldexpr)
{
  /* Integer arguments.  */

  sprintf (d, " %-8e ", iexpr); /* { dg-warning "20: format '%e' expects argument of type 'double', but argument 3 has type 'int'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8e ", iexpr);
                 ~~~^    ~~~~~
                    |    |
                    |    int
                    double
                 %-8d
   { dg-end-multiline-output "" } */

  /* Floating-point arguments.  */

  sprintf (d, " %-8e ", fexpr);
  sprintf (d, " %-8e ", dexpr);
  sprintf (d, " %-8e ", ldexpr); /* { dg-warning "20: format '%e' expects argument of type 'double', but argument 3 has type 'long double'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8e ", ldexpr);
                 ~~~^    ~~~~~~
                    |    |
                    |    long double
                    double
                 %-8Le
   { dg-end-multiline-output "" } */
}

/* Tests of "e" with "L", with various param types.
   Suggestions should preserve the "e" for float  arguments.  */

void
test_Le (char *d, int iexpr, float fexpr, double dexpr, long double ldexpr)
{
  /* Integer arguments.  */

  sprintf (d, " %-8Le ", iexpr); /* { dg-warning "21: format '%Le' expects argument of type 'long double', but argument 3 has type 'int'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8Le ", iexpr);
                 ~~~~^    ~~~~~
                     |    |
                     |    int
                     long double
                 %-8d
   { dg-end-multiline-output "" } */

  /* Floating-point arguments.  */

  sprintf (d, " %-8Le ", fexpr); /* { dg-warning "21: format '%Le' expects argument of type 'long double', but argument 3 has type 'double'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8Le ", fexpr);
                 ~~~~^    ~~~~~
                     |    |
                     |    double
                     long double
                 %-8e
   { dg-end-multiline-output "" } */

  sprintf (d, " %-8Le ", dexpr); /* { dg-warning "21: format '%Le' expects argument of type 'long double', but argument 3 has type 'double'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8Le ", dexpr);
                 ~~~~^    ~~~~~
                     |    |
                     |    double
                     long double
                 %-8e
   { dg-end-multiline-output "" } */

  sprintf (d, " %-8Le ", ldexpr);
}

/* Tests of "E" without a length modifier, with various param types.
   Suggestions should preserve the "E" for floating-point arguments.  */

void
test_E (char *d, int iexpr, float fexpr, double dexpr, long double ldexpr)
{
  /* Integer arguments.  */

  sprintf (d, " %-8E ", iexpr); /* { dg-warning "20: format '%E' expects argument of type 'double', but argument 3 has type 'int'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8E ", iexpr);
                 ~~~^    ~~~~~
                    |    |
                    |    int
                    double
                 %-8d
   { dg-end-multiline-output "" } */

  /* Floating-point arguments.  */

  sprintf (d, " %-8E ", fexpr);
  sprintf (d, " %-8E ", dexpr);
  sprintf (d, " %-8E ", ldexpr); /* { dg-warning "20: format '%E' expects argument of type 'double', but argument 3 has type 'long double'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8E ", ldexpr);
                 ~~~^    ~~~~~~
                    |    |
                    |    long double
                    double
                 %-8LE
   { dg-end-multiline-output "" } */
}

/* Tests of "E" with "L", with various param types.
   Suggestions should preserve the "E" for floating-point arguments.  */

void
test_LE (char *d, int iexpr, float fexpr, double dexpr, long double ldexpr)
{
  /* Integer arguments.  */

  sprintf (d, " %-8LE ", iexpr); /* { dg-warning "21: format '%LE' expects argument of type 'long double', but argument 3 has type 'int'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8LE ", iexpr);
                 ~~~~^    ~~~~~
                     |    |
                     |    int
                     long double
                 %-8d
   { dg-end-multiline-output "" } */

  sprintf (d, " %-8LE ", fexpr); /* { dg-warning "21: format '%LE' expects argument of type 'long double', but argument 3 has type 'double'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8LE ", fexpr);
                 ~~~~^    ~~~~~
                     |    |
                     |    double
                     long double
                 %-8E
   { dg-end-multiline-output "" } */

  sprintf (d, " %-8LE ", dexpr); /* { dg-warning "21: format '%LE' expects argument of type 'long double', but argument 3 has type 'double'" } */
/* { dg-begin-multiline-output "" }
   sprintf (d, " %-8LE ", dexpr);
                 ~~~~^    ~~~~~
                     |    |
                     |    double
                     long double
                 %-8E
   { dg-end-multiline-output "" } */

  sprintf (d, " %-8LE ", ldexpr);
}

/* Test of a suggestion for a conversion specification containing
   all features (flags, width, precision, length modifier), where
   all the other arguments have mismatching types.  */

void
test_everything (char *d, long lexpr)
{
  sprintf (d, "before %-+*.*lld after", lexpr, lexpr, lexpr); /* { dg-line test_everything_sprintf } */

  /* { dg-warning "26: field width specifier '\\*' expects argument of type 'int', but argument 3 has type 'long int'" "" { target *-*-* } test_everything_sprintf } */
  /* { dg-begin-multiline-output "" }
   sprintf (d, "before %-+*.*lld after", lexpr, lexpr, lexpr);
                       ~~~^~~~~~         ~~~~~
                          |              |
                          int            long int
   { dg-end-multiline-output "" } */

  /* { dg-warning "28: field precision specifier '\\.\\*' expects argument of type 'int', but argument 4 has type 'long int'" "" { target *-*-* } test_everything_sprintf } */
  /* { dg-begin-multiline-output "" }
   sprintf (d, "before %-+*.*lld after", lexpr, lexpr, lexpr);
                       ~~~~~^~~~                ~~~~~
                            |                   |
                            int                 long int
   { dg-end-multiline-output "" } */

  /* { dg-warning "31: format '%lld' expects argument of type 'long long int', but argument 5 has type 'long int'" "" { target *-*-* } test_everything_sprintf } */
  /* { dg-begin-multiline-output "" }
   sprintf (d, "before %-+*.*lld after", lexpr, lexpr, lexpr);
                       ~~~~~~~~^                       ~~~~~
                               |                       |
                               long long int           long int
                       %-+*.*ld
   { dg-end-multiline-output "" } */
}
