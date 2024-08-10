/* Test whether buffer overflow warnings for __*_chk builtins
   are emitted properly.  */
/* { dg-do compile } */
/* { dg-options "-O2 -Wno-format -std=gnu99 -ftrack-macro-expansion=0 -fno-tree-dse" } */
// { dg-skip-if "packed attribute missing for t" { "epiphany-*-*" } }

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

#define va_list    __builtin_va_list
#define va_start   __builtin_va_start
#define va_end     __builtin_va_end

volatile void *vx;
char buf1[20];
int x;

void
test (int arg, ...)
{
  char buf2[20];
  va_list ap;
  char *p = &buf1[10], *q;

  memcpy (&buf2[19], "ab", 1);
  memcpy (&buf2[19], "ab", 2); /* { dg-warning "writing 2 bytes into a region of size 1" "memcpy" } */
  vx = mempcpy (&buf2[19], "ab", 1);
  vx = mempcpy (&buf2[19], "ab", 2); /* { dg-warning "writing 2 " "mempcpy" } */
  memmove (&buf2[18], &buf1[10], 2);
  memmove (&buf2[18], &buf1[10], 3); /* { dg-warning "writing 3 " "memmove" } */
  memset (&buf2[16], 'a', 4);
  memset (&buf2[15], 'b', 6); /* { dg-warning "writing 6 " "memset" } */
  strcpy (&buf2[18], "a");
  strcpy (&buf2[18], "ab"); /* { dg-warning "writing 3 " "strcpy" } */
  vx = stpcpy (&buf2[18], "a");
  vx = stpcpy (&buf2[18], "ab"); /* { dg-warning "writing 3" "stpcpy" } */
  strncpy (&buf2[18], "a", 2);

  /* Both warnings below are equally meaningful.  */
  strncpy (&buf2[18], "a", 3); /* { dg-warning "(writing 3 bytes into a region of size 2|specified bound 3 exceeds destination size 2)" "strncpy" } */

  strncpy (&buf2[18], "abc", 2);
  strncpy (&buf2[18], "abc", 3); /* { dg-warning "writing 3 " "strncpy" } */
  memset (buf2, '\0', sizeof (buf2));
  strcat (&buf2[18], "a");
  memset (buf2, '\0', sizeof (buf2));
  strcat (&buf2[18], "ab"); /* { dg-warning "writing 3 " "strcat" } */
  sprintf (&buf2[18], "%s", buf1);
  sprintf (&buf2[18], "%s", "a");
  sprintf (&buf2[18], "%s", "ab"); /* { dg-warning "writing 3 " "sprintf" } */
  sprintf (&buf2[18], "a");
  sprintf (&buf2[18], "ab"); /* { dg-warning "writing 3 " "sprintf" } */
  snprintf (&buf2[18], 2, "%d", x);
  /* N argument to snprintf is the size of the buffer.
     Although this particular call wouldn't overflow buf2,
     incorrect buffer size was passed to it and therefore
     we want a warning and runtime failure.  */
  snprintf (&buf2[18], 3, "%d", x); /* { dg-warning "specified bound 3 exceeds destination size 2" "snprintf" } */
  va_start (ap, arg);
  vsprintf (&buf2[18], "a", ap);
  va_end (ap);

  va_start (ap, arg);
  vsprintf (&buf2[18], "ab", ap); /* { dg-warning "writing 3" "vsprintf" } */
  va_end (ap);
  va_start (ap, arg);
  vsnprintf (&buf2[18], 2, "%s", ap);
  va_end (ap);
  va_start (ap, arg);
  /* See snprintf above.  */
  vsnprintf (&buf2[18], 3, "%s", ap); /* { dg-warning "specified bound 3 exceeds destination size 2" "vsnprintf" } */
  va_end (ap);

  p = p + 10;
  memset (p, 'd', 0);
  q = strcpy (p, ""); /* { dg-warning "writing 1 " "strcpy" } */

  /* This invokes undefined behavior, since we are past the end of buf1.  */
  p = p + 10;
  memset (p, 'd', 1); /* { dg-warning "writing 1 " "memset" } */

  memset (q, 'd', 0);
  memset (q, 'd', 1); /* { dg-warning "writing 1 " "memset" } */
  q = q - 10;
  memset (q, 'd', 10);
}

char *str = "ABCDEFG";
typedef struct { char b[16]; } H;

/* Some brown paper bag bugs found in real applications.
   This test is here merely for amusement.  */

void
test2 (const H h)
{
  char c;
  strncpy (&c, str, 3); /* { dg-warning "(writing 3 bytes into a region of size 1|specified bound 3 exceeds destination size 1)" "strncpy" } */

  struct { char b[4]; } x;
  sprintf (x.b, "%s", "ABCD"); /* { dg-warning "writing 5" "sprintf" } */

  unsigned int i;
  memcpy (&i, &h, sizeof (h)); /* { dg-warning "writing 16 " "memcpy" } */

  unsigned char buf[21];
  memset (buf + 16, 0, 8); /* { dg-warning "writing 8 " "memset" } */

  typedef struct { __INT32_TYPE__ i, j, k, l; } S;
  S *s[3];
  memset (s, 0, sizeof (S) * 3); /* { dg-warning "writing 48 " "memset" } */

  struct T { char a[8]; char b[4]; char c[10]; } t;
  stpcpy (t.c,"Testing..."); /* { dg-warning "writing" "stpcpy" } */

  char b1[7];
  char b2[4];
  memset (b1, 0, sizeof (b1));
  memset (b2, 0, sizeof (b1)); /* { dg-warning "writing 7" "memset" } */
}
