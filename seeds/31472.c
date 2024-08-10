/* PR tree-optimization/83821 - local aggregate initialization defeats
   strlen optimization

   Avoid exercising targets other than x86_64 in LP64 mode due to PR 83543
   - strlen of a local array member not optimized on some targets
   { dg-do compile { target { { i?86-*-* x86_64-*-* } && lp64 } } }

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
#define ELIM(expr) \
  if (!(expr)) FAIL (in_true_branch_not_eliminated); else (void)0

/* Macro to emit a call to a function named
     call_made_in_{true,false}_branch_on_line_NNN()
   for each call that's expected to be retained.  The dg-final
   scan-tree-dump-time directive at the bottom of the test verifies
   that the expected number of both kinds of calls appears in output
   (a pair for each line with the invocation of the KEEP() macro.  */
#define KEEP(expr)				\
  if (expr)					\
    FAIL (made_in_true_branch);			\
  else						\
    FAIL (made_in_false_branch)

#define STR10 "0123456789"
#define STR20 STR10 STR10
#define STR30 STR20 STR10
#define STR40 STR20 STR20

void elim_char_array_init_consecutive (void)
{
  char a[][10] = { "1", "12", "123", "1234", "12345", "12345" };

  ELIM (strlen (a[0]) == 1);
  ELIM (strlen (a[1]) == 2);
  ELIM (strlen (a[2]) == 3);
  ELIM (strlen (a[3]) == 4);
  ELIM (strlen (a[4]) == 5);
}

void elim_char_array_cpy_consecutive (void)
{
  char a[5][10];

  strcpy (a[0], "12345");
  strcpy (a[1], "1234");
  strcpy (a[2], "123");
  strcpy (a[3], "12");
  strcpy (a[4], "1");

  ELIM (strlen (a[0]) == 5);
  ELIM (strlen (a[1]) == 4);
  ELIM (strlen (a[2]) == 3);
  ELIM (strlen (a[3]) == 2);
  ELIM (strlen (a[4]) == 1);
}

void elim_clear_char_array_cpy_consecutive (void)
{
  char a[5][10] = { };

  strcpy (a[0], "12345");
  strcpy (a[1], "1234");
  strcpy (a[2], "123");
  strcpy (a[3], "12");
  strcpy (a[4], "1");

  ELIM (strlen (a[0]) == 5);
  ELIM (strlen (a[1]) == 4);
  ELIM (strlen (a[2]) == 3);
  ELIM (strlen (a[3]) == 2);
  ELIM (strlen (a[4]) == 1);
}

struct Consec
{
  char s1[sizeof STR40];
  char s2[sizeof STR40];
  const char *p1;
  const char *p2;
};

void elim_struct_init_consecutive (void)
{
  struct Consec a = { STR10, STR10, STR10, STR10 };

  ELIM (strlen (a.s1) == sizeof STR10 - 1);
  ELIM (strlen (a.s2) == sizeof STR10 - 1);
  ELIM (strlen (a.p1) == sizeof STR10 - 1);
  ELIM (strlen (a.p2) == sizeof STR10 - 1);
}

void elim_struct_array_init_consecutive (void)
{
  struct Consec a[2] = {
    { STR10, STR20, STR30, STR40 },
    { STR40, STR30, STR20, STR10 }
  };

  ELIM (strlen (a[0].s1) == sizeof STR10 - 1);
  ELIM (strlen (a[0].s2) == sizeof STR20 - 1);
  ELIM (strlen (a[0].p1) == sizeof STR30 - 1);
  ELIM (strlen (a[0].p2) == sizeof STR40 - 1);

  ELIM (strlen (a[1].s1) == sizeof STR40 - 1);
  ELIM (strlen (a[1].s2) == sizeof STR30 - 1);
  ELIM (strlen (a[1].p1) == sizeof STR20 - 1);
  ELIM (strlen (a[1].p2) == sizeof STR10 - 1);
}

