/* PR tree-optimization/91183 - strlen of a strcpy result with a conditional
   source not folded
   Test to verify that strlen can determine string lengths from wider stores
   than narrow characters.  This matters because on targets that can handle
   unaligned stores and where GCC lowers multi-character stores into smaller
   numbers of wider stores.
   { dg-do compile }
   { dg-options "-O2 -fdump-tree-optimized" }
   On strictly aligned targets the consecutive char assignments used
   by the test aren't merged.  When they involve multiple trailing nuls
   these assignments then defeat the strlen optimization as a result of
   pr83821.  When the bug is resolved the directive below can be removed.
   { dg-require-effective-target non_strict_align } */

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

/* Macros to emit a call to function named
     call_failed_to_be_eliminated_on_line_NNN()
   for each call that's expected to be eliminated.  The dg-final
   scan-tree-dump-time directive at the bottom of the test verifies
   that no such call appears in output.  */
#define ELIM(expr)					\
  if ((expr)) FAIL (not_eliminated); else (void)0

#undef T
#define T(N, ncpy, expect, assign) do {				\
    char a[N], b[N];						\
    assign;							\
    memcpy (b, a, ncpy);					\
    ELIM (!(expect == strlen (b)));				\
  } while (0)

void test_copy (void)
{
  T (2, 1, 0, (a[0] = 0));
  T (2, 2, 0, (a[0] = 0, a[1] = 0));
  T (2, 2, 1, (a[0] = '1', a[1] = 0));
  T (4, 3, 2, (a[0] = '1', a[1] = '2', a[2] = 0));
  // Not handled due to pr83821:
  // T (4, 3, 1, (a[0] = '1', a[1] = 0, a[2] = '2'));
  T (4, 2, 1, (a[0] = '1', a[1] = 0,   a[2] = 0,   a[3] = 0));
  // Not handled due to pr83821:
  // T (4, 3, 1, (a[0] = '1', a[1] = 0,   a[2] = 0,   a[3] = 0));
  T (4, 4, 1, (a[0] = '1', a[1] = 0,   a[2] = 0,   a[3] = 0));
  T (4, 3, 2, (a[0] = '1', a[1] = '2', a[2] = 0,   a[3] = 0));
  T (4, 4, 2, (a[0] = '1', a[1] = '2', a[2] = 0,   a[3] = 0));
  T (4, 4, 3, (a[0] = '1', a[1] = '2', a[2] = '3', a[3] = 0));
  T (5, 4, 1, (a[0] = '1', a[1] = 0,   a[2] = 0,   a[3] = 0));
  T (5, 4, 2, (a[0] = '1', a[1] = '2', a[2] = 0,   a[3] = 0));
  T (5, 4, 3, (a[0] = '1', a[1] = '2', a[2] = '3', a[3] = 0));
  // Not handled:
  // T (5, 5, 1, (a[0] = '1', a[1] = 0,   a[2] = 0,   a[3] = 0,   a[4] = 0));
  // T (5, 5, 2, (a[0] = '1', a[1] = '2', a[2] = 0,   a[3] = 0,   a[4] = 0));
  // T (5, 5, 3, (a[0] = '1', a[1] = '2', a[2] = '3', a[3] = 0,   a[4] = 0));
  T (5, 5, 4, (a[0] = '1', a[1] = '2', a[2] = '3', a[3] = '4', a[4] = 0));
}


/* { dg-final { scan-tree-dump-times "strlen" 0 "optimized" } }
   { dg-final { scan-tree-dump-times "_not_eliminated_" 0 "optimized" } } */
