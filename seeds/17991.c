/* Verify strlen results of vector assignments.
   { dg-do compile }
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

#define V(N) __attribute__ ((vector_size (N)))

typedef V (1) char VC1;
typedef V (2) char VC2;
typedef V (4) char VC4;
typedef V (8) char VC8;
typedef V (16) char VC16;

extern char a[];

#define A(expr) ((expr) ? (void)0 : abort ())

void test_fold (int i)
{
  *(VC4*)a = (VC4){ };
  A (strlen (a) == 0);
  A (!a[1] && !a[2] && !a[3]);

  *(VC4*)a = (VC4){ 0, 1 };
  A (strlen (a) == 0);
  A (a[1] == 1 && !a[2] && !a[3]);

  *(VC4*)a = (VC4){ 1 };
  A (strlen (a) == 1);
  A (!a[1] && !a[2] && !a[3]);

  *(VC4*)a = (VC4){ 1, 0, 3 };
  A (strlen (a) == 1);
  A (!a[1] && a[2] == 3 && !a[3]);

  *(VC4*)a = (VC4){ 1, 2 };
  A (strlen (a) == 2);
  A (!a[2] && !a[3]);

  *(VC4*)a = (VC4){ 1, 2, 0, 4 };
  A (strlen (a) == 2);
  A (!a[2] && a[3] == 4);

  *(VC4*)a = (VC4){ 1, 2, 3 };
  A (strlen (a) == 3);
  A (!a[3]);

  *(VC8*)a = (VC8){ 1, 2, 3, 0, 5 };
  A (strlen (a) == 3);

  *(VC8*)a = (VC8){ 1, 2, 3, 0, 5, 6 };
  A (strlen (a) == 3);

  *(VC8*)a = (VC8){ 1, 2, 3, 0, 5, 6, 7, 8 };
  A (strlen (a) == 3);
  A (strlen (a + 1) == 2);
  A (strlen (a + 2) == 1);
  A (strlen (a + 3) == 0);

  A (a[4] == 5 && a[5] == 6 && a[6] == 7 && a[7] == 8);
}

/* { dg-final { scan-tree-dump-not "abort \\(" "optimized" } }
   { dg-final { scan-tree-dump-not "strlen \\(" "optimized" } } */
