/* { dg-options "-Wformat -fdiagnostics-show-caret" } */

/* See PR 52952. */

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

void test_mismatching_types (const char *msg)
{
  printf("hello %i", msg);  /* { dg-warning "format '%i' expects argument of type 'int', but argument 2 has type 'const char \\*' " } */

/* { dg-begin-multiline-output "" }
   printf("hello %i", msg);
                 ~^   ~~~
                  |   |
                  int const char *
                 %s
   { dg-end-multiline-output "" } */


  printf("hello %s", 42);  /* { dg-warning "format '%s' expects argument of type 'char \\*', but argument 2 has type 'int'" } */
/* { dg-begin-multiline-output "" }
   printf("hello %s", 42);
                 ~^   ~~
                  |   |
                  |   int
                  char *
                 %d
   { dg-end-multiline-output "" } */

  printf("hello %i", (long)0);  /* { dg-warning "format '%i' expects argument of type 'int', but argument 2 has type 'long int' " } */
/* { dg-begin-multiline-output "" }
   printf("hello %i", (long)0);
                 ~^   ~~~~~~~
                  |   |
                  int long int
                 %li
   { dg-end-multiline-output "" } */
}

void test_multiple_arguments (void)
{
  printf ("arg0: %i  arg1: %s arg 2: %i", /* { dg-warning "29: format '%s'" } */
          100, 101, 102);
/* { dg-begin-multiline-output "" }
   printf ("arg0: %i  arg1: %s arg 2: %i",
                            ~^
                             |
                             char *
                            %d
           100, 101, 102);
                ~~~           
                |
                int
   { dg-end-multiline-output "" } */
}

void test_multiple_arguments_2 (int i, int j)
{
  printf ("arg0: %i  arg1: %s arg 2: %i", /* { dg-warning "29: format '%s'" } */
          100, i + j, 102);
/* { dg-begin-multiline-output "" }
   printf ("arg0: %i  arg1: %s arg 2: %i",
                            ~^
                             |
                             char *
                            %d
           100, i + j, 102);
                ~~~~~         
                  |
                  int
   { dg-end-multiline-output "" } */
}

void multiline_format_string (void) {
  printf ("before the fmt specifier" /* { dg-warning "11: format '%d' expects a matching 'int' argument" } */
/* { dg-begin-multiline-output "" }
   printf ("before the fmt specifier"
           ^~~~~~~~~~~~~~~~~~~~~~~~~~
   { dg-end-multiline-output "" } */

          "%"
          "d" /* { dg-message "12: format string is defined here" } */
          "after the fmt specifier");

/* { dg-begin-multiline-output "" }
           "%"
            ~~
           "d"
           ~^
            |
            int
   { dg-end-multiline-output "" } */
}

void test_hex (const char *msg)
{
  /* "%" is \x25
     "i" is \x69 */
  printf("hello \x25\x69", msg);  /* { dg-warning "format '%i' expects argument of type 'int', but argument 2 has type 'const char \\*' " } */

/* { dg-begin-multiline-output "" }
   printf("hello \x25\x69", msg);
                 ~~~~^~~~   ~~~
                     |      |
                     int    const char *
                 \x25s
   { dg-end-multiline-output "" } */
}

void test_oct (const char *msg)
{
  /* "%" is octal 045
     "i" is octal 151.  */
  printf("hello \045\151", msg);  /* { dg-warning "format '%i' expects argument of type 'int', but argument 2 has type 'const char \\*' " } */

/* { dg-begin-multiline-output "" }
   printf("hello \045\151", msg);
                 ~~~~^~~~   ~~~
                     |      |
                     int    const char *
                 \045s
   { dg-end-multiline-output "" } */
}

void test_multiple (const char *msg)
{
  /* "%" is \x25 in hex
     "i" is \151 in octal.  */
  printf("prefix"  "\x25"  "\151"  "suffix",  /* { dg-warning "format '%i'" } */
         msg);
/* { dg-begin-multiline-output "" }
   printf("prefix"  "\x25"  "\151"  "suffix",
          ^~~~~~~~
          msg);
          ~~~
          |
          const char *
  { dg-end-multiline-output "" } */

/* { dg-begin-multiline-output "" }
   printf("prefix"  "\x25"  "\151"  "suffix",
                     ~~~~~~~~^~~~
                             |
                             int
                     \x25"  "s
  { dg-end-multiline-output "" } */
}

void test_u8 (const char *msg)
{
  printf(u8"hello %i", msg);/* { dg-warning "format '%i' expects argument of type 'int', but argument 2 has type 'const char \\*' " } */
/* { dg-begin-multiline-output "" }
   printf(u8"hello %i", msg);
                   ~^   ~~~
                    |   |
                    int const char *
                   %s
   { dg-end-multiline-output "" } */
}

