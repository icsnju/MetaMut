/* { dg-do run } */
/* { dg-options "-O2 -fno-code-hoisting -fdump-tree-strlen -fdump-tree-optimized" } */

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
fn1 (int r)
{
  char *p = r ? "a" : "bc";
  /* String length for p varies, therefore strchr below isn't
     optimized away.  */
  return strchr (p, '\0');
}

__attribute__((noinline, noclone)) size_t
fn2 (int r)
{
  char *p, q[10];
  strcpy (q, "abc");
  p = r ? "a" : q;
  /* String length is constant for both alternatives, and strlen is
     optimized away.  */
  return strlen (p);
}

__attribute__((noinline, noclone)) size_t
fn3 (char *p, int n)
{
  int i;
  p = strchr (p, '\0');
  /* strcat here can be optimized into memcpy.  */
  strcat (p, "abcd");
  for (i = 0; i < n; i++)
    if ((i % 123) == 53)
      /* strcat here is optimized into strlen and memcpy.  */
      strcat (p, "efg");
  /* The strlen here can't be optimized away, as in the loop string
     length of p might change.  */
  return strlen (p);
}

char buf[64];

__attribute__((noinline, noclone)) size_t
fn4 (char *x, int n)
{
  int i;
  size_t l;
  char a[64];
  char *p = strchr (x, '\0');
  /* strcpy here is optimized into memcpy, length computed as p - x + 1.  */
  strcpy (a, x);
  /* strcat here is optimized into memcpy.  */
  strcat (p, "abcd");
  for (i = 0; i < n; i++)
    if ((i % 123) == 53)
      /* strcat here is optimized into strlen and memcpy.  */
      strcat (a, "efg");
  /* The strlen should be optimized here into 4.  */
  l = strlen (p);
  /* This stays strcpy.  */
  strcpy (buf, a);
  return l;
}

int
main ()
{
  volatile int l = 1;
  char b[64];

  if (memcmp (fn1 (l) - 1, "a", 2) != 0)
    abort ();
  if (memcmp (fn1 (!l) - 2, "bc", 3) != 0)
    abort ();
  if (fn2 (l) != 1 || fn2 (!l) != 3)
    abort ();
  memset (b, '\0', sizeof b);
  memset (b, 'a', 3);
  if (fn3 (b, 10) != 4 || memcmp (b, "aaaabcd", 8) != 0)
    abort ();
  if (fn3 (b, 128) != 7 || memcmp (b, "aaaabcdabcdefg", 15) != 0)
    abort ();
  if (fn3 (b, 256) != 10 || memcmp (b, "aaaabcdabcdefgabcdefgefg", 25) != 0)
    abort ();
  if (fn4 (b, 10) != 4
      || memcmp (b, "aaaabcdabcdefgabcdefgefgabcd", 29) != 0
      || memcmp (buf, "aaaabcdabcdefgabcdefgefg", 25) != 0)
    abort ();
  if (fn4 (b, 128) != 4
      || memcmp (b, "aaaabcdabcdefgabcdefgefgabcdabcd", 33) != 0
      || memcmp (buf, "aaaabcdabcdefgabcdefgefgabcdefg", 32) != 0)
    abort ();
  if (fn4 (b, 256) != 4
      || memcmp (b, "aaaabcdabcdefgabcdefgefgabcdabcdabcd", 37) != 0
      || memcmp (buf, "aaaabcdabcdefgabcdefgefgabcdabcdefgefg", 39) != 0)
    abort ();
  return 0;
}

/* { dg-final { scan-tree-dump-times "strlen \\(" 5 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "memcpy \\(" 6 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strcpy \\(" 1 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strcat \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strchr \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "stpcpy \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "return 4;" 1 "optimized" } } */