struct NonConsec
{
  char s1[sizeof STR40];
  int i1;
  char s2[sizeof STR40];
  int i2;
  const char *p1;
  int i3;
  const char *p2;
  int i4;
};

void elim_struct_init_nonconsecutive (void)
{
  struct NonConsec b = { STR10, 123, STR20, 456, b.s1, 789, b.s2, 123 };

  ELIM (strlen (b.s1) == sizeof STR10 - 1);
  ELIM (strlen (b.s2) == sizeof STR20 - 1);
  ELIM (strlen (b.p1) == sizeof STR10 - 1);
  ELIM (strlen (b.p2) == sizeof STR20 - 1);
}

void elim_struct_assign_tmp_nonconsecutive (void)
{
  struct NonConsec b = { "a", 1, "b", 2, "c", 3, "d", 4 };

  b = (struct NonConsec){ STR10, 123, STR20, 456, STR30, 789, STR40, 123 };

  ELIM (strlen (b.s1) == sizeof STR10 - 1);
  ELIM (strlen (b.s2) == sizeof STR20 - 1);
  ELIM (strlen (b.p1) == sizeof STR30 - 1);
  ELIM (strlen (b.p2) == sizeof STR40 - 1);
}

const struct NonConsec bcst = {
  STR40, -1, STR30, -2, STR20, -3, STR10, -4
};

void elim_struct_assign_cst_nonconsecutive (void)
{
  struct NonConsec b = { "a", 1, "b", 2, "c", 3, "d" };

  b = bcst;

  ELIM (strlen (b.s1) == sizeof STR40 - 1);
  ELIM (strlen (b.s2) == sizeof STR30 - 1);
  ELIM (strlen (b.p1) == sizeof STR20 - 1);
  ELIM (strlen (b.p2) == sizeof STR10 - 1);
}

void elim_struct_copy_cst_nonconsecutive (void)
{
  struct NonConsec b = { "a", 1, "b", 2, "c", 3, "d" };
  memcpy (&b, &bcst, sizeof b);

  /* ELIM (strlen (b.s1) == sizeof STR40 - 1);
     ELIM (strlen (b.s2) == sizeof STR30 - 1); */
  ELIM (strlen (b.p1) == sizeof STR20 - 1);
  ELIM (strlen (b.p2) == sizeof STR10 - 1);
}


#line 1000

int sink (void*);

void keep_init_nonconsecutive (void)
{
  struct NonConsec b = {
    STR10, 123, STR20, 456, b.s1, 789, b.s2,
    sink (&b)
  };

  KEEP (strlen (b.s1) == sizeof STR10 - 1);
  KEEP (strlen (b.s2) == sizeof STR10 - 1);
  KEEP (strlen (b.p1) == sizeof STR10 - 1);
  KEEP (strlen (b.p2) == sizeof STR10 - 1);
}

void keep_assign_tmp_nonconsecutive (void)
{
  struct NonConsec b = { "a", 1, "b", 2, "c", 3, "d", 4 };

  b = (struct NonConsec){
    STR10, 123, STR20, 456, STR30, 789, STR40,
    sink (&b)
  };

  KEEP (strlen (b.s1) == sizeof STR10 - 1);
  KEEP (strlen (b.s2) == sizeof STR20 - 1);
  KEEP (strlen (b.p1) == sizeof STR30 - 1);
  KEEP (strlen (b.p2) == sizeof STR40 - 1);
}

/* { dg-final { scan-tree-dump-times "call_in_true_branch_not_eliminated_" 0 "optimized" } }

   { dg-final { scan-tree-dump-times "call_made_in_true_branch_on_line_1\[0-9\]\[0-9\]\[0-9\]" 8 "optimized" } }
   { dg-final { scan-tree-dump-times "call_made_in_false_branch_on_line_1\[0-9\]\[0-9\]\[0-9\]" 8 "optimized" } } */
