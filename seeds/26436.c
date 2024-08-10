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

volatile int v;

size_t
f1 (void)
{
  char a[30];
  v += 1;
  memcpy (a, "1234567", 8);
  memcpy (a + 7, "89abcdefg", 10);
  memcpy (a + 16, "h", 2);
  return strlen (a);	// This strlen should be optimized into 17.
}

size_t
f2 (void)
{
  char a[30];
  v += 2;
  strcpy (a, "1234567");
  strcpy (a + 7, "89abcdefg");
  strcpy (a + 16, "h");
  return strlen (a);	// This strlen should be optimized into 17.
}

size_t
f3 (char *a)
{
  v += 3;
  memcpy (a, "1234567", 8);
  memcpy (a + 7, "89abcdefg", 10);
  memcpy (a + 16, "h", 2);
  return strlen (a);	// This strlen should be optimized into 17.
}

size_t
f4 (char *a)
{
  v += 4;
  strcpy (a, "1234567");
  strcpy (a + 7, "89abcdefg");
  strcpy (a + 16, "h");
  return strlen (a);	// This strlen should be optimized into 17.
}

int
main ()
{
  char a[30];
  if (f1 () != 17 || f2 () != 17 || f3 (a) != 17 || f4 (a) != 17)
    abort ();
  return 0;
}

/* { dg-final { scan-tree-dump-times "strlen \\(" 0 "strlen1" } } */