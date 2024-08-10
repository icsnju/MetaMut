/* Verify that strcmp and strncmp calls with mixed constant and
   non-constant strings are evaluated correctly.
   { dg-do run }
   { dg-options "-O2" } */

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

__attribute__ ((noclone, noinline)) int
test_strlen_gt2_strcmp_abcd (const char *s)
{
  if (strlen (s) < 3)
    return -1;

  return strcmp (s, "abcd") == 0;
}

__attribute__ ((noclone, noinline)) int
test_strlen_lt6_strcmp_abcd (const char *s)
{
  if (strlen (s) > 5)
    return -1;

  return strcmp (s, "abcd") == 0;
}

__attribute__ ((noclone, noinline)) int
test_strcpy_strcmp_abc (const char *s)
{
  char a[5];
  strcpy (a, s);
  return strcmp (a, "abc") == 0;
}

__attribute__ ((noclone, noinline)) int
test_strcpy_abc_strcmp (const char *s)
{
  char a[4], b[6];
  strcpy (a, "abc");
  strcpy (b, s);
  return strcmp (a, b) == 0;
}

/* Exercise strcmp of two strings between 1 and 3 characters long
   stored in arrays of the same known size.  */
char ga4[4], gb4[4];

__attribute__ ((noclone, noinline)) int
test_store_0_nulterm_strcmp_same_size_arrays (void)
{
  ga4[0] = gb4[0] = 'x';
  ga4[3] = gb4[3] = '\0';
  return strcmp (ga4, gb4) == 0;
}

__attribute__ ((noclone, noinline)) int
test_store_0_nulterm_strncmp_bound_2_same_size_arrays (void)
{
  ga4[0] = gb4[0] = 'x';
  ga4[3] = gb4[3] = '\0';
  return strncmp (ga4, gb4, 2) == 0;
}

__attribute__ ((noclone, noinline)) int
test_store_0_nulterm_strncmp_bound_equal_same_size_arrays (void)
{
  ga4[0] = gb4[0] = 'x';
  ga4[3] = gb4[3] = '\0';
  return strncmp (ga4, gb4, 4) == 0;
}

/* Exercise strcmp of two strings between 0 and 3 characters long
   stored in arrays of the same known size.  */

__attribute__ ((noclone, noinline)) int
test_nulterm_strcmp_same_size_arrays (void)
{
  ga4[3] = gb4[3] = '\0';
  return strcmp (ga4, gb4) == 0;
}

/* Exercise strcmp of two strings between 1 and 3 and 1 and 4 characters
   long, respectively, stored in arrays of known but different sizes.  */
char gc5[5];

__attribute__ ((noclone, noinline)) int
test_store_0_nulterm_strcmp_arrays (void)
{
  ga4[0] = gc5[0] = 'x';
  ga4[3] = gc5[4] = '\0';
  return strcmp (ga4, gc5) == 0;
}

/* Exercise strcmp of two strings between 0 and 3 and 1 and 4 characters
   long, respectively, stored in arrays of known but different sizes.  */

__attribute__ ((noclone, noinline)) int
test_nulterm_strcmp_arrays (void)
{
  ga4[3] = gc5[4] = '\0';
  return strcmp (ga4, gc5) == 0;
}


__attribute__ ((noclone, noinline)) int
test_strcpy_strncmp_abcd (const char *s)
{
  char a[6];
  strcpy (a, s);
  return strcmp (a, "abcd") == 0;
}

__attribute__ ((noclone, noinline)) int
test_strcpy_abcd_strncmp_3 (const char *s)
{
  char a[6], b[8];
  strcpy (a, "abcd");
  strcpy (b, s);
  return strncmp (a, b, 3) == 0;
}

__attribute__ ((noclone, noinline)) int
test_strcpy_abcd_strncmp_4 (const char *s)
{
  char a[6], b[8];
  strcpy (a, "abcd");
  strcpy (b, s);
  return strncmp (a, b, 4) == 0;
}


