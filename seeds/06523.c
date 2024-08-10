/* PR tree-optimization/92226 - live nul char store to array eliminated
   { dg-do compile }
   { dg-options "-O2 -fdump-tree-strlen" } */

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
    extern char a ## SIZE[SIZE];					\
    char *d = a ## SIZE;						\
    size_t len = strlen (s);						\
    size_t idx = IDX;							\
    if (MIN <= len && len <= MAX)					\
      {									\
	strcpy (d, s);							\
	d[idx] = 0;							\
	if (strlen (d) != idx)						\
	  abort ();							\
      }									\
  } typedef void dummy_type


/* The final nul store must be retained but the second strlen call should
   be eliminated because the final length of the destination after the nul
   store must be equal to the index of the store.  */
T (0, 2, 4, 0);

/* Not handled yet (see below):
   T (0, 2, 4, 1);  */

/* Not handled yet: in addition to the cases above, the second strlen
   call can also be eliminated in those below because in both the final
   length of the destination after the nul store must be in the same
   range as the length of the source.
   T (0, 2, 4, 2);
   T (0, 2, 4, 3);  */

T (2, 3, 4, 0);
T (2, 3, 4, 1);

/* Not handled yet (see above):
   T (2, 3, 4, 2);
   T (2, 3, 4, 3);  */

T (3, 4, 5, 0);
T (3, 4, 5, 1);
T (3, 4, 5, 2);

/* Not handled yet (see above):
   T (3, 4, 5, 3);
   T (3, 4, 5, 4);  */

T (3, 4, 6, 0);
T (3, 4, 6, 1);
T (3, 4, 6, 2);

/* Not handled yet (see above):
   T (3, 4, 6, 3);
   T (3, 4, 6, 4);
   T (3, 4, 6, 5);  */


/* Verify that each function makes just one call to strlen to compute
   the length of its argument (and not also to compute the length of
   the copy):
  { dg-final { scan-tree-dump-times "strlen \\(s_" 9 "strlen1" } }
  { dg-final { scan-tree-dump-not "strlen \\(\\&a" "strlen1" } }

  Verify that nul stores into the destination have not been eliminated:
  { dg-final { scan-tree-dump-times "a4\\\] = 0;" 2 "strlen1" } }
  { dg-final { scan-tree-dump-times "a4 \\\+ 1B\\\] = 0;" 1 "strlen1" } }

  { dg-final { scan-tree-dump-times "a5\\\] = 0;" 1 "strlen1" } }
  { dg-final { scan-tree-dump-times "a5 \\\+ 1B\\\] = 0;" 1 "strlen1" } }
  { dg-final { scan-tree-dump-times "a5 \\\+ 2B\\\] = 0;" 1 "strlen1" } }

  { dg-final { scan-tree-dump-times "a6\\\] = 0;" 1 "strlen1" } }
  { dg-final { scan-tree-dump-times "a6 \\\+ 1B\\\] = 0;" 1 "strlen1" } }
  { dg-final { scan-tree-dump-times "a6 \\\+ 2B\\\] = 0;" 1 "strlen1" } }  */
