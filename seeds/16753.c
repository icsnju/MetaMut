/* PR tree-optimization/92226 - live nul char store to array eliminated
   Test to verify warnings are issued for overflow detected thanks to
   the enhancement committed on top of the fix for PR 92226.
   { dg-do compile }
   { dg-options "-O2 -Wall -Wno-array-bounds -ftrack-macro-expansion=0" } */

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

#define NOIPA __attribute__ ((noipa))

#define T(MIN, MAX, SIZE, IDX)						\
  NOIPA void								\
  test_ ## MIN ## _ ## MAX ## _ ## SIZE ## _ ## IDX (const char *s)	\
  {									\
    size_t len = strlen (s);						\
    if (MIN <= len && len <= MAX)					\
      {									\
	extern char d[];						\
	strcpy (d, s);							\
	d[IDX] = 0;							\
	extern char a ## SIZE[SIZE];					\
	strcpy (a ## SIZE, d);						\
      }									\
  } typedef void dummy_type


T (2, 2, 1, 0);
T (2, 2, 1, 1);     // { dg-warning "writing 2 bytes into a region of size 1" }
T (2, 2, 1, 2);     // { dg-warning "writing 3 bytes into a region of size 1" }

T (2, 3, 1, 0);
T (2, 3, 1, 1);     // { dg-warning "writing 2 bytes into a region of size 1" }
T (2, 3, 1, 2);     // { dg-warning "writing 3 bytes into a region of size 1" "" { xfail *-*-*} }
T (2, 3, 1, 3);     // { dg-warning "writing 4 bytes into a region of size 1" "" { xfail *-*-* } }

T (5, 7, 3, 1);
T (5, 7, 3, 2);
T (5, 7, 3, 3);     // { dg-warning "writing 4 bytes into a region of size 3" }
T (5, 7, 3, 4);     // { dg-warning "writing 5 bytes into a region of size 3" }
T (5, 7, 3, 5);     // { dg-warning "writing 6 bytes into a region of size 3" "" { xfail *-*-* } }
