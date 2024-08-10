/* This test needs runtime that provides stpcpy and mempcpy functions.  */
/* { dg-do run { target *-*-linux* *-*-gnu* *-*-uclinux* } } */
/* { dg-options "-O2 -fdump-tree-strlen" } */
/* Bionic targets don't have mempcpy */
/* { dg-require-effective-target non_bionic } */

#define USE_GNU
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

__attribute__((noinline, noclone)) char *
fn1 (char *p, size_t *l1, size_t *l2)
{
  char *a = mempcpy (p, "abcde", 6);
  /* This strlen needs to stay.  */
  size_t la = strlen (a);
  /* This strlen can be optimized into 5.  */
  size_t lp = strlen (p);
  *l1 = la;
  *l2 = lp;
  return a;
}

__attribute__((noinline, noclone)) char *
fn2 (char *p, const char *q, size_t *l1, size_t *l2, size_t *l3)
{
  /* This strlen needs to stay.  */
  size_t lq = strlen (q);
  char *a = mempcpy (p, q, lq + 1);
  /* This strlen needs to stay.  */
  size_t la = strlen (a);
  /* This strlen can be optimized into lq.  */
  size_t lp = strlen (p);
  *l1 = lq;
  *l2 = la;
  *l3 = lp;
  return a;
}

__attribute__((noinline, noclone)) char *
fn3 (char *p, size_t *l1, size_t *l2)
{
  char *a = stpcpy (p, "abcde");
  /* This strlen can be optimized into 0.  */
  size_t la = strlen (a);
  /* This strlen can be optimized into 5.  */
  size_t lp = strlen (p);
  *l1 = la;
  *l2 = lp;
  return a;
}

__attribute__((noinline, noclone)) char *
fn4 (char *p, const char *q, size_t *l1, size_t *l2, size_t *l3)
{
  /* This strlen needs to stay.  */
  size_t lq = strlen (q);
  char *a = stpcpy (p, q);
  /* This strlen can be optimized into 0.  */
  size_t la = strlen (a);
  /* This strlen can be optimized into lq.  */
  size_t lp = strlen (p);
  *l1 = lq;
  *l2 = la;
  *l3 = lp;
  return a;
}

__attribute__((noinline, noclone)) char *
fn5 (char *p, const char *q, size_t *l1, size_t *l2)
{
  char *a = stpcpy (p, q);
  /* This strlen can be optimized into 0.  */
  size_t la = strlen (a);
  /* This strlen can be optimized into a - p.  */
  size_t lp = strlen (p);
  *l1 = la;
  *l2 = lp;
  return a;
}

int
main ()
{
  char buf[64];
  const char *volatile q = "ABCDEFGH";
  size_t l1, l2, l3;
  memset (buf, '\0', sizeof buf);
  memset (buf + 6, 'z', 7);
  if (fn1 (buf, &l1, &l2) != buf + 6 || l1 != 7 || l2 != 5
      || memcmp (buf, "abcde\0zzzzzzz", 14) != 0)
    abort ();
  if (fn2 (buf, q, &l1, &l2, &l3) != buf + 9 || l1 != 8 || l2 != 4 || l3 != 8
      || memcmp (buf, "ABCDEFGH\0zzzz", 14) != 0)
    abort ();
  if (fn3 (buf, &l1, &l2) != buf + 5 || l1 != 0 || l2 != 5
      || memcmp (buf, "abcde\0GH\0zzzz", 14) != 0)
    abort ();
  l3 = 0;
  memset (buf, 'n', 9);
  if (fn4 (buf, q, &l1, &l2, &l3) != buf + 8 || l1 != 8 || l2 != 0 || l3 != 8
      || memcmp (buf, "ABCDEFGH\0zzzz", 14) != 0)
    abort ();
  memset (buf, 'm', 9);
  if (fn5 (buf, q, &l1, &l2) != buf + 8 || l1 != 0 || l2 != 8
      || memcmp (buf, "ABCDEFGH\0zzzz", 14) != 0)
    abort ();
  return 0;
}

/* { dg-final { scan-tree-dump-times "strlen \\(" 4 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "memcpy \\(" 1 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "mempcpy \\(" 2 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strcpy \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strcat \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strchr \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "stpcpy \\(" 2 "strlen1" } } */
