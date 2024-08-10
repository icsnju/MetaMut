/* PR tree-optimization/86042 - missing strlen optimization after second strcpy
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

#define CAT(x, y) x ## y
#define CONCAT(x, y) CAT (x, y)
#define FAILNAME(name) CONCAT (call_ ## name ##_on_line_, __LINE__)

#define FAIL(name) do {				\
    extern void FAILNAME (name) (void);		\
    FAILNAME (name)();				\
  } while (0)

/* Macro to emit a call to function named
   call_in_true_branch_not_eliminated_on_line_NNN()
   for each call that's expected to be eliminated.  The dg-final
   scan-tree-dump-time directive at the bottom of the test verifies
   that no such call appears in output.  */
#define ELIM(expr)							\
  if (!(expr)) FAIL (in_true_branch_not_eliminated); else (void)0

void elim_after_duplicate_strcpy (void)
{
#define T(N, assign, off, str, r0, r1)		\
  do {						\
    char a[N];					\
    strcpy (a, assign);				\
    unsigned n0 = strlen (a);			\
    strcpy (a + off, str);			\
    unsigned n1 = strlen (a);			\
    ELIM (n0 == r0 && n1 == r1);		\
  } while (0)

  T (2, "",   0, "1",   0, 1);

  T (2, "1",  0, "2",   1, 1);
  T (2, "1",  1, "",    1, 1);

  T (3, "\0", 0, "1",   0, 1);
  T (3, "1",  1, "2",   1, 2);

  T (3, "12", 0, "23",  2, 2);
  T (3, "12", 1, "3",   2, 2);
  T (3, "12", 2, "",    2, 2);

  T (4, "1",   1, "23",  1, 3);
  T (4, "12",  1, "23",  2, 3);

  T (4, "123", 0, "234", 3, 3);
  T (4, "123", 1, "34",  3, 3);
  T (4, "123", 2, "4",   3, 3);
  T (4, "123", 3, "",    3, 3);

  T (5, "1234", 0, "1",    4, 1);
  T (5, "1234", 0, "12",   4, 2);
  T (5, "1234", 0, "123",  4, 3);
  T (5, "1234", 0, "1234", 4, 4);

  T (5, "123",  1, "234", 3, 4);
  T (6, "123",  2, "234", 3, 5);
}

void elim_after_init_memcpy (void)
{
#undef T
#define T(init, off, str, n, res)		\
  do {						\
    char a[] = init;				\
    memcpy (a + off, str, n);			\
    ELIM (strlen (a) == res);			\
  } while (0)

  T ("\0",   0, "1",    2, 1);
  T ("\0\0", 0, "12",   3, 2);

#define INIT { '1', '2', '3', '4' }
  T (INIT,   0, "",     1, 0);
  T (INIT,   0, "1",    2, 1);
  T (INIT,   0, "12",   3, 2);
  T (INIT,   0, "123",  4, 3);

  T ("1234", 0, "2",    1, 4);
  T ("1234", 0, "23",   2, 4);
  T ("1234", 0, "234",  3, 4);
  T ("1234", 0, "2345", 4, 4);
  T ("1234", 0, "2345", 5, 4);

  T ("1234", 1, "2",    1, 4);
  T ("1234", 1, "23",   2, 4);
  T ("1234", 1, "234",  3, 4);
  T ("1234", 1, "234",  4, 4);

  T ("1234", 2, "3",    1, 4);
  T ("1234", 2, "3",    2, 3);
  T ("1234", 2, "34",   2, 4);
  T ("1234", 2, "34",   3, 4);

  T ("12\00034", 0, "1", 1, 2);
  T ("12\00034", 0, "1", 2, 1);

  T ("AB\000CD", 0, "ab", 2, 2);
  T ("AB\000CD", 0, "ab", 3, 2);
  T ("AB\000CD", 0, "ab\000c", 4, 2);
}

/* { dg-final { scan-tree-dump-times "strlen1" 0 "optimized" } }
   { dg-final { scan-tree-dump-times "call_in_true_branch_not_eliminated" 0 "optimized" } } */
