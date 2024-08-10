/* Copyright (C) 2004, 2005  Free Software Foundation.

   Ensure builtin __vsprintf_chk performs correctly.  */

#include <stdarg.h>

extern void abort (void);
typedef __SIZE_TYPE__ size_t;
extern size_t strlen(const char *);
extern void *memcpy (void *, const void *, size_t);
extern char *strcpy (char *, const char *);
extern int memcmp (const void *, const void *, size_t);
extern void *memset (void *, int, size_t);
extern int vsprintf (char *, const char *, va_list);

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

const char s1[] = "123";
char p[32] = "";
char *s2 = "defg";
char *s3 = "FGH";
char *s4;
size_t l1 = 1;
static char buffer[32];
char * volatile ptr = "barf";  /* prevent constant propagation to happen when whole program assumptions are made.  */

int
__attribute__((noinline))
test1_sub (int i, ...)
{
  int ret = 0;
  va_list ap;
  va_start (ap, i);
  switch (i)
    {
    case 0:
      vsprintf (buffer, "foo", ap);
      break;
    case 1:
      ret = vsprintf (buffer, "foo", ap);
      break;
    case 2:
      vsprintf (buffer, "%s", ap);
      break;
    case 3:
      ret = vsprintf (buffer, "%s", ap);
      break;
    case 4:
      vsprintf (buffer, "%d - %c", ap);
      break;
    case 5:
      vsprintf (s4, "%d - %c", ap);
      break;
    }
  va_end (ap);
  return ret;
}

void
__attribute__((noinline))
test1 (void)
{
  chk_calls = 0;
  vsprintf_disallowed = 1;

  memset (buffer, 'A', 32);
  test1_sub (0);
  if (memcmp (buffer, "foo", 4) || buffer[4] != 'A')
    abort ();

  memset (buffer, 'A', 32);
  if (test1_sub (1) != 3)
    abort ();
  if (memcmp (buffer, "foo", 4) || buffer[4] != 'A')
    abort ();

  if (chk_calls)
    abort ();
  vsprintf_disallowed = 0;

  memset (buffer, 'A', 32);
  test1_sub (2, "bar");
  if (memcmp (buffer, "bar", 4) || buffer[4] != 'A')
    abort ();

  memset (buffer, 'A', 32);
  if (test1_sub (3, "bar") != 3)
    abort ();
  if (memcmp (buffer, "bar", 4) || buffer[4] != 'A')
    abort ();

  memset (buffer, 'A', 32);
  test1_sub (2, ptr);
  if (memcmp (buffer, "barf", 5) || buffer[5] != 'A')
    abort ();

  memset (buffer, 'A', 32);
  test1_sub (4, (int) l1 + 27, *ptr);
  if (memcmp (buffer, "28 - b\0AAAAA", 12))
    abort ();

  if (chk_calls != 4)
    abort ();
  chk_calls = 0;

  test1_sub (5, (int) l1 - 17, ptr[1]);
  if (memcmp (s4, "-16 - a", 8))
    abort ();
  if (chk_calls)
    abort ();
}

void
__attribute__((noinline))
test2_sub (int i, ...)
{
  va_list ap;
  struct A { char buf1[10]; char buf2[10]; } a;
  char *r = l1 == 1 ? &a.buf1[5] : &a.buf2[4];
  char buf3[20];
  int j;

  va_start (ap, i);
  /* The following calls should do runtime checking
     - source length is not known, but destination is.  */
  switch (i)
    {
    case 0:
      vsprintf (a.buf1 + 2, "%s", ap);
      break;
    case 1:
      vsprintf (r, "%s%c", ap);
      break;
    case 2:
      r = l1 == 1 ? __builtin_alloca (4) : &a.buf2[7];
      vsprintf (r, "%c %s", ap);
      break;
    case 3:
      r = l1 == 1 ? __builtin_alloca (4) : &a.buf2[7];
      vsprintf (r + 2, s3 + 3, ap);
      break;
    case 4:
    case 7:
      r = buf3;
      for (j = 0; j < 4; ++j)
	{
	  if (j == l1 - 1)
	    r = &a.buf1[1];
	  else if (j == l1)
	    r = &a.buf2[7];
	  else if (j == l1 + 1)
	    r = &buf3[5];
	  else if (j == l1 + 2)
	    r = &a.buf1[9];
	}
      if (i == 4)
	vsprintf (r, s2 + 4, ap);
      else
	vsprintf (r, "a", ap);
      break;
    case 5:
      r = l1 == 1 ? __builtin_alloca (4) : &a.buf2[7];
      vsprintf (r, "%s", ap);
      break;
    case 6:
      vsprintf (a.buf1 + 2, "", ap);
      break;
    case 8:
      vsprintf (s4, "%s %d", ap);
      break;
    }
  va_end (ap);
}

