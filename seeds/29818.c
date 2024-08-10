/* Copyright (C) 2004, 2005  Free Software Foundation.

   Ensure builtin __strcat_chk performs correctly.  */

extern void abort (void);
typedef __SIZE_TYPE__ size_t;
extern size_t strlen(const char *);
extern void *memcpy (void *, const void *, size_t);
extern char *strcat (char *, const char *);
extern int memcmp (const void *, const void *, size_t);
extern char *strcpy (char *, const char *);
extern int strcmp (const char *, const char *);
extern void *memset (void *, int, size_t);
#define RESET_DST_WITH(FILLER) \
  do { memset (dst, 'X', sizeof (dst)); strcpy (dst, (FILLER)); } while (0)

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
char *s5;

void
__attribute__((noinline))
test1 (void)
{
  const char *const x1 = "hello world";
  const char *const x2 = "";
  char dst[64], *d2;

  chk_calls = 0;
  strcat_disallowed = 1;
  /* Following strcat calls should be optimized out at compile time.  */  
  RESET_DST_WITH (x1);
  if (strcat (dst, "") != dst || strcmp (dst, x1))
    abort ();
  RESET_DST_WITH (x1);
  if (strcat (dst, x2) != dst || strcmp (dst, x1))
    abort ();
  RESET_DST_WITH (x1); d2 = dst;
  if (strcat (++d2, x2) != dst+1 || d2 != dst+1 || strcmp (dst, x1))
    abort ();
  RESET_DST_WITH (x1); d2 = dst;
  if (strcat (++d2+5, x2) != dst+6 || d2 != dst+1 || strcmp (dst, x1))
    abort ();
  RESET_DST_WITH (x1); d2 = dst;
  if (strcat (++d2+5, x1+11) != dst+6 || d2 != dst+1 || strcmp (dst, x1))
    abort ();
  if (chk_calls)
    abort ();
  strcat_disallowed = 0;

  RESET_DST_WITH (x1);
  if (strcat (dst, " 1111") != dst
      || memcmp (dst, "hello world 1111\0XXX", 20))
    abort ();
  
  RESET_DST_WITH (x1);
  if (strcat (dst+5, " 2222") != dst+5
      || memcmp (dst, "hello world 2222\0XXX", 20))
    abort ();
  
  RESET_DST_WITH (x1); d2 = dst;
  if (strcat (++d2+5, " 3333") != dst+6 || d2 != dst+1
      || memcmp (dst, "hello world 3333\0XXX", 20))
    abort ();
  
  RESET_DST_WITH (x1);
  strcat (strcat (strcat (strcat (strcat (strcat (dst, ": this "), ""),
				  "is "), "a "), "test"), ".");
  if (memcmp (dst, "hello world: this is a test.\0X", 30))
    abort ();

  chk_calls = 0;
  strcat_disallowed = 1;
  /* Test at least one instance of the __builtin_ style.  We do this
     to ensure that it works and that the prototype is correct.  */
  RESET_DST_WITH (x1);
  if (__builtin_strcat (dst, "") != dst || strcmp (dst, x1))
    abort ();
  if (chk_calls)
    abort ();
  strcat_disallowed = 0;
}


/* Test whether compile time checking is done where it should
   and so is runtime object size checking.  */
void
__attribute__((noinline))
test2 (void)
{
  struct A { char buf1[10]; char buf2[10]; } a;
  char *r = l1 == 1 ? &a.buf1[5] : &a.buf2[4];
  char buf3[20];
  int i;

  /* The following calls should do runtime checking
     - source length is not known, but destination is.  */
  memset (&a, '\0', sizeof (a));
  s5 = (char *) &a;
  __asm __volatile ("" : : "r" (s5) : "memory");
  chk_calls = 0;
  strcat (a.buf1 + 2, s3 + 3);
  strcat (r, s3 + 2);
  r = l1 == 1 ? __builtin_alloca (4) : &a.buf2[7];
  memset (r, '\0', 3);
  __asm __volatile ("" : : "r" (r) : "memory");
  strcat (r, s2 + 2);
  strcat (r + 2, s3 + 3);
  r = buf3;
  for (i = 0; i < 4; ++i)
    {
      if (i == l1 - 1)
	r = &a.buf1[1];
      else if (i == l1)
	r = &a.buf2[7];
      else if (i == l1 + 1)
	r = &buf3[5];
      else if (i == l1 + 2)
	r = &a.buf1[9];
    }
  strcat (r, s2 + 4);
  if (chk_calls != 5)
    abort ();

  /* Following have known destination and known source length,
     but we don't know the length of dest string, so runtime checking
     is needed too.  */
  memset (&a, '\0', sizeof (a));
  chk_calls = 0;
  s5 = (char *) &a;
  __asm __volatile ("" : : "r" (s5) : "memory");
  strcat (a.buf1 + 2, "a");
  strcat (r, "");
  r = l1 == 1 ? __builtin_alloca (4) : &a.buf2[7];
  memset (r, '\0', 3);
  __asm __volatile ("" : : "r" (r) : "memory");
  strcat (r, s1 + 1);
  if (chk_calls != 2)
    abort ();
  chk_calls = 0;
  /* Unknown destination and source, no checking.  */
  strcat (s4, s3);
  if (chk_calls)
    abort ();
  chk_calls = 0;
}

/* Test whether runtime and/or compile time checking catches
   buffer overflows.  */
void
__attribute__((noinline))
test3 (void)
{
  struct A { char buf1[10]; char buf2[10]; } a;
  char buf3[20];

  memset (&a, '\0', sizeof (a));
  memset (buf3, '\0', sizeof (buf3));
  s5 = (char *) &a;
  __asm __volatile ("" : : "r" (s5) : "memory");
  s5 = buf3;
  __asm __volatile ("" : : "r" (s5) : "memory");
  chk_fail_allowed = 1;
  /* Runtime checks.  */
  if (__builtin_setjmp (chk_fail_buf) == 0)
    {
      strcat (&a.buf2[9], s2 + 3);
      abort ();
    }
  if (__builtin_setjmp (chk_fail_buf) == 0)
    {
      strcat (&a.buf2[7], s3 + strlen (s3) - 3);
      abort ();
    }
  if (__builtin_setjmp (chk_fail_buf) == 0)
    {
      strcat (&buf3[19], "a");
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
  memset (p, '\0', sizeof (p));
  test2 ();
  test3 ();
}
