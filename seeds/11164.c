/* PR tree-optimization/86914 - wrong code with strlen() of poor-man's
   flexible array member plus offset
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

struct A0 { char i, a[0]; };
struct A1 { char i, a[1]; };
struct A9 { char i, a[9]; };
struct Ax { char i, a[]; };

extern int a[];

extern struct A0 a0;
extern struct A1 a1;
extern struct A9 a9;
extern struct Ax ax;

void test_var_flexarray_cst_off (void)
{
  /* Use arbitrary constants greater than 16 in case GCC ever starts
     unrolling strlen() calls with small array arguments.  */
  a[0] = 17 < strlen (a0.a + 1);        // { dg-warning "\\\[-Warray-bounds|-Wstringop-overread" }
  a[1] = 19 < strlen (a1.a + 1);        // { dg-warning "\\\[-Wstringop-overread" }
  a[2] = 23 < strlen (a9.a + 9);        // { dg-warning "\\\[-Wstringop-overread" }
  a[3] = 29 < strlen (ax.a + 3);
}

void test_ptr_flexarray_cst_off (struct A0 *p0, struct A1 *p1,
				 struct A9 *p9, struct Ax *px)
{
  a[0] = 17 < strlen (p0->a + 1);
  a[1] = 19 < strlen (p1->a + 1);
  a[2] = 23 < strlen (p9->a + 9);
  a[3] = 29 < strlen (px->a + 3);
}

void test_ptr_flexarray_var_off (struct A0 *p0, struct A1 *p1,
				 struct A9 *p9, struct Ax *px,
				 int i)
{
  a[0] = 17 < strlen (p0->a + i);
  a[1] = 19 < strlen (p1->a + i);
  a[2] = 23 < strlen (p9->a + i);
  a[3] = 29 < strlen (px->a + i);
}

/* { dg-final { scan-tree-dump-times "strlen" 12 "optimized" } } */
