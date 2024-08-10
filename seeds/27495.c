/* This test needs runtime that provides stpcpy function.  */
/* { dg-do run { target *-*-linux* *-*-gnu* *-*-uclinux* } } */
/* { dg-options "-O2" } */

#define USE_GNU
/* { dg-do run } */
/* { dg-options "-O2" } */

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
fn1 (char *p, size_t *l)
{
  char *q = strcat (p, "abcde");
  *l = strlen (p);
  return q;
}

__attribute__((noinline, noclone)) char *
fn2 (char *p, const char *q, size_t *l1, size_t *l2)
{
  size_t l = strlen (q);
  char *r = strcat (p, q);
  *l1 = l;
  *l2 = strlen (p);
  return r;
}

__attribute__((noinline, noclone)) char *
fn3 (char *p, const char *q, size_t *l)
{
  char *r = strcpy (p, q);
  *l = strlen (p);
  return r;
}

__attribute__((noinline, noclone)) char *
fn4 (char *p, const char *q, size_t *l)
{
  char *r = strcat (p, q);
  *l = strlen (p);
  return r;
}

__attribute__((noinline, noclone)) char *
fn5 (char *p, const char *q, size_t *l1, size_t *l2, size_t *l3)
{
  size_t l = strlen (q);
  size_t ll = strlen (p);
  char *r = strcat (p, q);
  *l1 = l;
  *l2 = strlen (p);
  *l3 = ll;
  return r;
}

__attribute__((noinline, noclone)) char *
fn6 (char *p, const char *q, size_t *l1, size_t *l2)
{
  size_t l = strlen (p);
  char *r = strcat (p, q);
  *l1 = strlen (p);
  *l2 = l;
  return r;
}

int
main ()
{
  char buf[64];
  const char *volatile q = "fgh";
  size_t l, l1, l2, l3;
  memset (buf, '\0', sizeof buf);
  memset (buf, 'a', 3);
  if (fn1 (buf, &l) != buf || l != 8 || memcmp (buf, "aaaabcde", 9) != 0)
    abort ();
  if (fn2 (buf, q, &l1, &l2) != buf || l1 != 3 || l2 != 11
      || memcmp (buf, "aaaabcdefgh", 12) != 0)
    abort ();
  if (fn3 (buf, q, &l) != buf || l != 3
      || memcmp (buf, "fgh\0bcdefgh", 12) != 0)
    abort ();
  if (fn4 (buf, q, &l) != buf || l != 6
      || memcmp (buf, "fghfgh\0efgh", 12) != 0)
    abort ();
  l1 = 0;
  l2 = 0;
  if (fn5 (buf, q, &l1, &l2, &l3) != buf || l1 != 3 || l2 != 9 || l3 != 6
      || memcmp (buf, "fghfghfgh\0h", 12) != 0)
    abort ();
  if (fn6 (buf, q, &l1, &l2) != buf || l1 != 12 || l2 != 9
      || memcmp (buf, "fghfghfghfgh", 13) != 0)
    abort ();
  return 0;
}
