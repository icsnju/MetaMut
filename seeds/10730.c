/* This test needs runtime that provides stpcpy and __*_chk functions.  */
/* { dg-do run { target *-*-linux* *-*-gnu* *-*-uclinux* } } */
/* { dg-options "-O2 -fdump-tree-strlen" } */

#define USE_GNU
#define FORTIFY_SOURCE 2
/* { dg-do run } */
/* { dg-options "-O2 -fdump-tree-strlen" } */

/* This is a replacement of needed parts from <stdlib.h> and <string.h>
   for -foptimize-strlen testing, to ensure we are testing the builtins
   rather than whatever the OS has in its headers.  */

#define NULL ((void *) 0)
typedef __SIZE_TYPE__ size_t;
extern void abort (void);
void *calloc (size_t, size_t);
void *malloc (size_t);
void free (void *);
char *strdup (const char *);
size_t strlen (const char *);
size_t strnlen (const char *, size_t);
void *memcpy (void *__restrict, const void *__restrict, size_t);
void *memmove (void *, const void *, size_t);
char *strcpy (char *__restrict, const char *__restrict);
char *strcat (char *__restrict, const char *__restrict);
char *strchr (const char *, int);
int strcmp (const char *, const char *);
int strncmp (const char *, const char *, size_t);
void *memset (void *, int, size_t);
int memcmp (const void *, const void *, size_t);
int strcmp (const char *, const char *);
#ifdef USE_GNU
void *mempcpy (void *__restrict, const void *__restrict, size_t);
char *stpcpy (char *__restrict, const char *__restrict);
#endif

int sprintf (char * __restrict, const char *__restrict, ...);
int snprintf (char * __restrict, size_t, const char *__restrict, ...);

#if defined(FORTIFY_SOURCE) && FORTIFY_SOURCE > 0 && __OPTIMIZE__
# define bos(ptr) __builtin_object_size (ptr, FORTIFY_SOURCE > 0)
# define bos0(ptr) __builtin_object_size (ptr, 0)

extern inline __attribute__((gnu_inline, always_inline, artificial)) void *
memcpy (void *__restrict dest, const void *__restrict src, size_t len)
{
  return __builtin___memcpy_chk (dest, src, len, bos0 (dest));
}

extern inline __attribute__((gnu_inline, always_inline, artificial)) void *
memmove (void *dest, const void *src, size_t len)
{
  return __builtin___memmove_chk (dest, src, len, bos0 (dest));
}

extern inline __attribute__((gnu_inline, always_inline, artificial)) char *
strcpy (char *__restrict dest, const char *__restrict src)
{
  return __builtin___strcpy_chk (dest, src, bos (dest));
}

extern inline __attribute__((gnu_inline, always_inline, artificial)) char *
strcat (char *__restrict dest, const char *__restrict src)
{
  return __builtin___strcat_chk (dest, src, bos (dest));
}

# ifdef USE_GNU
extern inline __attribute__((gnu_inline, always_inline, artificial)) void *
mempcpy (void *__restrict dest, const void *__restrict src, size_t len)
{
  return __builtin___mempcpy_chk (dest, src, len, bos0 (dest));
}

extern inline __attribute__((gnu_inline, always_inline, artificial)) char *
stpcpy (char *__restrict dest, const char *__restrict src)
{
  return __builtin___stpcpy_chk (dest, src, bos (dest));
}
# endif
#endif

/* If stpcpy can't be used, this is optimized into
   strcpy (p, q); strcat (p, r); memcpy (p + strlen (p), "abcd", 5);
   If stpcpy can be used (see strlenopt-4g.c test),
   this is optimized into
   memcpy (stpcpy (stpcpy (p, q), r), "abcd", 5);  */
__attribute__((noinline, noclone)) void
foo (char *p, const char *q, const char *r)
{
  strcpy (p, q);
  strcat (p, r);
  strcat (p, "abcd");
}

/* If stpcpy can't be used, this is optimized into
   memcpy (p, "abcd", 4); strcpy (p + 4, q); strcat (p, r);
   If stpcpy can be used, this is optimized into
   memcpy (p, "abcd", 4); strcpy (stpcpy (p + 4, q), r);  */
__attribute__((noinline, noclone)) void
bar (char *p, const char *q, const char *r)
{
  strcpy (p, "abcd");
  strcat (p, q);
  strcat (p, r);
}

/* If stpcpy can't be used, this is optimized into
   strcat (p, q); memcpy (t1 = p + strlen (p), "abcd", 4);
   strcpy (t1 + 4, r); memcpy (p + strlen (p), "efgh", 5);
   If stpcpy can be used, this is optimized into
   t1 = stpcpy (p + strlen (p), q); memcpy (t1, "abcd", 4);
   memcpy (stpcpy (t1 + 4, r), "efgh", 5);  */
__attribute__((noinline, noclone)) void
baz (char *p, const char *q, const char *r)
{
  strcat (p, q);
  strcat (p, "abcd");
  strcat (p, r);
  strcat (p, "efgh");
}

char buf[64];

int
main ()
{
  char *volatile p = buf;
  const char *volatile q = "ij";
  const char *volatile r = "klmno";
  foo (p, q, r);
  if (memcmp (buf, "ijklmnoabcd\0\0\0\0\0\0\0\0", 20) != 0)
    abort ();
  memset (buf, '\0', sizeof buf);
  bar (p, q, r);
  if (memcmp (buf, "abcdijklmno\0\0\0\0\0\0\0\0", 20) != 0)
    abort ();
  memset (buf, 'v', 3);
  memset (buf + 3, '\0', -3 + sizeof buf);
  baz (p, q, r);
  if (memcmp (buf, "vvvijabcdklmnoefgh\0", 20) != 0)
    abort ();
  return 0;
}

/* { dg-final { scan-tree-dump-times "strlen \\(" 3 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "memcpy \\(" 4 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strcpy \\(" 3 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strcat \\(" 3 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strchr \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "stpcpy \\(" 0 "strlen1" } } */

/* { dg-final { scan-tree-dump-times "strlen \\(" 1 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "__memcpy_chk \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "__strcpy_chk \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "__strcat_chk \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strchr \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "__stpcpy_chk \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "memcpy \\(" 4 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strcpy \\(" 1 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strcat \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "stpcpy \\(" 5 "strlen1" } } */
