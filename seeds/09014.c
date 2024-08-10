/* PR tree-optimization/83431 - Verify that snprintf (0, 0, "%s",
   with an argument that's a conditional expression evaluates to
   the expected result regardless of the order of the expression
   operands.
   { dg-do run }
   { dg-skip-if "UNIX 2003 return behavior not supported" { hppa*-*-hpux* } }
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

#define A(expr)                                                 \
  ((expr)                                                       \
   ? (void)0                                                    \
   : (__builtin_printf ("assertion failed on line %i: %s\n",    \
                        __LINE__, #expr),                       \
      __builtin_abort ()))

const char gs0[] = "";
const char gs3[] = "123";

char gc;
char ga5[7];

struct S { char n, ma7[7], max[]; };


__attribute__ ((noclone, noinline, noipa)) void
equal_4_gs0_gs3_ga5_m1 (int i)
{
  strcpy (ga5, "1234");
  const char *p = i < 0 ? gs0 : 0 < i ? gs3 : ga5;

  A (snprintf (0, 0, "%s", p) == 0);
}

__attribute__ ((noclone, noinline, noipa)) void
equal_4_gs0_gs3_ga5_0 (int i)
{
  strcpy (ga5, "1234");
  const char *p = i < 0 ? gs0 : 0 < i ? gs3 : ga5;

  A (snprintf (0, 0, "%s", p) == 4);
}

__attribute__ ((noclone, noinline, noipa)) void
equal_4_gs0_gs3_ga5_p1 (int i)
{
  strcpy (ga5, "1234");
  const char *p = i < 0 ? gs0 : 0 < i ? gs3 : ga5;

  A (snprintf (0, 0, "%s", p) == 3);
}


__attribute__ ((noclone, noinline, noipa)) void
equal_4_gs0_ga5_gs3_m1 (int i)
{
  strcpy (ga5, "1234");
  const char *p = i < 0 ? gs0 : 0 < i ? ga5 : gs3;

  A (snprintf (0, 0, "%s", p) == 0);
}

__attribute__ ((noclone, noinline, noipa)) void
equal_4_gs0_ga5_gs3_0 (int i)
{
  strcpy (ga5, "1234");
  const char *p = i < 0 ? gs0 : 0 < i ? ga5 : gs3;

  A (snprintf (0, 0, "%s", p) == 3);
}

__attribute__ ((noclone, noinline, noipa)) void
equal_4_gs0_ga5_gs3_p1 (int i)
{
  strcpy (ga5, "1234");
  const char *p = i < 0 ? gs0 : 0 < i ? ga5 : gs3;

  A (snprintf (0, 0, "%s", p) == 4);
}


__attribute__ ((noclone, noinline, noipa)) void
equal_4_ga5_gs0_gs3_m1 (int i)
{
  strcpy (ga5, "1234");
  const char *p = i < 0 ? ga5 : 0 < i ? gs0 : gs3;

  A (snprintf (0, 0, "%s", p) == 4);
}

__attribute__ ((noclone, noinline, noipa)) void
equal_4_ga5_gs0_gs3_0 (int i)
{
  strcpy (ga5, "1234");
  const char *p = i < 0 ? ga5 : 0 < i ? gs0 : gs3;

  A (snprintf (0, 0, "%s", p) == 3);
}

__attribute__ ((noclone, noinline, noipa)) void
equal_4_ga5_gs0_gs3_p1 (int i)
{
  strcpy (ga5, "1234");
  const char *p = i < 0 ? ga5 : 0 < i ? gs0 : gs3;

  A (snprintf (0, 0, "%s", p) == 0);
}


__attribute__ ((noclone, noinline, noipa)) void
equal_4_ga5_gs3_gs0_m1 (int i)
{
  strcpy (ga5, "1234");
  const char *p = i < 0 ? ga5 : 0 < i ? gs3 : gs0;

  A (snprintf (0, 0, "%s", p) == 4);
}

__attribute__ ((noclone, noinline, noipa)) void
equal_4_ga5_gs3_gs0_0 (int i)
{
  strcpy (ga5, "1234");
  const char *p = i < 0 ? ga5 : 0 < i ? gs3 : gs0;

  A (snprintf (0, 0, "%s", p) == 0);
}

__attribute__ ((noclone, noinline, noipa)) void
equal_4_ga5_gs3_gs0_p1 (int i)
{
  strcpy (ga5, "1234");
  const char *p = i < 0 ? ga5 : 0 < i ? gs3 : gs0;

  A (snprintf (0, 0, "%s", p) == 3);
}


__attribute__ ((noclone, noinline, noipa)) void
equal_4_gs3_gs0_ga5_m1 (int i)
{
  strcpy (ga5, "1234");
  const char *p = i < 0 ? gs3 : 0 < i ? gs0 : ga5;

  A (snprintf (0, 0, "%s", p) == 3);
}

__attribute__ ((noclone, noinline, noipa)) void
equal_4_gs3_gs0_ga5_0 (int i)
{
  strcpy (ga5, "1234");
  const char *p = i < 0 ? gs3 : 0 < i ? gs0 : ga5;

  A (snprintf (0, 0, "%s", p) == 4);
}

__attribute__ ((noclone, noinline, noipa)) void
equal_4_gs3_gs0_ga5_p1 (int i)
{
  strcpy (ga5, "1234");
  const char *p = i < 0 ? gs3 : 0 < i ? gs0 : ga5;

  A (snprintf (0, 0, "%s", p) == 0);
}


/* Similar to the above but with memcpy creating a string at least
   four characters long, and the address of the NUL character.  */

