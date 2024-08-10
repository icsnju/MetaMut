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

struct S { char *p; size_t l; };

__attribute__((noinline, noclone)) struct S
foo (char *x, int n)
{
  int i;
  char a[64];
  char *p = strchr (x, '\0');
  struct S s;
  /* strcpy here is optimized into memcpy, length computed as p - x + 1.  */
  strcpy (a, x);
  /* strcat here is optimized into memcpy.  */
  strcat (p, "abcd");
  for (i = 0; i < n; i++)
    if ((i % 123) == 53)
      /* strcat here is optimized into strlen and memcpy.  */
      strcat (a, "efg");
  s.p = strdup (a);
  /* The strlen should be optimized here into 4.  */
  s.l = strlen (p);
  return s;
}

int
main ()
{
  char buf[32];
  struct S s;
  buf[0] = 'z';
  buf[1] = '\0';
  s = foo (buf, 0);
  if (s.l != 4 || memcmp (buf, "zabcd", 6) != 0)
    abort ();
  if (s.p == NULL)
    return 0;
  if (memcmp (s.p, "z", 2) != 0)
    abort ();
  s = foo (buf, 60);
  if (s.l != 4 || memcmp (buf, "zabcdabcd", 10) != 0)
    abort ();
  if (s.p == NULL)
    return 0;
  if (memcmp (s.p, "zabcdefg", 9) != 0)
    abort ();
  s = foo (buf, 240);
  if (s.l != 4 || memcmp (buf, "zabcdabcdabcd", 14) != 0)
    abort ();
  if (s.p == NULL)
    return 0;
  if (memcmp (s.p, "zabcdabcdefgefg", 16) != 0)
    abort ();
  return 0;
}

/* { dg-final { scan-tree-dump-times "strlen \\(" 2 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "memcpy \\(" 3 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strcpy \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strcat \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strchr \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "stpcpy \\(" 0 "strlen1" } } */
