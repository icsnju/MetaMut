/* PR tree-optimization/78450 - strlen(s) return value can be assumed
   to be less than the size of s
   { dg-do compile }
   { dg-options "-O2 -fdump-tree-optimized" } */

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

extern char ax[];

struct MemArray7 { char a7[7]; };
struct MemArray6 { char a6[6]; };
struct MemArray5 { char a5[5]; };
struct MemArray4 { char a4[4]; };
struct MemArray3 { char a3[3]; };
struct MemArray2 { char a2[2]; };
struct MemArray1 { char a1[1]; };
struct MemArray0 { int n; char a0[0]; };
struct MemArrayX { int n; char ax[]; };

struct MemArrays
{
  struct MemArray7 *ma7;
  struct MemArray6 *ma6;
  struct MemArray5 *ma5;
  struct MemArray4 *ma4;
  struct MemArray3 *ma3;
  struct MemArray2 *ma2;
  struct MemArray1 *ma1;
  struct MemArray0 *ma0;
  struct MemArrayX *max;
};

extern void if_stmt_on_line (int);
extern void else_stmt_on_line (int);

#define T(expr)								\
  (!!(expr) ? if_stmt_on_line (__LINE__) : else_stmt_on_line (__LINE__))

void test_memarray_lt (struct MemArrays *p)
{
  T (strlen (p->ma7->a7) < sizeof p->ma7->a7);
  T (strlen (p->ma6->a6) < sizeof p->ma6->a6);
  T (strlen (p->ma5->a5) < sizeof p->ma5->a5);
  T (strlen (p->ma4->a4) < sizeof p->ma4->a4);
  T (strlen (p->ma3->a3) < sizeof p->ma3->a3);
  T (strlen (p->ma2->a2) < sizeof p->ma2->a2);
  T (strlen (p->ma1->a1) < sizeof p->ma1->a1);

  T (strlen (p->ma0->a0) < 1);
  T (strlen (p->max->ax) < 1);
}

void test_memarray_eq (struct MemArrays *p)
{
  T (strlen (p->ma7->a7) == sizeof p->ma7->a7);
  T (strlen (p->ma6->a6) == sizeof p->ma6->a6);
  T (strlen (p->ma5->a5) == sizeof p->ma5->a5);
  T (strlen (p->ma4->a4) == sizeof p->ma4->a4);
  T (strlen (p->ma3->a3) == sizeof p->ma3->a3);
  T (strlen (p->ma2->a2) == sizeof p->ma2->a2);
  T (strlen (p->ma1->a1) == sizeof p->ma1->a1);

  T (strlen (p->ma0->a0) == 1);
  T (strlen (p->max->ax) == 1);
}

void test_memarray_gt (struct MemArrays *p)
{
  T (strlen (p->ma7->a7) > sizeof p->ma7->a7);
  T (strlen (p->ma6->a6) > sizeof p->ma6->a6);
  T (strlen (p->ma5->a5) > sizeof p->ma5->a5);
  T (strlen (p->ma4->a4) > sizeof p->ma4->a4);
  T (strlen (p->ma3->a3) > sizeof p->ma3->a3);
  T (strlen (p->ma2->a2) > sizeof p->ma2->a2);
  T (strlen (p->ma1->a1) > sizeof p->ma1->a1);

  T (strlen (p->ma0->a0) > 1);
  T (strlen (p->max->ax) > 1);
 }

/* Verify that no if or else statements have been eliminated.
   { dg-final { scan-tree-dump-times "if_stmt_on_line" 27 "optimized" } }
   { dg-final { scan-tree-dump-times "else_stmt_on_line" 27 "optimized" } }  */