__attribute__ ((noclone, noinline, noipa)) void
min_4_gc_gs3_ga5_m1 (int i)
{
  gc = 0;
  memcpy (ga5, "1234", 4);
  const char *p = i < 0 ? &gc : 0 < i ? gs3 : ga5;

  A (snprintf (0, 0, "%s", p) == 0);
}

__attribute__ ((noclone, noinline, noipa)) void
min_4_gc_gs3_ga5_0 (int i)
{
  gc = 0;
  memcpy (ga5, "1234", 4);
  const char *p = i < 0 ? &gc : 0 < i ? gs3 : ga5;

  A (snprintf (0, 0, "%s", p) == 4);
}

__attribute__ ((noclone, noinline, noipa)) void
min_4_gc_gs3_ga5_p1 (int i)
{
  gc = 0;
  memcpy (ga5, "1234", 4);
  const char *p = i < 0 ? &gc : 0 < i ? gs3 : ga5;

  A (snprintf (0, 0, "%s", p) == 3);
}


__attribute__ ((noclone, noinline, noipa)) void
min_4_gc_ga5_gs3_m1 (int i)
{
  gc = 0;
  memcpy (ga5, "1234", 4);
  const char *p = i < 0 ? &gc : 0 < i ? ga5 : gs3;

  A (snprintf (0, 0, "%s", p) == 0);
}

__attribute__ ((noclone, noinline, noipa)) void
min_4_gc_ga5_gs3_0 (int i)
{
  gc = 0;
  memcpy (ga5, "1234", 4);
  const char *p = i < 0 ? &gc : 0 < i ? ga5 : gs3;

  A (snprintf (0, 0, "%s", p) == 3);
}

__attribute__ ((noclone, noinline, noipa)) void
min_4_gc_ga5_gs3_p1 (int i)
{
  gc = 0;
  memcpy (ga5, "1234", 4);
  const char *p = i < 0 ? &gc : 0 < i ? ga5 : gs3;

  A (snprintf (0, 0, "%s", p) == 4);
}


__attribute__ ((noclone, noinline, noipa)) void
min_4_ga5_gc_gs3_m1 (int i)
{
  gc = 0;
  memcpy (ga5, "1234", 4);
  const char *p = i < 0 ? ga5 : 0 < i ? &gc : gs3;

  A (snprintf (0, 0, "%s", p) == 4);
}

__attribute__ ((noclone, noinline, noipa)) void
min_4_ga5_gc_gs3_0 (int i)
{
  gc = 0;
  memcpy (ga5, "1234", 4);
  const char *p = i < 0 ? ga5 : 0 < i ? &gc : gs3;

  A (snprintf (0, 0, "%s", p) == 3);
}

__attribute__ ((noclone, noinline, noipa)) void
min_4_ga5_gc_gs3_p1 (int i)
{
  gc = 0;
  memcpy (ga5, "1234", 4);
  const char *p = i < 0 ? ga5 : 0 < i ? &gc : gs3;

  A (snprintf (0, 0, "%s", p) == 0);
}


__attribute__ ((noclone, noinline, noipa)) void
min_4_ga5_gs3_gc_m1 (int i)
{
  gc = 0;
  memcpy (ga5, "1234", 4);
  const char *p = i < 0 ? ga5 : 0 < i ? gs3 : &gc;

  A (snprintf (0, 0, "%s", p) == 4);
}

