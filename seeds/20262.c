/* { dg-do compile } */
/* { dg-options "-O2 -Wall -Wno-array-bounds -ftrack-macro-expansion=0" } */

extern void abort (void);

#ifndef os
# define os(ptr) __builtin_object_size (ptr, 0)
#endif

/* This is one of the alternatives for object size checking.
   If dst has side-effects, size checking will never be done.  */
#undef memcpy
#define memcpy(dst, src, len) \
  __builtin___memcpy_chk (dst, src, len, os (dst))
#undef mempcpy
#define mempcpy(dst, src, len) \
  __builtin___mempcpy_chk (dst, src, len, os (dst))
#undef memmove
#define memmove(dst, src, len) \
  __builtin___memmove_chk (dst, src, len, os (dst))
#undef memset
#define memset(dst, val, len) \
  __builtin___memset_chk (dst, val, len, os (dst))
#undef strcpy
#define strcpy(dst, src) \
  __builtin___strcpy_chk (dst, src, os (dst))
#undef stpcpy
#define stpcpy(dst, src) \
  __builtin___stpcpy_chk (dst, src, os (dst))
#undef strcat
#define strcat(dst, src) \
  __builtin___strcat_chk (dst, src, os (dst))
#undef strncpy
#define strncpy(dst, src, len) \
  __builtin___strncpy_chk (dst, src, len, os (dst))
#undef stpncpy
#define stpncpy(dst, src, len) \
  __builtin___stpncpy_chk (dst, src, len, os (dst))
#undef strncat
#define strncat(dst, src, len) \
  __builtin___strncat_chk (dst, src, len, os (dst))
#undef sprintf
#define sprintf(dst, ...) \
  __builtin___sprintf_chk (dst, 0, os (dst), __VA_ARGS__)
#undef vsprintf
#define vsprintf(dst, fmt, ap) \
  __builtin___vsprintf_chk (dst, 0, os (dst), fmt, ap)
#undef snprintf
#define snprintf(dst, len, ...) \
  __builtin___snprintf_chk (dst, len, 0, os (dst), __VA_ARGS__)
#undef vsnprintf
#define vsnprintf(dst, len, fmt, ap) \
  __builtin___vsnprintf_chk (dst, len, 0, os (dst), fmt, ap)

/* Now "redefine" even builtins for the purpose of testing.  */
#undef __builtin_memcpy
#define __builtin_memcpy(dst, src, len) memcpy (dst, src, len)
#undef __builtin_mempcpy
#define __builtin_mempcpy(dst, src, len) mempcpy (dst, src, len)
#undef __builtin_memmove
#define __builtin_memmove(dst, src, len) memmove (dst, src, len)
#undef __builtin_memset
#define __builtin_memset(dst, val, len) memset (dst, val, len)
#undef __builtin_strcpy
#define __builtin_strcpy(dst, src) strcpy (dst, src)
#undef __builtin_stpcpy
#define __builtin_stpcpy(dst, src) stpcpy (dst, src)
#undef __builtin_strcat
#define __builtin_strcat(dst, src) strcat (dst, src)
#undef __builtin_strncpy
#define __builtin_strncpy(dst, src, len) strncpy (dst, src, len)
#undef __builtin_strncat
#define __builtin_strncat(dst, src, len) strncat (dst, src, len)
#undef __builtin_sprintf
#define __builtin_sprintf(dst, ...) sprintf (dst, __VA_ARGS__)
#undef __builtin_vsprintf
#define __builtin_vsprintf(dst, fmt, ap) vsprintf (dst, fmt, ap)
#undef __builtin_snprintf
#define __builtin_snprintf(dst, len, ...) snprintf (dst, len, __VA_ARGS__)
#undef __builtin_vsnprintf
#define __builtin_vsnprintf(dst, len, fmt, ap) vsnprintf (dst, len, fmt, ap)

extern void *chk_fail_buf[];
extern volatile int chk_fail_allowed, chk_calls;
extern volatile int memcpy_disallowed, mempcpy_disallowed, memmove_disallowed;
extern volatile int memset_disallowed, strcpy_disallowed, stpcpy_disallowed;
extern volatile int strncpy_disallowed, stpncpy_disallowed, strcat_disallowed;
extern volatile int strncat_disallowed, sprintf_disallowed, vsprintf_disallowed;
extern volatile int snprintf_disallowed, vsnprintf_disallowed;

/* A storage class that ensures that declarations bind locally.  We want
   to test non-static declarations where we know it is safe to do so.  */
#if __PIC__ && !__PIE__
#define LOCAL static
#else
#define LOCAL
#endif

extern char *mallocminus1(int size) __attribute__((alloc_size(-1))); /* { dg-warning ".alloc_size. attribute argument value .-1. exceeds the number of function parameters 1" } */
extern char *malloc0(int size) __attribute__((alloc_size(0))); /* { dg-warning ".alloc_size. attribute argument value .0. does not refer to a function parameter" } */
extern char *malloc1(int size) __attribute__((alloc_size(1)));
extern char *malloc2(int empty, int size) __attribute__((alloc_size(2)));
extern char *calloc1(int size, int elements) __attribute__((alloc_size(1,2)));
extern char *calloc2(int size, int empty, int elements) __attribute__((alloc_size(1,3)));
extern char *balloc1(void *size) __attribute__((alloc_size(1)));   /* { dg-warning ".alloc_size. attribute argument value .1. refers to parameter type .void *." } */

void
test (void)
{
  char *p;

  p = malloc0 (6);
  strcpy (p, "Hello");
  p = malloc1 (6);
  strcpy (p, "Hello");
  strcpy (p, "Hello World"); /* { dg-warning "\\\[-Warray-bounds|-Wstringop-overflow" "strcpy" } */
  p = malloc2 (__INT_MAX__ >= 1700000 ? 424242 : __INT_MAX__ / 4, 6);
  strcpy (p, "World");
  strcpy (p, "Hello World"); /* { dg-warning "\\\[-Warray-bounds|-Wstringop-overflow" "strcpy" } */
  p = calloc1 (2, 5);
  strcpy (p, "World");
  strcpy (p, "Hello World"); /* { dg-warning "\\\[-Warray-bounds|-Wstringop-overflow" "strcpy" } */
  p = calloc2 (2, __INT_MAX__ >= 1700000 ? 424242 : __INT_MAX__ / 4, 5);
  strcpy (p, "World");
  strcpy (p, "Hello World"); /* { dg-warning "\\\[-Warray-bounds|-Wstringop-overflow" "strcpy" } */
}

