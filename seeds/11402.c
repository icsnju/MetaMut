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

/* Verify that the nul store  into the destination is only eliminated
   when overwrites the existing terminating nul added by the strcpy call.
   Also verify that the second strlen call is eliminated in all cases.  */
#define T(SIZE, IDX)							\
  NOIPA void test_ ## SIZE ## _store_nul_ ## IDX (const char *s)	\
  {									\
    extern char a ## SIZE[SIZE];					\
    char *d = a ## SIZE;						\
    size_t len = SIZE - 1;						\
    size_t idx = IDX;							\
    if (strlen (s) == len)						\
      {									\
	strcpy (d, s);							\
	d[idx] = 0;							\
	if (strlen (d) != idx)						\
	  abort ();							\
      }									\
  } typedef void dummy_type


T (1, 0);   // expect nul store to be eliminated

T (2, 0);   // nul store must be retained
T (2, 1);   // expect nul store to be eliminated

// Same as above but for larger arrays.
T (3, 0);
T (3, 1);
T (3, 2);

T (4, 0);
T (4, 1);
T (4, 2);
T (4, 3);

T (5, 0);
T (5, 1);
T (5, 2);
T (5, 3);
T (5, 4);

T (6, 0);
T (6, 1);
T (6, 2);
T (6, 3);
T (6, 4);
T (6, 5);

T (7, 0);
T (7, 1);
T (7, 2);
T (7, 3);
T (7, 4);
T (7, 5);
T (7, 6);

T (8, 0);
T (8, 1);
T (8, 2);
T (8, 3);
T (8, 4);
T (8, 5);
T (8, 6);
T (8, 7);

/* Verify that each function makes just one call to strlen to compute
   the length of its argument (and not also to compute the length of
   the copy):
  { dg-final { scan-tree-dump-times "strlen \\(s_" 36 "strlen1" } }
  { dg-final { scan-tree-dump-not "strlen \\(\\&a" "strlen1" } }

  Verify that nul stores into the last array element have been eliminated
  (they are preceded by a strcpy storing into all the elements of the array:
  { dg-final { scan-tree-dump-not "a1\\\] = 0;" "strlen1" } }
  { dg-final { scan-tree-dump-not "a2 \\\+ 1B\\\] = 0;" "strlen1" } }
  { dg-final { scan-tree-dump-not "a3 \\\+ 2B\\\] = 0;" "strlen1" } }
  { dg-final { scan-tree-dump-not "a4 \\\+ 3B\\\] = 0;" "strlen1" } }
  { dg-final { scan-tree-dump-not "a5 \\\+ 4B\\\] = 0;" "strlen1" } }
  { dg-final { scan-tree-dump-not "a6 \\\+ 5B\\\] = 0;" "strlen1" } }
  { dg-final { scan-tree-dump-not "a7 \\\+ 6B\\\] = 0;" "strlen1" } }
  { dg-final { scan-tree-dump-not "a8 \\\+ 7B\\\] = 0;" "strlen1" } } */
