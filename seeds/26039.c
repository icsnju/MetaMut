/* PR tree-optimization/83821 - local aggregate initialization defeats
   strlen optimization
   { dg-do compile }
   { dg-options "-O2 -Wall -fdump-tree-optimized" }
   { dg-require-effective-target alloca } */

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
char *p_p2, *p_p5, *p_p9, *p_p14;

unsigned n0, n1, n2, n3, n4;


static inline __attribute__ ((always_inline)) void
elim_strlen_of_consecutive_strcpy (char *p)
{
  p_p2 = p + 2;
  __builtin_strcpy (p_p2, "12");

  p_p5 = p_p2 + 3;
  __builtin_strcpy (p_p5, "124");

  p_p9 = p_p5 + 4;
  __builtin_strcpy (p_p9, "1245");

  p_p14 = p_p9 + 5;

  n0 = __builtin_strlen (p);
  n1 = __builtin_strlen (p_p2);
  n2 = __builtin_strlen (p_p5);
  n3 = __builtin_strlen (p_p9);

  /* The following isn't handled yet:
     n4 = __builtin_strlen (p_p14); */

  if (n0 || n1 != 2 || n2 != 3 || n3 != 4)
    __builtin_abort ();
}


void elim_strlen_of_consecutive_strcpy_in_alloca (unsigned n)
{
  /* Only known sizes are handled so far.  */
  n = 14;

  char *p = __builtin_alloca (n);

  *p = '\0';

  elim_strlen_of_consecutive_strcpy (p);
}


void elim_strlen_of_consecutive_strcpy_in_vla (unsigned n)
{
  /* Only known sizes are handled so far.  */
  n = 14;

  char vla[n];

  *vla = '\0';

  elim_strlen_of_consecutive_strcpy (vla);
}

void elim_strlen_of_consecutive_strcpy_in_malloc (unsigned n)
{
  char *p = __builtin_malloc (n);

  *p = '\0';

  elim_strlen_of_consecutive_strcpy (p);
}


void elim_strlen_of_consecutive_strcpy_in_calloc (unsigned n)
{
  char *p = __builtin_calloc (n, 1);

  /* Do not store into *P to verify that strlen knows it's zero.  */

  elim_strlen_of_consecutive_strcpy (p);
}

/* { dg-final { scan-tree-dump-not "abort" "optimized" } } */
