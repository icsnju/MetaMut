/* { dg-do run } */
/* { dg-options "-O2 -fdump-tree-strlen" } */

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

__attribute__((noinline, noclone)) void
fn1 (char *p, const char *z, size_t *lp)
{
  char *q, *r, *s;
  char buf[64];
  size_t l[11];
  /* The first strlen stays, all the strcpy calls can be optimized
     into memcpy and most other strlen calls and all strchr calls
     optimized away.  l[6] = strlen (r); and l[9] = strlen (r); need
     to stay, because we need to invalidate the knowledge about
     r strlen after strcpy (q, "jklmnopqrst").  */
  l[0] = strlen (z);
  strcpy (buf, z);
  strcpy (p, "abcde");
  q = strchr (p, '\0');
  strcpy (q, "efghi");
  r = strchr (q, '\0');
  strcpy (r, buf);
  l[1] = strlen (p);
  l[2] = strlen (q);
  l[3] = strlen (r);
  strcpy (q, "jklmnopqrst");
  l[4] = strlen (p);
  l[5] = strlen (q);
  l[6] = strlen (r);
  s = strchr (q, '\0');
  strcpy (s, buf);
  l[7] = strlen (p);
  l[8] = strlen (q);
  l[9] = strlen (r);
  l[10] = strlen (s);
  memcpy (lp, l, sizeof l);
}

int
main ()
{
  char buf[64];
  size_t l[11];
  const char *volatile z = "ABCDEFG";
  memset (buf, '\0', sizeof buf);
  fn1 (buf, z, l);
  if (memcmp (buf, "abcdejklmnopqrstABCDEFG", 24) != 0)
    abort ();
  if (l[0] != 7)
    abort ();
  if (l[1] != 17 || l[2] != 12 || l[3] != 7)
    abort ();
  if (l[4] != 16 || l[5] != 11 || l[6] != 6)
    abort ();
  if (l[7] != 23 || l[8] != 18 || l[9] != 13 || l[10] != 7)
    abort ();
  return 0;
}

/* { dg-final { scan-tree-dump-times "strlen \\(" 3 "strlen1" } } */
/* Some targets have BIGGEST_ALIGNMENT 8-bits, allowing fold_builtin_memory_op
   to expand the memcpy call at the end of fn1.  */
/* { dg-final { scan-tree-dump-times "memcpy \\(" 7 "strlen1" { target { ! no_alignment_constraints } } } } */
/* { dg-final { scan-tree-dump-times "memcpy \\(" 6 "strlen1" { target { no_alignment_constraints } } } } */
/* { dg-final { scan-tree-dump-times "strcpy \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strcat \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strchr \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "stpcpy \\(" 0 "strlen1" } } */
/* Where the memcpy is expanded, the assignemts to elements of l are
   propagated.  */
/* { dg-final { scan-tree-dump-times "  _\[0-9\]* = strlen \\(\[^\n\r\]*;\[\n\r\]*  l.0. = " 1 "strlen1" { target { ! no_alignment_constraints } } } } */
/* { dg-final { scan-tree-dump-times "  _\[0-9\]* = strlen \\(\[^\n\r\]*;\[\n\r\]*  l.6. = " 1 "strlen1" { target { ! no_alignment_constraints } } } } */
/* { dg-final { scan-tree-dump-times "  _\[0-9\]* = strlen \\(\[^\n\r\]*;\[\n\r\]*  l.9. = " 1 "strlen1" { target { ! no_alignment_constraints } } } } */
/* { dg-final { scan-tree-dump-times "  _\[0-9\]* = strlen \\(\[^\n\r\]*;" 3 "strlen1" { target { no_alignment_constraints } } } } */
