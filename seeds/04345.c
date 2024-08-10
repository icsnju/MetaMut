/* PRE tree-optimization/90626 - fold strcmp(a, b) == 0 to zero when
   one string length is exact and the other is unequal
   { dg-do run }
   { dg-options "-O2 -Wall" } */

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

#define A(expr)                                                 \
  ((expr)                                                       \
   ? (void)0                                                    \
   : (__builtin_printf ("assertion failed on line %i: %s\n",    \
                        __LINE__, #expr),                       \
      __builtin_abort ()))


__attribute__ ((noclone, noinline, noipa)) void
clobber (void *p, int x, size_t n)
{
  for (volatile unsigned char *q = p; n--; )
    *q = x;
}

__attribute__ ((noclone, noinline, noipa)) void
test_strcmp (void)
{
  char a[8], b[8];
  strcpy (a, "1235");
  strcpy (b, "1234");

  A (strcmp (a, b));

  clobber (a, 0, sizeof a);
  clobber (b, 0, sizeof b);
  clobber (b + 4, '5', 1);

  memcpy (a, "1234", 4);
  memcpy (b, "1234", 4);

  A (0 > strcmp (a, b));
  A (0 < strcmp (b, a));
}

__attribute__ ((noclone, noinline, noipa)) void
test_strncmp (void)
{
  char a[8], b[8];
  strcpy (a, "1235");
  strcpy (b, "1234");

  A (0 == strncmp (a, b, 1));
  A (0 == strncmp (a, b, 2));
  A (0 == strncmp (a, b, 3));
  A (0 <  strncmp (a, b, 4));
  A (0 >  strncmp (b, a, 4));

  clobber (a, 0, sizeof a);
  clobber (b, 0, sizeof b);
  clobber (b + 4, '5', 1);

  memcpy (a, "1234", 4);
  memcpy (b, "1234", 4);

  A (0 == strncmp (a, b, 4));
  A (0 >  strncmp (a, b, 5));
  A (0 <  strncmp (b, a, 5));
}


__attribute__ ((noclone, noinline, noipa)) void
test_strncmp_a4_cond_s5_s2_2 (const char *s, int i)
{
  char a4[4];
  strcpy (a4, s);
  A (0 == strncmp (a4, i ? "12345" : "12", 2));
}


__attribute__ ((noclone, noinline, noipa)) void
test_strncmp_a4_cond_a5_s2_5 (const char *s, const char *t, int i)
{
  char a4[4], a5[5];
  strcpy (a4, s);
  strcpy (a5, t);
  A (0 == strncmp (a4, i ? a5 : "12", 5));
}

__attribute__ ((noclone, noinline, noipa)) void
test_strncmp_a4_cond_a5_a3_n (const char *s1, const char *s2, const char *s3,
			      int i, unsigned n)
{
  char a3[3], a4[4], a5[5];
  strcpy (a3, s1);
  strcpy (a4, s2);
  strcpy (a5, s3);
  A (0 == strncmp (a4, i ? a5 : a3, n));
}


int main (void)
{
  test_strcmp ();
  test_strncmp ();
  test_strncmp_a4_cond_s5_s2_2 ("12", 0);
  test_strncmp_a4_cond_a5_s2_5 ("12", "1234", 0);

  test_strncmp_a4_cond_a5_a3_n ("12", "1", "1",    0, 1);
  test_strncmp_a4_cond_a5_a3_n ("",   "1", "1234", 1, 1);

  test_strncmp_a4_cond_a5_a3_n ("12", "12", "1",    0, 2);
  test_strncmp_a4_cond_a5_a3_n ("",   "12", "1234", 1, 2);

  test_strncmp_a4_cond_a5_a3_n ("12", "123", "1",    0, 2);
  test_strncmp_a4_cond_a5_a3_n ("",   "123", "1234", 1, 3);
}
