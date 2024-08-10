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

__attribute__((noinline, noclone)) size_t
fn1 (char *p, size_t l)
{
  memcpy (p, "abcdef", l);
  /* This strlen can't be optimized, as l is unknown.  */
  return strlen (p);
}

__attribute__((noinline, noclone)) size_t
fn2 (char *p, const char *q, size_t *lp)
{
  size_t l = strlen (q), l2;
  memcpy (p, q, 7);
  /* This strlen can't be optimized, as l might be bigger than 7.  */
  l2 = strlen (p);
  *lp = l;
  return l2;
}

__attribute__((noinline, noclone)) char *
fn3 (char *p)
{
  *p = 0;
  return p + 1;
}

int
main ()
{
  char buf[64];
  const char *volatile q = "ABCDEFGH";
  const char *volatile q2 = "IJ\0KLMNOPQRS";
  size_t l;
  memset (buf, '\0', sizeof buf);
  memset (buf + 2, 'a', 7);
  if (fn1 (buf, 3) != 9 || memcmp (buf, "abcaaaaaa", 10) != 0)
    abort ();
  if (fn1 (buf, 7) != 6 || memcmp (buf, "abcdef\0aa", 10) != 0)
    abort ();
  if (fn2 (buf, q, &l) != 9 || l != 8 || memcmp (buf, "ABCDEFGaa", 10) != 0)
    abort ();
  if (fn2 (buf, q2, &l) != 2 || l != 2 || memcmp (buf, "IJ\0KLMNaa", 10) != 0)
    abort ();
  if (fn3 (buf) != buf + 1 || memcmp (buf, "\0J\0KLMNaa", 10) != 0)
    abort ();
  return 0;
}

/* { dg-final { scan-tree-dump-times "strlen \\(" 3 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "memcpy \\(" 2 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strcpy \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strcat \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strchr \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "stpcpy \\(" 0 "strlen1" } } */
