/* Test for scanf formats.  Formats using C2X features.  */
/* { dg-do compile } */
/* { dg-options "-std=c2x -pedantic -Wformat" } */

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
foo (unsigned int *uip, unsigned short int *uhp, unsigned char *uhhp,
     unsigned long int *ulp, unsigned long long *ullp, uintmax_t *ujp,
     size_t *zp, unsigned_ptrdiff_t *utp, int_least8_t *i8, int_least16_t *i16,
     int_least32_t *i32, int_least64_t *i64, uint_least8_t *u8,
     uint_least16_t *u16, uint_least32_t *u32, uint_least64_t *u64,
     int_fast8_t *if8, int_fast16_t *if16, int_fast32_t *if32,
     int_fast64_t *if64, uint_fast8_t *uf8, uint_fast16_t *uf16,
     uint_fast32_t *uf32, uint_fast64_t *uf64)
{
  scanf ("%*b");
  scanf ("%2b", uip);
  scanf ("%hb%hhb%lb%llb%jb%zb%tb", uhp, uhhp, ulp, ullp, ujp, zp, utp);
  scanf ("%Lb", ullp); /* { dg-warning "does not support" } */
  scanf ("%qb", ullp); /* { dg-warning "does not support" } */
  /* Use of %wN and %wfN with each valid conversion specifier.  */
  scanf ("%w8d %w16d %w32d %w64d %wf8d %wf16d %wf32d %wf64d",
	 i8, i16, i32, i64, if8, if16, if32, if64);
  scanf ("%w8i %w16i %w32i %w64i %wf8i %wf16i %wf32i %wf64i",
	 i8, i16, i32, i64, if8, if16, if32, if64);
  scanf ("%w8b %w16b %w32b %w64b %wf8b %wf16b %wf32b %wf64b",
	 u8, u16, u32, u64, uf8, uf16, uf32, uf64);
  scanf ("%w8o %w16o %w32o %w64o %wf8o %wf16o %wf32o %wf64o",
	 u8, u16, u32, u64, uf8, uf16, uf32, uf64);
  scanf ("%w8u %w16u %w32u %w64u %wf8u %wf16u %wf32u %wf64u",
	 u8, u16, u32, u64, uf8, uf16, uf32, uf64);
  scanf ("%w8x %w16x %w32x %w64x %wf8x %wf16x %wf32x %wf64x",
	 u8, u16, u32, u64, uf8, uf16, uf32, uf64);
  scanf ("%w8X %w16X %w32X %w64X %wf8X %wf16X %wf32X %wf64X",
	 u8, u16, u32, u64, uf8, uf16, uf32, uf64);
  scanf ("%w8n %w16n %w32n %w64n %wf8n %wf16n %wf32n %wf64n",
	 i8, i16, i32, i64, if8, if16, if32, if64);
  /* Use of %wN and %wfN with bad conversion specifiers.  */
  scanf ("%w8a", i8); /* { dg-warning "length modifier" } */
  scanf ("%w16a", i16); /* { dg-warning "length modifier" } */
  scanf ("%w32a", i32); /* { dg-warning "length modifier" } */
  scanf ("%w64a", i64); /* { dg-warning "length modifier" } */
  scanf ("%wf8a", if8); /* { dg-warning "length modifier" } */
  scanf ("%wf16a", if16); /* { dg-warning "length modifier" } */
  scanf ("%wf32a", if32); /* { dg-warning "length modifier" } */
  scanf ("%wf64a", if64); /* { dg-warning "length modifier" } */
  scanf ("%w8A", i8); /* { dg-warning "length modifier" } */
  scanf ("%w16A", i16); /* { dg-warning "length modifier" } */
  scanf ("%w32A", i32); /* { dg-warning "length modifier" } */
  scanf ("%w64A", i64); /* { dg-warning "length modifier" } */
  scanf ("%wf8A", if8); /* { dg-warning "length modifier" } */
  scanf ("%wf16A", if16); /* { dg-warning "length modifier" } */
  scanf ("%wf32A", if32); /* { dg-warning "length modifier" } */
  scanf ("%wf64A", if64); /* { dg-warning "length modifier" } */
  scanf ("%w8c", i8); /* { dg-warning "length modifier" } */
  scanf ("%w16c", i16); /* { dg-warning "length modifier" } */
  scanf ("%w32c", i32); /* { dg-warning "length modifier" } */
  scanf ("%w64c", i64); /* { dg-warning "length modifier" } */
  scanf ("%wf8c", if8); /* { dg-warning "length modifier" } */
  scanf ("%wf16c", if16); /* { dg-warning "length modifier" } */
  scanf ("%wf32c", if32); /* { dg-warning "length modifier" } */
  scanf ("%wf64c", if64); /* { dg-warning "length modifier" } */
  scanf ("%w8e", i8); /* { dg-warning "length modifier" } */
  scanf ("%w16e", i16); /* { dg-warning "length modifier" } */
  scanf ("%w32e", i32); /* { dg-warning "length modifier" } */
  scanf ("%w64e", i64); /* { dg-warning "length modifier" } */
  scanf ("%wf8e", if8); /* { dg-warning "length modifier" } */
  scanf ("%wf16e", if16); /* { dg-warning "length modifier" } */
  scanf ("%wf32e", if32); /* { dg-warning "length modifier" } */
  scanf ("%wf64e", if64); /* { dg-warning "length modifier" } */
  scanf ("%w8E", i8); /* { dg-warning "length modifier" } */
  scanf ("%w16E", i16); /* { dg-warning "length modifier" } */
  scanf ("%w32E", i32); /* { dg-warning "length modifier" } */
  scanf ("%w64E", i64); /* { dg-warning "length modifier" } */
  scanf ("%wf8E", if8); /* { dg-warning "length modifier" } */
  scanf ("%wf16E", if16); /* { dg-warning "length modifier" } */
  scanf ("%wf32E", if32); /* { dg-warning "length modifier" } */
  scanf ("%wf64E", if64); /* { dg-warning "length modifier" } */
  scanf ("%w8f", i8); /* { dg-warning "length modifier" } */
  scanf ("%w16f", i16); /* { dg-warning "length modifier" } */
  scanf ("%w32f", i32); /* { dg-warning "length modifier" } */
  scanf ("%w64f", i64); /* { dg-warning "length modifier" } */
  scanf ("%wf8f", if8); /* { dg-warning "length modifier" } */
  scanf ("%wf16f", if16); /* { dg-warning "length modifier" } */
  scanf ("%wf32f", if32); /* { dg-warning "length modifier" } */
  scanf ("%wf64f", if64); /* { dg-warning "length modifier" } */
  scanf ("%w8F", i8); /* { dg-warning "length modifier" } */
  scanf ("%w16F", i16); /* { dg-warning "length modifier" } */
  scanf ("%w32F", i32); /* { dg-warning "length modifier" } */
  scanf ("%w64F", i64); /* { dg-warning "length modifier" } */
  scanf ("%wf8F", if8); /* { dg-warning "length modifier" } */
  scanf ("%wf16F", if16); /* { dg-warning "length modifier" } */
  scanf ("%wf32F", if32); /* { dg-warning "length modifier" } */
  scanf ("%wf64F", if64); /* { dg-warning "length modifier" } */
  scanf ("%w8g", i8); /* { dg-warning "length modifier" } */
  scanf ("%w16g", i16); /* { dg-warning "length modifier" } */
  scanf ("%w32g", i32); /* { dg-warning "length modifier" } */
  scanf ("%w64g", i64); /* { dg-warning "length modifier" } */
  scanf ("%wf8g", if8); /* { dg-warning "length modifier" } */
  scanf ("%wf16g", if16); /* { dg-warning "length modifier" } */
  scanf ("%wf32g", if32); /* { dg-warning "length modifier" } */
  scanf ("%wf64g", if64); /* { dg-warning "length modifier" } */
  scanf ("%w8G", i8); /* { dg-warning "length modifier" } */
  scanf ("%w16G", i16); /* { dg-warning "length modifier" } */
  scanf ("%w32G", i32); /* { dg-warning "length modifier" } */
  scanf ("%w64G", i64); /* { dg-warning "length modifier" } */
  scanf ("%wf8G", if8); /* { dg-warning "length modifier" } */
  scanf ("%wf16G", if16); /* { dg-warning "length modifier" } */
  scanf ("%wf32G", if32); /* { dg-warning "length modifier" } */
  scanf ("%wf64G", if64); /* { dg-warning "length modifier" } */
  scanf ("%w8p", i8); /* { dg-warning "length modifier" } */
  scanf ("%w16p", i16); /* { dg-warning "length modifier" } */
  scanf ("%w32p", i32); /* { dg-warning "length modifier" } */
  scanf ("%w64p", i64); /* { dg-warning "length modifier" } */
  scanf ("%wf8p", if8); /* { dg-warning "length modifier" } */
  scanf ("%wf16p", if16); /* { dg-warning "length modifier" } */
  scanf ("%wf32p", if32); /* { dg-warning "length modifier" } */
  scanf ("%wf64p", if64); /* { dg-warning "length modifier" } */
  scanf ("%w8s", i8); /* { dg-warning "length modifier" } */
  scanf ("%w16s", i16); /* { dg-warning "length modifier" } */
  scanf ("%w32s", i32); /* { dg-warning "length modifier" } */
  scanf ("%w64s", i64); /* { dg-warning "length modifier" } */
  scanf ("%wf8s", if8); /* { dg-warning "length modifier" } */
  scanf ("%wf16s", if16); /* { dg-warning "length modifier" } */
  scanf ("%wf32s", if32); /* { dg-warning "length modifier" } */
  scanf ("%wf64s", if64); /* { dg-warning "length modifier" } */
  scanf ("%w8[abc]", i8); /* { dg-warning "length modifier" } */
  scanf ("%w16[abc]", i16); /* { dg-warning "length modifier" } */
  scanf ("%w32[abc]", i32); /* { dg-warning "length modifier" } */
  scanf ("%w64[abc]", i64); /* { dg-warning "length modifier" } */
  scanf ("%wf8[abc]", if8); /* { dg-warning "length modifier" } */
  scanf ("%wf16[abc]", if16); /* { dg-warning "length modifier" } */
  scanf ("%wf32[abc]", if32); /* { dg-warning "length modifier" } */
  scanf ("%wf64[abc]", if64); /* { dg-warning "length modifier" } */
}