int main (void)
{
  test_strlen_gt2_strcmp_abcd ("abcd");
  test_strlen_lt6_strcmp_abcd ("abcd");

  A (0 == test_strcpy_strcmp_abc ("ab"));
  A (0 != test_strcpy_strcmp_abc ("abc"));
  A (0 == test_strcpy_strcmp_abc ("abcd"));

  A (0 == test_strcpy_abc_strcmp ("ab"));
  A (0 != test_strcpy_abc_strcmp ("abc"));
  A (0 == test_strcpy_abc_strcmp ("abcd"));

  strcpy (ga4, "abc"); strcpy (gb4, "abd");
  A (0 == test_store_0_nulterm_strcmp_same_size_arrays ());
  strcpy (ga4, "abd"); strcpy (gb4, "abc");
  A (0 == test_store_0_nulterm_strcmp_same_size_arrays ());
  strcpy (ga4, "abc"); strcpy (gb4, "abc");
  A (0 != test_store_0_nulterm_strcmp_same_size_arrays ());

  strcpy (ga4, "abc"); strcpy (gb4, "acd");
  A (0 == test_store_0_nulterm_strncmp_bound_2_same_size_arrays ());
  strcpy (ga4, "acd"); strcpy (gb4, "abc");
  A (0 == test_store_0_nulterm_strncmp_bound_2_same_size_arrays ());
  strcpy (ga4, "abc"); strcpy (gb4, "abc");
  A (0 != test_store_0_nulterm_strncmp_bound_2_same_size_arrays ());

  strcpy (ga4, "abc"); strcpy (gb4, "abd");
  A (0 == test_store_0_nulterm_strncmp_bound_equal_same_size_arrays ());
  strcpy (ga4, "abd"); strcpy (gb4, "abc");
  A (0 == test_store_0_nulterm_strncmp_bound_equal_same_size_arrays ());
  strcpy (ga4, "abc"); strcpy (gb4, "abc");
  A (0 != test_store_0_nulterm_strncmp_bound_equal_same_size_arrays ());

  strcpy (ga4, "abc"); strcpy (gb4, "abd");
  A (0 == test_nulterm_strcmp_same_size_arrays ());
  strcpy (ga4, "abd"); strcpy (gb4, "abc");
  A (0 == test_nulterm_strcmp_same_size_arrays ());
  strcpy (ga4, "abc"); strcpy (gb4, "abc");
  A (0 != test_nulterm_strcmp_same_size_arrays ());

  strcpy (ga4, "abc"); strcpy (gc5, "abcd");
  A (0 == test_store_0_nulterm_strcmp_arrays ());
  strcpy (ga4, "abd"); strcpy (gc5, "abcd");
  A (0 == test_store_0_nulterm_strcmp_arrays ());
  strcpy (ga4, "abc"); strcpy (gc5, "abc");
  A (0 != test_store_0_nulterm_strcmp_arrays ());

  strcpy (ga4, "abc"); strcpy (gc5, "abcd");
  A (0 == test_nulterm_strcmp_arrays ());
  strcpy (ga4, "abd"); strcpy (gc5, "abc");
  A (0 == test_nulterm_strcmp_arrays ());
  strcpy (ga4, "abc"); strcpy (gc5, "abc");
  A (0 != test_nulterm_strcmp_arrays ());

  A (0 == test_strcpy_strncmp_abcd ("ab"));
  A (0 == test_strcpy_strncmp_abcd ("abc"));
  A (0 != test_strcpy_strncmp_abcd ("abcd"));
  A (0 == test_strcpy_strncmp_abcd ("abcde"));

  A (0 == test_strcpy_abcd_strncmp_3 ("ab"));
  A (0 != test_strcpy_abcd_strncmp_3 ("abc"));
  A (0 != test_strcpy_abcd_strncmp_3 ("abcd"));
  A (0 != test_strcpy_abcd_strncmp_3 ("abcde"));

  A (0 == test_strcpy_abcd_strncmp_4 ("ab"));
  A (0 == test_strcpy_abcd_strncmp_4 ("abc"));
  A (0 != test_strcpy_abcd_strncmp_4 ("abcd"));
  A (0 != test_strcpy_abcd_strncmp_4 ("abcde"));
}