void test_param (long long_i, long long_j)
{
  printf ("foo %s bar", long_i + long_j); /* { dg-warning "17: format '%s' expects argument of type 'char \\*', but argument 2 has type 'long int'" } */
/* { dg-begin-multiline-output "" }
   printf ("foo %s bar", long_i + long_j);
                ~^       ~~~~~~~~~~~~~~~
                 |              |
                 char *         long int
                %ld
   { dg-end-multiline-output "" } */
}

void test_field_width_specifier (long l, int i1, int i2)
{
  printf (" %*.*d ", l, i1, i2); /* { dg-warning "14: field width specifier '\\*' expects argument of type 'int', but argument 2 has type 'long int'" } */
/* { dg-begin-multiline-output "" }
   printf (" %*.*d ", l, i1, i2);
             ~^~~~    ~
              |       |
              int     long int
   { dg-end-multiline-output "" } */
}

/* PR c/72857.  */

void test_field_width_specifier_2 (char *d, long foo, long bar)
{
  __builtin_sprintf (d, " %*ld ", foo, foo); /* { dg-warning "28: field width specifier '\\*' expects argument of type 'int', but argument 3 has type 'long int'" } */
  /* { dg-begin-multiline-output "" }
   __builtin_sprintf (d, " %*ld ", foo, foo);
                           ~^~~    ~~~
                            |      |
                            int    long int
   { dg-end-multiline-output "" } */

  __builtin_sprintf (d, " %*ld ", foo + bar, foo); /* { dg-warning "28: field width specifier '\\*' expects argument of type 'int', but argument 3 has type 'long int'" } */
  /* { dg-begin-multiline-output "" }
   __builtin_sprintf (d, " %*ld ", foo + bar, foo);
                           ~^~~    ~~~~~~~~~
                            |          |
                            int        long int
   { dg-end-multiline-output "" } */
}

void test_field_precision_specifier (char *d, long foo, long bar)
{
  __builtin_sprintf (d, " %.*ld ", foo, foo); /* { dg-warning "29: field precision specifier '\\.\\*' expects argument of type 'int', but argument 3 has type 'long int'" } */
  /* { dg-begin-multiline-output "" }
   __builtin_sprintf (d, " %.*ld ", foo, foo);
                           ~~^~~    ~~~
                             |      |
                             int    long int
   { dg-end-multiline-output "" } */

  __builtin_sprintf (d, " %.*ld ", foo + bar, foo); /* { dg-warning "29: field precision specifier '\\.\\*' expects argument of type 'int', but argument 3 has type 'long int'" } */
  /* { dg-begin-multiline-output "" }
   __builtin_sprintf (d, " %.*ld ", foo + bar, foo);
                           ~~^~~    ~~~~~~~~~
                             |          |
                             int        long int
   { dg-end-multiline-output "" } */
}

void test_spurious_percent (void)
{
  printf("hello world %"); /* { dg-warning "23: spurious trailing" } */

/* { dg-begin-multiline-output "" }
   printf("hello world %");
                       ^
   { dg-end-multiline-output "" } */
}

void test_empty_precision (char *s, size_t m, double d)
{
  strfmon (s, m, "%#.5n", d); /* { dg-warning "20: empty left precision in gnu_strfmon format" } */
/* { dg-begin-multiline-output "" }
   strfmon (s, m, "%#.5n", d);
                    ^
   { dg-end-multiline-output "" } */

  strfmon (s, m, "%#5.n", d); /* { dg-warning "22: empty precision in gnu_strfmon format" } */
/* { dg-begin-multiline-output "" }
   strfmon (s, m, "%#5.n", d);
                      ^
   { dg-end-multiline-output "" } */
}

void test_repeated (int i)
{
  printf ("%++d", i); /* { dg-warning "14: repeated '\\+' flag in format" } */
/* { dg-begin-multiline-output "" }
   printf ("%++d", i);
              ^
   { dg-end-multiline-output "" } */
}

void test_conversion_lacks_type (void)
{
  printf (" %h"); /* { dg-warning "14:conversion lacks type at end of format" } */
/* { dg-begin-multiline-output "" }
   printf (" %h");
              ^
   { dg-end-multiline-output "" } */
}

void test_embedded_nul (void)
{
  printf (" \0 "); /* { dg-warning "13:embedded" "warning for embedded NUL" } */
/* { dg-begin-multiline-output "" }
   printf (" \0 ");
             ^~
   { dg-end-multiline-output "" } */
}