__attribute__ ((noclone, noinline, noipa)) void
min_4_ga5_gs3_gc_0 (int i)
{
  gc = 0;
  memcpy (ga5, "1234", 4);
  const char *p = i < 0 ? ga5 : 0 < i ? gs3 : &gc;

  A (snprintf (0, 0, "%s", p) == 0);
}

__attribute__ ((noclone, noinline, noipa)) void
min_4_ga5_gs3_gc_p1 (int i)
{
  gc = 0;
  memcpy (ga5, "1234", 4);
  const char *p = i < 0 ? ga5 : 0 < i ? gs3 : &gc;

  A (snprintf (0, 0, "%s", p) == 3);
}


__attribute__ ((noclone, noinline, noipa)) void
min_4_gs3_gc_ga5_m1 (int i)
{
  gc = 0;
  memcpy (ga5, "1234", 4);
  const char *p = i < 0 ? gs3 : 0 < i ? &gc : ga5;

  A (snprintf (0, 0, "%s", p) == 3);
}

__attribute__ ((noclone, noinline, noipa)) void
min_4_gs3_gc_ga5_0 (int i)
{
  gc = 0;
  memcpy (ga5, "1234", 4);
  const char *p = i < 0 ? gs3 : 0 < i ? &gc : ga5;

  A (snprintf (0, 0, "%s", p) == 4);
}

__attribute__ ((noclone, noinline, noipa)) void
min_4_gs3_gc_ga5_p1 (int i)
{
  gc = 0;
  memcpy (ga5, "1234", 4);
  const char *p = i < 0 ? gs3 : 0 < i ? &gc : ga5;

  A (snprintf (0, 0, "%s", p) == 0);
}


int main (void)
{
  equal_4_gs0_gs3_ga5_m1 (-1);
  equal_4_gs0_gs3_ga5_0  ( 0);
  equal_4_gs0_gs3_ga5_p1 (+1);

  equal_4_gs0_ga5_gs3_m1 (-1);
  equal_4_gs0_ga5_gs3_0  ( 0);
  equal_4_gs0_ga5_gs3_p1 (+1);

  equal_4_ga5_gs0_gs3_m1 (-1);
  equal_4_ga5_gs0_gs3_0  ( 0);
  equal_4_ga5_gs0_gs3_p1 (+1);

  equal_4_ga5_gs3_gs0_m1 (-1);
  equal_4_ga5_gs3_gs0_0  ( 0);
  equal_4_ga5_gs3_gs0_p1 (+1);

  equal_4_gs3_gs0_ga5_m1 (-1);
  equal_4_gs3_gs0_ga5_0  ( 0);
  equal_4_gs3_gs0_ga5_p1 (+1);

  /* Same as aabove but with memcpy creating a string at least four
     characters long.  */
  memset (ga5, 0, sizeof ga5);
  min_4_gc_gs3_ga5_m1 (-1);
  memset (ga5, 0, sizeof ga5);
  min_4_gc_gs3_ga5_0  ( 0);
  memset (ga5, 0, sizeof ga5);
  min_4_gc_gs3_ga5_p1 (+1);

  memset (ga5, 0, sizeof ga5);
  min_4_gc_ga5_gs3_m1 (-1);
  memset (ga5, 0, sizeof ga5);
  min_4_gc_ga5_gs3_0  ( 0);
  memset (ga5, 0, sizeof ga5);
  min_4_gc_ga5_gs3_p1 (+1);

  memset (ga5, 0, sizeof ga5);
  min_4_ga5_gc_gs3_m1 (-1);
  memset (ga5, 0, sizeof ga5);
  min_4_ga5_gc_gs3_0  ( 0);
  memset (ga5, 0, sizeof ga5);
  min_4_ga5_gc_gs3_p1 (+1);

  memset (ga5, 0, sizeof ga5);
  min_4_ga5_gs3_gc_m1 (-1);
  memset (ga5, 0, sizeof ga5);
  min_4_ga5_gs3_gc_0  ( 0);
  memset (ga5, 0, sizeof ga5);
  min_4_ga5_gs3_gc_p1 (+1);

  memset (ga5, 0, sizeof ga5);
  min_4_gs3_gc_ga5_m1 (-1);
  memset (ga5, 0, sizeof ga5);
  min_4_gs3_gc_ga5_0  ( 0);
  memset (ga5, 0, sizeof ga5);
  min_4_gs3_gc_ga5_p1 (+1);
}
