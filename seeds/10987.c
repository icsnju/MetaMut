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

__attribute__((noinline, noclone)) const char *
fn1 (int x, int y)
{
  const char *p;
  switch (x)
    {
    case 0:
      p = "abcd";
      /* Prevent cswitch optimization.  */
      asm volatile ("" : : : "memory");
      break;
    case 1:
      p = "efgh";
      break;
    case 2:
      p = "ijkl";
      break;
    default:
      p = "mnop";
      break;
    }
  if (y)
    /* strchr should be optimized into p + 4 here.  */
    return strchr (p, '\0');
  else
    /* and strlen into 3.  */
    return p + strlen (p + 1);
}

__attribute__((noinline, noclone)) size_t
fn2 (char *p, char *q)
{
  size_t l;
  /* Both strcpy calls can be optimized into memcpy, strlen needs to stay.  */
  strcpy (p, "abc");
  p[3] = 'd';
  l = strlen (p);
  strcpy (q, p);
  return l;
}

__attribute__((noinline, noclone)) char *
fn3 (char *p)
{
  char *c;
  /* The strcpy call can be optimized into memcpy, strchr needs to stay,
     strcat is optimized into memcpy.  */
  strcpy (p, "abc");
  p[3] = 'd';
  c = strchr (p, '\0');
  strcat (p, "efgh");
  return c;
}

int
main ()
{
  int i;
  char buf[64], buf2[64];
  for (i = 0; i < 5; i++)
    {
      const char *p = "abcdefghijklmnop" + (i < 3 ? i : 3) * 4;
      const char *q;
      q = fn1 (i, 1);
      if (memcmp (q - 4, p, 4) != 0 || q[0] != '\0')
	abort ();
      q = fn1 (i, 0);
      if (memcmp (q - 3, p, 4) != 0 || q[1] != '\0')
	abort ();
    }
  memset (buf, '\0', sizeof buf);
  memset (buf + 4, 'z', 2);
  if (fn2 (buf, buf2) != 6
      || memcmp (buf, "abcdzz", 7) != 0
      || memcmp (buf2, "abcdzz", 7) != 0)
    abort ();
  memset (buf, '\0', sizeof buf);
  memset (buf + 4, 'z', 2);
  if (fn3 (buf) != buf + 6 || memcmp (buf, "abcdzzefgh", 11) != 0)
    abort ();
  return 0;
}

/* { dg-final { scan-tree-dump-times "strlen \\(" 2 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "memcpy \\(" 4 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strcpy \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strcat \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "strchr \\(" 0 "strlen1" } } */
/* { dg-final { scan-tree-dump-times "stpcpy \\(" 0 "strlen1" } } */
