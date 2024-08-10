/* PR tree-optimization/91294 - strlen result of a conditional with
   an offset
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

#define NOIPA __attribute__ ((noclone, noinline, noipa))

int i = 0;

const char s[] = "1234567";

char a[32];

/* Exercise a memcpy overwriting a destination string of known length
   with a source argument involving a conditional expression with strings
   of unqual lengths, with the selected one being the longer of the two
   and resulting in no change to the length of the overwritten destination
   string.  */
NOIPA void test_memcpy_same_length ()
{
  memcpy (a, "123456789a", 11);
  memcpy (a + 6, i ? "78\0" : "789\0", 4);
  if (strlen (a) != 9)
    abort ();
}

/* Same as above but with strcpy/strcat.  */

NOIPA void test_strcpy_strcat_same_length ()
{
  strcpy (a, "12345678");
  strcat (a, "9a");
  memcpy (a + 6, i ? "78\0" : "789\0", 4);
  if (strlen (a) != 9)
    abort ();
}

/* Same as above but using a memcpy of a power-of-two size that gets
   (on some targets) transformed into a single MEM_REF assignment.  */

NOIPA void test_assign_same_length ()
{
  memcpy (a, s, 8);
  memcpy (a + 5, i ? "67\0" : "678\0", 4);
  if (strlen (a) != 8)
    abort ();
}

/* Same as above but resulting in increasing the length of the destination
   string.  */

NOIPA void test_memcpy_lengthen ()
{
  memcpy (a, "123456789a", 11);
  memcpy (a + 8, i ? "9a\0" : "9ab\0", 4);
  if (strlen (a) != 11)
    abort ();
}

NOIPA void test_strcpy_strcat_lengthen ()
{
  strcpy (a, "12345678");
  strcat (a, "9a");
  memcpy (a + 8, i ? "9a\0" : "9ab\0", 4);
  if (strlen (a) != 11)
    abort ();
}

NOIPA void test_assign_lengthen ()
{
  memcpy (a, s, 8);
  memcpy (a + 6, i ? "78\0" : "789\0", 4);
  if (strlen (a) != 9)
    abort ();
}

NOIPA void test_memcpy_shorten ()
{
  memcpy (a, "123456789a", 11);
  memcpy (a + 6, i ? "789\0" : "78\0", 4);
  if (strlen (a) != 8)
    abort ();
}

NOIPA void test_strcpy_strcat_shorten ()
{
  strcpy (a, "12345678");
  strcat (a, "9a");
  memcpy (a + 6, i ? "789\0" : "78\0", 4);
  if (strlen (a) != 8)
    abort ();
}

NOIPA void test_assign_shorten ()
{
  memcpy (a, s, 8);
  memcpy (a + 6, i ? "789\0" : "78\0", 4);
  if (strlen (a) != 8)
    abort ();
}


int main (void)
{
  test_memcpy_same_length ();
  test_strcpy_strcat_same_length ();
  test_assign_same_length ();

  test_memcpy_lengthen ();
  test_strcpy_strcat_lengthen ();
  test_assign_lengthen ();

  test_memcpy_shorten ();
  test_strcpy_strcat_shorten ();
  test_assign_shorten ();
}
