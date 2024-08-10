/* PRE tree-optimization/90626 - fold strcmp(a, b) == 0 to zero when
   one string length is exact and the other is unequal
   { dg-do compile }
   { dg-options "-O2 -Wall -Wno-string-compare -fdump-tree-optimized -ftrack-macro-expansion=0" } */

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
#define ELIM_IF_TRUE(expr)						\
  if (!(expr)) FAIL (in_true_branch_not_eliminated); else (void)0

/* Macro to emit a call to a function named
     call_made_in_{true,false}_branch_on_line_NNN()
   for each call that's expected to be retained.  The dg-final
   scan-tree-dump-time directive at the bottom of the test verifies
   that the expected number of both kinds of calls appears in output
   (a pair for each line with the invocation of the KEEP() macro.  */
#define TEST_KEEP(expr)				\
  if (expr)					\
    FAIL (made_in_true_branch);			\
  else						\
    FAIL (made_in_false_branch)

#define FOLD(init1, init2, arg1, arg2, bnd)	\
  do {							\
    char a[8], b[8];					\
    sink (a, b);					\
    memcpy (a, init1, sizeof init1 - 1);		\
    memcpy (b, init2, sizeof init2 - 1);		\
    ELIM_IF_TRUE (0 != CMPFUNC (arg1, arg2, bnd));	\
  } while (0)

#define KEEP(init1, init2, arg1, arg2, bnd)	\
  do {						\
    char a[8], b[8];				\
    sink (a, b);				\
    memcpy (a, init1, sizeof init1 - 1);	\
    memcpy (b, init2, sizeof init2 - 1);	\
    TEST_KEEP (0 == CMPFUNC (arg1, arg2, bnd));	\
  } while (0)

const char s0[1] = "";
const char s00[2] = "\0";
const char s10[2] = "1";
const char s20[2] = "2";

void sink (void*, ...);

void test_strcmp_elim (void)
{
#undef CMPFUNC
#define CMPFUNC(a, b, dummy) strcmp (a, b)

  FOLD (s00, s10, "\0", "1", -1);
  FOLD (s00, s10, "\0", b, -1);
  FOLD (s00, s10, "\0", s10, -1);

  FOLD (s00, s10, s0, "1", -1);
  FOLD (s00, s10, s0, b, -1);
  FOLD (s00, s10, s0, s10, -1);

  FOLD ("\0", "1", s0, "1", -1);
  FOLD ("\0", "1", s0, b, -1);
  FOLD ("\0", "1", s0, s10, -1);

  FOLD ("2",  "\0", "2", "\0", -1);
  FOLD ("2",  "\0", s20, s0, -1);

  FOLD ("\0", "1", a, b, -1);
  FOLD ("2",  "\0", a, b, -1);

  FOLD ("4\0", "44", a, b, -1);
  FOLD ("55", "5\0", a, b, -1);

  FOLD ("666\0", "6666", a, "6666", -1);
  FOLD ("666\0", "6666", a, b, -1);
  FOLD ("7777", "777\0", a, b, -1);

  /* Avoid testing substrings of equal length with different characters.
     The optimization doesn't have access to the contents of the strings
     so it can't determine whether they are equal.

     FOLD ("111\0", "112", a, b, -1);
     FOLD ("112", "111\0", a, b, -1);  */
}

const char s123[] = "123";
const char s1230[] = "123\0";

const char s1234[] = "1234";
const char s12340[] = "1234\0";

void test_strncmp_elim (void)
{
#undef CMPFUNC
#define CMPFUNC(a, b, n) strncmp (a, b, n)

  FOLD (s1230, s1234, "123",  "1234", 4);
  FOLD (s1234, s1230, "1234", "123",  4);

  FOLD (s1230, s1234, "123",  s1234, 4);
  FOLD (s1234, s1230, "1234", s123,  4);

  FOLD (s1230, s1234, s123,  "1234", 4);
  FOLD (s1234, s1230, s1234, "123",  4);

  FOLD (s1230, s1234, s123,  b, 4);
  FOLD (s1234, s1230, s1234, b, 4);

  FOLD (s1230, s1234, a, b, 4);
  FOLD (s1234, s1230, a, b, 4);

  FOLD ("123\0", "1234",  a, b, 5);
  FOLD ("1234",  "123\0", a, b, 5);
}


#line 1000

void test_strcmp_keep (const char *s, const char *t)
{
#undef CMPFUNC
#define CMPFUNC(a, b, dummy) strcmp (a, b)

  KEEP ("123", "123\0", a, b, /* bnd = */ -1);
  KEEP ("123\0", "123", a, b, -1);

  {
    char a[8], b[8];
    sink (a, b);
    strcpy (a, s);
    strcpy (b, t);
    TEST_KEEP (0 == strcmp (a, b));
  }
}


void test_strncmp_keep (const char *s, const char *t)
{
#undef CMPFUNC
#define CMPFUNC(a, b, n) strncmp (a, b, n)

  KEEP ("1", "1", a, b, 2);

  KEEP ("1\0", "1", a, b, 2);
  KEEP ("1",   "1\0", a, b, 2);

  KEEP ("12\0", "12", a, b, 2);
  KEEP ("12",   "12\0", a, b, 2);

  KEEP ("111\0", "111", a, b, 3);
  KEEP ("112", "112\0", a, b, 3);

  {
    char a[8], b[8];
    sink (a, b);
    strcpy (a, s);
    strcpy (b, t);
    TEST_KEEP (0 == strncmp (a, b, sizeof a));
  }
}

/* { dg-final { scan-tree-dump-times "call_in_true_branch_not_eliminated_" 0 "optimized" } }

   { dg-final { scan-tree-dump-times "call_made_in_true_branch_on_line_1\[0-9\]\[0-9\]\[0-9\]" 11 "optimized" } }
   { dg-final { scan-tree-dump-times "call_made_in_false_branch_on_line_1\[0-9\]\[0-9\]\[0-9\]" 11 "optimized" } } */
