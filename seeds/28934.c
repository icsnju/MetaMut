/* PR tree-optimization/86204 - wrong strlen result after prior strnlen
   { dg-do run }
   { dg-options "-O2 -Wall -fdump-tree-optimized" } */

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

#define NOIPA   __attribute__ ((noipa))

char a[] = "12345";

NOIPA void f0 (void)
{
  unsigned n0 = strnlen (a, 0);
  unsigned n1 = strlen (a);

  if (n0 != 0 || n1 != 5)
    abort ();
}

NOIPA void f1 (void)
{
  unsigned n0 = strnlen (a, 1);
  unsigned n1 = strlen (a);

  if (n0 != 1 || n1 != 5)
    abort ();
}

NOIPA void f2 (void)
{
  unsigned n0 = strnlen (a, 2);
  unsigned n1 = strlen (a);

  if (n0 != 2 || n1 != 5)
    abort ();
}

NOIPA void f3 (void)
{
  unsigned n0 = strnlen (a, 3);
  unsigned n1 = strlen (a);

  if (n0 != 3 || n1 != 5)
    abort ();
}

NOIPA void f4 (void)
{
  unsigned n0 = strnlen (a, 4);
  unsigned n1 = strlen (a);

  if (n0 != 4 || n1 != 5)
    abort ();
}

NOIPA void f5 (void)
{
  unsigned n0 = strnlen (a, 5);
  unsigned n1 = strlen (a);

  if (n0 != 5 || n1 != 5)
    abort ();
}

NOIPA void f6 (void)
{
  unsigned n0 = strnlen (a, 6);
  unsigned n1 = strlen (a);

  if (n0 != 5 || n1 != 5)
    abort ();
}

NOIPA void fx (unsigned n)
{
  unsigned n0 = strnlen (a, n);
  unsigned n1 = strlen (a);

  unsigned min = n < 5 ? n : 5;
  if (n0 != min || n1 != 5)
    abort ();
}

NOIPA void g2 (void)
{
  strcpy (a, "123");
  unsigned n0 = strnlen (a, 2);
  unsigned n1 = strlen (a);

  if (n0 != 2 || n1 != 3)
    abort ();
}

NOIPA void g7 (void)
{
  strcpy (a, "123");
  unsigned n0 = strnlen (a, 7);
  unsigned n1 = strlen (a);

  if (n0 != 3 || n1 != 3)
    abort ();
}

NOIPA void gx (unsigned n)
{
  strcpy (a, "123");
  unsigned n0 = strnlen (a, n);
  unsigned n1 = strlen (a);

  unsigned min = n < 3 ? n : 3;
  if (n0 != min || n1 != 3)
    abort ();
}

int main (void)
{
  f0 ();
  f1 ();
  f2 ();
  f3 ();
  f4 ();
  f5 ();
  f6 ();
  fx (2);
  fx (7);

  g2 ();
  g7 ();
  gx (2);
  gx (7);
}


/* For targets like Solaris 10 that don't define strnlen().  */

NOIPA size_t
strnlen (const char *s, size_t n)
{
  size_t len = 0;
  while (*s++ && n--)
    ++len;
  return len;
}

/* Verify that at least some of the 11 calls to strnlen have been
   folded (this number of folded calls may need to be adjusted up
   if the strnlen optimization improves, but it should not go down.
  { dg-final { scan-tree-dump-times "= strnlen" 7 "optimized" } } */