/* Test whether compile time checking is done where it should
   and so is runtime object size checking.  */
void
__attribute__((noinline))
test2 (void)
{
  /* The following calls should do runtime checking
     - source length is not known, but destination is.  */
  chk_calls = 0;
  test2_sub (0, s3 + 3);
  test2_sub (1, s3 + 3, s3[3]);
  test2_sub (2, s2[2], s2 + 4);
  test2_sub (3);
  test2_sub (4);
  test2_sub (5, s1 + 1);
  if (chk_calls != 6)
    abort ();

  /* Following have known destination and known source length,
     so if optimizing certainly shouldn't result in the checking
     variants.  */
  chk_calls = 0;
  vsprintf_disallowed = 1;
  test2_sub (6);
  test2_sub (7);
  vsprintf_disallowed = 0;
  /* Unknown destination and source, no checking.  */
  test2_sub (8, s3, 0);
  if (chk_calls)
    abort ();
}

void
__attribute__((noinline))
test3_sub (int i, ...)
{
  va_list ap;
  struct A { char buf1[10]; char buf2[10]; } a;
  char buf3[20];

  va_start (ap, i);
  /* The following calls should do runtime checking
     - source length is not known, but destination is.  */
  switch (i)
    {
    case 0:
      vsprintf (&a.buf2[9], "%c%s", ap);
      break;
    case 1:
      vsprintf (&a.buf2[7], "%s%c", ap);
      break;
    case 2:
      vsprintf (&a.buf2[7], "%d", ap);
      break;
    case 3:
      vsprintf (&buf3[17], "%s", ap);
      break;
    case 4:
      vsprintf (&buf3[19], "a", ap);
      break;
    }
  va_end (ap);
}

/* Test whether runtime and/or compile time checking catches
   buffer overflows.  */
void
__attribute__((noinline))
test3 (void)
{
  chk_fail_allowed = 1;
  /* Runtime checks.  */
  if (__builtin_setjmp (chk_fail_buf) == 0)
    {
      test3_sub (0, s2[3], s2 + 4);
      abort ();
    }
  if (__builtin_setjmp (chk_fail_buf) == 0)
    {
      test3_sub (1, s3 + strlen (s3) - 2, *s3);
      abort ();
    }
  if (__builtin_setjmp (chk_fail_buf) == 0)
    {
      test3_sub (2, (int) l1 + 9999);
      abort ();
    }
  if (__builtin_setjmp (chk_fail_buf) == 0)
    {
      test3_sub (3, "abc");
      abort ();
    }
  /* This should be detectable at compile time already.  */
  if (__builtin_setjmp (chk_fail_buf) == 0)
    {
      test3_sub (4);
      abort ();
    }
  chk_fail_allowed = 0;
}

void
main_test (void)
{
#ifndef __OPTIMIZE__
  /* Object size checking is only intended for -O[s123].  */
  return;
#endif
  __asm ("" : "=r" (s2) : "0" (s2));
  __asm ("" : "=r" (s3) : "0" (s3));
  __asm ("" : "=r" (l1) : "0" (l1));
  s4 = p;
  test1 ();
  test2 ();
  test3 ();
}
