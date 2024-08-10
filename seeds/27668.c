/* PR tree-optimization/90662 - strlen of a string in a vla plus offset
   not folded
   Verify that strlen of pointers to wide character arrays (emulated
   by int16_t) are computed correctly (whether folded or not).
   { dg-do run }
   { dg-options "-O2 -Wall -Wno-incompatible-pointer-types" } */

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

typedef __INT16_TYPE__ int16_t;

#define A(expr)                                                 \
  ((expr)                                                       \
   ? (void)0                                                    \
   : (__builtin_printf ("assertion failed on line %i: %s\n",    \
                        __LINE__, #expr),                       \
      __builtin_abort ()))

typedef int16_t A5[5];

A5 a5[5];
A5* p[5] = { &a5[4], &a5[3], &a5[2], &a5[1], &a5[0] };

__attribute__ ((noclone, noinline, noipa))
void deref_deref (void)
{
  strcpy (**p, "12345");
  A (strlen (**p) == 5);
}

__attribute__ ((noclone, noinline, noipa))
void deref_idx_0 (void)
{
  strcpy (*p[0], "");
  A (strlen (*p[0]) == 0);
}

__attribute__ ((noclone, noinline, noipa))
void deref_idx_1 (void)
{
  strcpy (*p[1], "12");
  A (strlen (*p[1]) == 2);
  A (strlen (&(*p[1])[1]) == 0);

  A (strlen ((char*)*p[1] + 1) == 1);
  A (strlen ((char*)*p[1] + 2) == 0);
  A (strlen ((char*)*p[1] + 3) == 0);

  A (strlen ((char*)&(*p[1])[1] + 1) == 0);

  A (strlen (*p[0]) == 0);
}

__attribute__ ((noclone, noinline, noipa))
void deref_idx_2 (void)
{
  strcpy (*p[2], "1234");
  A (strlen (*p[2]) == 4);
  A (strlen (&(*p[2])[1]) == 2);
  A (strlen (&(*p[2])[2]) == 0);

  A (strlen ((char*)*p[2] + 1) == 3);
  A (strlen ((char*)*p[2] + 2) == 2);
  A (strlen ((char*)*p[2] + 3) == 1);
  A (strlen ((char*)*p[2] + 4) == 0);
  A (strlen ((char*)*p[2] + 5) == 0);

  A (strlen ((char*)&(*p[2])[1] + 1) == 1);
  A (strlen ((char*)&(*p[2])[1] + 2) == 0);

  A (strlen (*p[1]) == 2);
  A (strlen (*p[0]) == 0);
}

__attribute__ ((noclone, noinline, noipa))
void deref_idx_3 (void)
{
  strcpy (*p[3], "123456");
  A (strlen (*p[3]) == 6);
  A (strlen (&(*p[3])[1]) == 4);
  A (strlen (&(*p[3])[2]) == 2);
  A (strlen (&(*p[3])[3]) == 0);

  A (strlen ((char*)*p[3] + 1) == 5);
  A (strlen ((char*)*p[3] + 2) == 4);
  A (strlen ((char*)*p[3] + 3) == 3);
  A (strlen ((char*)*p[3] + 4) == 2);
  A (strlen ((char*)*p[3] + 5) == 1);
  A (strlen ((char*)*p[3] + 6) == 0);

  A (strlen (*p[2]) == 4);
  A (strlen (*p[1]) == 2);
  A (strlen (*p[0]) == 0);
}

__attribute__ ((noclone, noinline, noipa))
void deref_idx_4 (void)
{
  strcpy (*p[4], "12345678");
  A (strlen (*p[4]) == 8);
  A (strlen (&(*p[4])[1]) == 6);
  A (strlen (&(*p[4])[2]) == 4);
  A (strlen (&(*p[4])[3]) == 2);
  A (strlen (&(*p[4])[4]) == 0);

  A (strlen (*p[3]) == 6);
  A (strlen (*p[2]) == 4);
  A (strlen (*p[1]) == 2);
  A (strlen (*p[0]) == 0);
}

__attribute__ ((noclone, noinline, noipa))
void deref_idx_4_x (void)
{
  strcpy (*p[4], "");
  A (strlen (*p[4]) == 0);
  A (strlen (*p[3]) == 6);
  A (strlen (*p[2]) == 4);
  A (strlen (*p[1]) == 2);
  A (strlen (*p[0]) == 0);
}

__attribute__ ((noclone, noinline, noipa))
void deref_idx_3_x (void)
{
  strcpy (&(*p[3])[0], "1");
  A (strlen (*p[4]) == 0);
  A (strlen (*p[3]) == 1);
  A (strlen (*p[2]) == 4);
  A (strlen (*p[1]) == 2);
  A (strlen (*p[0]) == 0);
}

__attribute__ ((noclone, noinline, noipa))
void deref_idx_2_x (void)
{
  strcpy (*p[2], "12");
  A (strlen (*p[4]) == 0);
  A (strlen (*p[3]) == 1);
  A (strlen (*p[2]) == 2);
  A (strlen (*p[1]) == 2);
  A (strlen (*p[0]) == 0);
}

__attribute__ ((noclone, noinline, noipa))
void deref_idx_1_x (void)
{
  strcpy (*p[1], "123");
  A (strlen (*p[4]) == 0);
  A (strlen (*p[3]) == 1);
  A (strlen (*p[2]) == 2);
  A (strlen (*p[1]) == 3);
  A (strlen (*p[0]) == 0);
}

__attribute__ ((noclone, noinline, noipa))
void deref_idx_0_x (void)
{
  strcpy (*p[0], "1234");
  A (strlen (*p[4]) == 0);
  A (strlen (*p[3]) == 1);
  A (strlen (*p[2]) == 2);
  A (strlen (*p[1]) == 3);
  A (strlen (*p[0]) == 4);
}

int main (void)
{
  deref_deref ();

  deref_idx_0 ();
  deref_idx_1 ();
  deref_idx_2 ();
  deref_idx_3 ();
  deref_idx_4 ();

  deref_idx_4_x ();
  deref_idx_3_x ();
  deref_idx_2_x ();
  deref_idx_1_x ();
  deref_idx_0_x ();
}
