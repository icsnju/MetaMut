/* PR tree-optimization/91315 - missing strlen lower bound of a string
   known to be at least N characters
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
#define ASSERT_ELIM(expr)						\
  if (!!(expr)) FAIL (in_true_branch_not_eliminated); else (void)0

char a[32];

void lower_bound_assign_1 (void)
{
  a[0] = '1';
  ASSERT_ELIM (strlen (a) < 1);
}

void lower_bound_assign_2 (void)
{
  a[0] = '1';
  a[1] = '2';
  ASSERT_ELIM (strlen (a) < 2);
}

void lower_bound_assign_3 (void)
{
  a[0] = '1';
  a[1] = '2';
  a[2] = '3';
  ASSERT_ELIM (strlen (a) < 3);
}

void lower_bound_memcpy (void)
{
  memcpy (a, "123", 3);
  ASSERT_ELIM (strlen (a) < 3);
}

void lower_bound_memcpy_memcpy_2 (void)
{
  memcpy (a, "123", 3);
  memcpy (a + 2, "345", 3);
  ASSERT_ELIM (strlen (a) < 5);
}

void lower_bound_memcpy_memcpy_3 (void)
{
  memcpy (a, "123", 3);
  memcpy (a + 3, "456", 3);
  ASSERT_ELIM (strlen (a) < 6);
}

/* FIXME: Not optimized yet.
void lower_bound_stpcpy_stpcpy_assign (void)
{
  *stpcpy (strcpy (a, "123"), "4567") = '8';
  ASSERT_ELIM (strlen (a) < 8);
}
*/

void lower_bound_strcpy_strcat_assign (void)
{
  strcpy (a, "123");
  strcat (a, "45");
  a[5] = '6';
  ASSERT_ELIM (strlen (a) < 6);
}

/* { dg-final { scan-tree-dump-times "call_in_true_branch_not_eliminated_" 0 "optimized" } } */
