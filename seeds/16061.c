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

#define assert(expr)						\
  ((expr)                                                       \
   ? (void)0                                                    \
   : (__builtin_printf ("line %i %s: assertion failed: %s\n",	\
                        __LINE__, __func__, #expr),		\
      __builtin_abort ()))

int i = 0;

const char s[] = "1234567";

char a[32];

NOIPA void lower_bound_assign_into_empty (void)
{
  a[0] = '1';
  a[1] = '2';
  a[2] = '3';
  assert (strlen (a) == 3);
}

NOIPA void lower_bound_assign_into_longest (void)
{
  a[0] = '1';
  a[1] = '2';
  a[2] = '3';
  assert (strlen (a) == 31);
}


NOIPA void lower_bound_assign_into_empty_idx_3 (int idx)
{
  a[0] = '1';
  a[1] = '2';
  a[2] = '3';
  a[idx] = 'x';
  assert (strlen (a) == 4);
}

NOIPA void lower_bound_assign_into_longest_idx_2 (int idx)
{
  a[0] = '1';
  a[1] = '2';
  a[2] = '3';
  a[idx] = '\0';
  assert (strlen (a) == 2);
}


NOIPA void lower_bound_memcpy_into_empty (void)
{
  memcpy (a, "123", 3);
  assert (strlen (a) == 3);
}

NOIPA void lower_bound_memcpy_into_longest (void)
{
  memcpy (a, "123", 3);
  assert (strlen (a) == 31);
}


NOIPA void lower_bound_memcpy_memcpy_into_empty (void)
{
  memcpy (a, "123", 3);
  memcpy (a + 2, "345", 3);
  assert (strlen (a) == 5);
}

NOIPA void lower_bound_memcpy_memcpy_into_longest (void)
{
  memcpy (a, "123", 3);
  memcpy (a + 2, "345", 3);
  assert (strlen (a) == 31);
}


NOIPA void memove_forward_strlen (void)
{
  char a[] = "123456";

  memmove (a, a + 1, sizeof a - 1);

  assert (strlen (a) == 5);
}

NOIPA void memove_backward_into_empty_strlen (void)
{
  strcpy (a, "123456");

  memmove (a + 1, a, 6);

  assert (strlen (a) == 7);
}

NOIPA void memove_backward_into_longest_strlen (void)
{
  memcpy (a, "123456", 6);

  memmove (a + 1, a, 6);

  assert (strlen (a) == 31);
}

NOIPA void memove_strcmp (void)
{
  /* Test derived from libstdc++-v3's
     20_util/specialized_algorithms/memory_management_tools/1.cc  */

  char a[] = "123456";
  char b[] = "000000";

  memmove (b, a, sizeof a);

  assert (strlen (a) == 6);
  assert (strlen (b) == 6);
  assert (strcmp (a, b) == 0);
}


int main (void)
{
  memset (a, '\0', sizeof a);
  lower_bound_assign_into_empty ();

  memset (a, 'x', sizeof a - 1);
  a[sizeof a - 1] = '\0';
  lower_bound_assign_into_longest ();

  memset (a, '\0', sizeof a);
  lower_bound_assign_into_empty_idx_3 (3);

  memset (a, 'x', sizeof a - 1);
  a[sizeof a - 1] = '\0';
  lower_bound_assign_into_longest_idx_2 (2);

  memset (a, '\0', sizeof a);
  lower_bound_memcpy_into_empty ();

  memset (a, 'x', sizeof a - 1);
  a[sizeof a - 1] = '\0';
  lower_bound_memcpy_into_longest ();

  memset (a, 'x', sizeof a - 1);
  a[sizeof a - 1] = '\0';
  lower_bound_memcpy_into_longest ();

  memset (a, '\0', sizeof a);
  lower_bound_memcpy_memcpy_into_empty ();

  memset (a, 'x', sizeof a - 1);
  a[sizeof a - 1] = '\0';
  lower_bound_memcpy_memcpy_into_longest ();

  memove_forward_strlen ();

  memset (a, '\0', sizeof a);
  memove_backward_into_empty_strlen ();

  memset (a, 'x', sizeof a - 1);
  a[sizeof a - 1] = '\0';
  memove_backward_into_longest_strlen ();

  memove_strcmp ();
}
