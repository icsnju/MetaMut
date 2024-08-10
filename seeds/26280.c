/* PR tree-optimization/86428 - strlen of const array initialized with
   a string of the same length not folded
   { dg-do compile }
   { dg-options "-O0 -Wall -fdump-tree-gimple" } */

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

const char a1[1] = "\0";
const char a2[2] = "1\0";
const char a3[3] = "12\0";
const char a8[8] = "1234567\0";
const char a9[9] = "12345678\0";

const char ax[9] = "12345678\0\0\0\0";   /* { dg-warning "initializer-string for array of 'char' is too long" } */
const char ay[9] = "\00012345678\0\0\0\0";   /* { dg-warning "initializer-string for array of 'char' is too long" } */


int len1 (void)
{
  size_t len0 = strlen (a1);
  return len0;
}

int len (void)
{
  size_t len = strlen (a2) + strlen (a3) + strlen (a8) + strlen (a9);
  return len;
}

int lenx (void)
{
  size_t lenx = strlen (ax);
  return lenx;
}

int leny (void)
{
  size_t leny = strlen (ay);
  return leny;
}

int cmp88 (void)
{
  int cmp88 = memcmp (a8, "1234567\0", sizeof a8);
  return cmp88;
}

/* { dg-final { scan-tree-dump-times "strlen1" 0 "gimple" } }
   { dg-final { scan-tree-dump-times "len0 = 0;" 1 "gimple" } }
   { dg-final { scan-tree-dump-times "len = 18;" 1 "gimple" } }
   { dg-final { scan-tree-dump-times "lenx = 8;" 1 "gimple" } }
   { dg-final { scan-tree-dump-times "leny = 0;" 1 "gimple" } }
   { dg-final { scan-tree-dump-times "cmp88 = 0;" 1 "gimple" } } */