void test_macro (const char *msg)
{
#define INT_FMT "%i" /* { dg-message "19: format string is defined here" } */
  printf("hello " INT_FMT " world", msg);  /* { dg-warning "10: format '%i' expects argument of type 'int', but argument 2 has type 'const char \\*' " } */
/* { dg-begin-multiline-output "" }
   printf("hello " INT_FMT " world", msg);
          ^~~~~~~~                   ~~~
                                     |
                                     const char *
   { dg-end-multiline-output "" } */
/* { dg-begin-multiline-output "" }
 #define INT_FMT "%i"
                  ~^
                   |
                   int
                  %s
   { dg-end-multiline-output "" } */
#undef INT_FMT
}

void test_macro_2 (const char *msg)
{
#define PRIu32 "u" /* { dg-message "17: format string is defined here" } */
  printf("hello %" PRIu32 " world", msg);  /* { dg-warning "10: format '%u' expects argument of type 'unsigned int', but argument 2 has type 'const char \\*' " } */
/* { dg-begin-multiline-output "" }
   printf("hello %" PRIu32 " world", msg);
          ^~~~~~~~~                  ~~~
                                     |
                                     const char *
   { dg-end-multiline-output "" } */
/* { dg-begin-multiline-output "" }
 #define PRIu32 "u"
                 ^
                 |
                 unsigned int
   { dg-end-multiline-output "" } */
#undef PRIu32
}

void test_macro_3 (const char *msg)
{
#define FMT_STRING "hello %i world" /* { dg-line test_macro_3_macro_line } */
  /* { dg-warning "20: format '%i' expects argument of type 'int', but argument 2 has type 'const char \\*'" "" { target *-*-*} .-1 } */
  printf(FMT_STRING, msg);  /* { dg-message "10: in expansion of macro 'FMT_STRING" } */
/* { dg-begin-multiline-output "" }
 #define FMT_STRING "hello %i world"
                    ^~~~~~~~~~~~~~~~
   { dg-end-multiline-output "" } */
/* { dg-begin-multiline-output "" }
   printf(FMT_STRING, msg);
          ^~~~~~~~~~
   { dg-end-multiline-output "" } */
/* { dg-message "28: format string is defined here" "" { target *-*-* } test_macro_3_macro_line } */
/* { dg-begin-multiline-output "" }
 #define FMT_STRING "hello %i world"
                           ~^
                            |
                            int
                           %s
   { dg-end-multiline-output "" } */
#undef FMT_STRING
}

void test_macro_4 (const char *msg)
{
#define FMT_STRING "hello %i world" /* { dg-warning "20: format '%i' expects argument of type 'int', but argument 2 has type 'const char \\*' " } */
  printf(FMT_STRING "\n", msg);  /* { dg-message "10: in expansion of macro 'FMT_STRING" } */
/* { dg-begin-multiline-output "" }
 #define FMT_STRING "hello %i world"
                    ^~~~~~~~~~~~~~~~
   { dg-end-multiline-output "" } */
/* { dg-begin-multiline-output "" }
   printf(FMT_STRING "\n", msg);
          ^~~~~~~~~~
   { dg-end-multiline-output "" } */
/* { dg-begin-multiline-output "" }
 #define FMT_STRING "hello %i world"
                           ~^
                            |
                            int
                           %s
   { dg-end-multiline-output "" } */
#undef FMT_STRING
}

void test_non_contiguous_strings (void)
{
  __builtin_printf(" %" "d ", 0.5); /* { dg-warning "20: format .%d. expects argument of type .int., but argument 2 has type .double." } */
                                    /* { dg-message "26: format string is defined here" "" { target *-*-* } .-1 } */
  /* { dg-begin-multiline-output "" }
   __builtin_printf(" %" "d ", 0.5);
                    ^~~~       ~~~
                               |
                               double
   { dg-end-multiline-output "" } */
  /* { dg-begin-multiline-output "" }
   __builtin_printf(" %" "d ", 0.5);
                      ~~~~^
                          |
                          int
                      %" "f
   { dg-end-multiline-output "" } */
}

void test_const_arrays (void)
{
  /* TODO: ideally we'd highlight both the format string *and* the use of
     it here.  For now, just verify that we gracefully handle this case.  */
  const char a[] = " %d ";
  __builtin_printf(a, 0.5); /* { dg-warning "20: format .%d. expects argument of type .int., but argument 2 has type .double." } */
  /* { dg-begin-multiline-output "" }
   __builtin_printf(a, 0.5);
                    ^  ~~~
                       |
                       double
   { dg-end-multiline-output "" } */
}
