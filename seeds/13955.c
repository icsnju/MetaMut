/* PR tree-optimization/92226 - live nul char store to array eliminated
   { dg-do run }
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

#define NOIPA __attribute__ ((noipa))

unsigned nfails;

char a[8];

void test (int line, const char *func, size_t expect)
{
  size_t len = strlen (a);
  if (len == expect)
    return;

  ++nfails;

  __builtin_printf ("assertion failed in %s on line %i: "
		    "strlen (\"%s\") == %zu, got %zu\n",
		    func, line, a, expect, len);
}

NOIPA const char* str (size_t n)
{
  return "9876543210" + 10 - n;
}

#define T(name, CMPEXP, LEN, IDX, EXPECT)	\
  NOIPA static void name (void)			\
  {						\
    const char *s = str (LEN);			\
    if (strlen (s) CMPEXP)			\
      {						\
	strcpy (a, s);				\
	a[IDX] = 0;				\
	test (__LINE__, #name, EXPECT);		\
      }						\
  } typedef void dummy_type


T (len_eq_1_store_nul_0, == 1, 1, 0, 0);
T (len_eq_1_store_nul_1, == 1, 1, 1, 1);
T (len_eq_1_store_nul_2, == 1, 1, 2, 1);
T (len_eq_1_store_nul_3, == 1, 1, 3, 1);
T (len_eq_1_store_nul_4, == 1, 1, 4, 1);

T (len_eq_2_store_nul_0, == 2, 2, 0, 0);
T (len_eq_2_store_nul_1, == 2, 2, 1, 1);
T (len_eq_2_store_nul_2, == 2, 2, 2, 2);
T (len_eq_2_store_nul_3, == 2, 2, 3, 2);
T (len_eq_2_store_nul_4, == 2, 2, 4, 2);

T (len_eq_3_store_nul_0, == 3, 3, 0, 0);
T (len_eq_3_store_nul_1, == 3, 3, 1, 1);
T (len_eq_3_store_nul_2, == 3, 3, 2, 2);
T (len_eq_3_store_nul_3, == 3, 3, 3, 3);
T (len_eq_3_store_nul_4, == 3, 3, 4, 3);


T (len_gt_1_store_nul_0, > 2, 2, 0, 0);
T (len_gt_1_store_nul_1, > 2, 2, 1, 1);
T (len_gt_1_store_nul_2, > 2, 2, 2, 2);
T (len_gt_1_store_nul_3, > 2, 2, 3, 2);
T (len_gt_1_store_nul_4, > 2, 2, 4, 2);

T (len_gt_2_store_nul_0, > 2, 3, 0, 0);
T (len_gt_2_store_nul_1, > 2, 3, 1, 1);
T (len_gt_2_store_nul_2, > 2, 3, 2, 2);
T (len_gt_2_store_nul_3, > 2, 3, 3, 3);
T (len_gt_2_store_nul_4, > 2, 3, 4, 3);

T (len_gt_3_store_nul_0, > 2, 4, 0, 0);
T (len_gt_3_store_nul_1, > 2, 4, 1, 1);
T (len_gt_3_store_nul_2, > 2, 4, 2, 2);
T (len_gt_3_store_nul_3, > 2, 4, 3, 3);
T (len_gt_3_store_nul_4, > 2, 4, 4, 4);


T (len_1_lt_4_store_nul_0, < 4, 1, 0, 0);
T (len_1_lt_4_store_nul_1, < 4, 1, 1, 1);
T (len_1_lt_4_store_nul_2, < 4, 1, 2, 1);
T (len_1_lt_4_store_nul_3, < 4, 1, 3, 1);
T (len_1_lt_4_store_nul_4, < 4, 1, 4, 1);
T (len_1_lt_4_store_nul_5, < 4, 1, 5, 1);
T (len_1_lt_4_store_nul_6, < 4, 1, 6, 1);
T (len_1_lt_4_store_nul_7, < 4, 1, 7, 1);

T (len_2_lt_4_store_nul_0, < 4, 2, 0, 0);
T (len_2_lt_4_store_nul_1, < 4, 2, 1, 1);
T (len_2_lt_4_store_nul_2, < 4, 2, 2, 2);
T (len_2_lt_4_store_nul_3, < 4, 2, 3, 2);
T (len_2_lt_4_store_nul_4, < 4, 2, 4, 2);
T (len_2_lt_4_store_nul_5, < 4, 2, 5, 2);
T (len_2_lt_4_store_nul_6, < 4, 2, 6, 2);
T (len_2_lt_4_store_nul_7, < 4, 2, 7, 2);

T (len_3_lt_4_store_nul_0, < 4, 3, 0, 0);
T (len_3_lt_4_store_nul_1, < 4, 3, 1, 1);
T (len_3_lt_4_store_nul_2, < 4, 3, 2, 2);
T (len_3_lt_4_store_nul_3, < 4, 3, 3, 3);
T (len_3_lt_4_store_nul_4, < 4, 3, 4, 3);
T (len_3_lt_4_store_nul_5, < 4, 3, 5, 3);
T (len_3_lt_4_store_nul_6, < 4, 3, 6, 3);
T (len_3_lt_4_store_nul_7, < 4, 3, 7, 3);

T (len_7_lt_8_store_nul_0, < 8, 7, 0, 0);
T (len_7_lt_8_store_nul_1, < 8, 7, 1, 1);
T (len_7_lt_8_store_nul_2, < 8, 7, 2, 2);
T (len_7_lt_8_store_nul_3, < 8, 7, 3, 3);
T (len_7_lt_8_store_nul_4, < 8, 7, 4, 4);
T (len_7_lt_8_store_nul_5, < 8, 7, 5, 5);
T (len_7_lt_8_store_nul_6, < 8, 7, 6, 6);
T (len_7_lt_8_store_nul_7, < 8, 7, 7, 7);


int main (void)
{
  len_eq_1_store_nul_0 ();
  len_eq_1_store_nul_1 ();
  len_eq_1_store_nul_2 ();
  len_eq_1_store_nul_3 ();
  len_eq_1_store_nul_4 ();

  len_eq_2_store_nul_0 ();
  len_eq_2_store_nul_1 ();
  len_eq_2_store_nul_2 ();
  len_eq_2_store_nul_3 ();
  len_eq_2_store_nul_4 ();

  len_eq_3_store_nul_0 ();
  len_eq_3_store_nul_1 ();
  len_eq_3_store_nul_2 ();
  len_eq_3_store_nul_3 ();
  len_eq_3_store_nul_4 ();


  len_gt_1_store_nul_0 ();
  len_gt_1_store_nul_1 ();
  len_gt_1_store_nul_2 ();
  len_gt_1_store_nul_3 ();
  len_gt_1_store_nul_4 ();

  len_gt_2_store_nul_0 ();
  len_gt_2_store_nul_1 ();
  len_gt_2_store_nul_2 ();
  len_gt_2_store_nul_3 ();
  len_gt_2_store_nul_4 ();

  len_gt_3_store_nul_0 ();
  len_gt_3_store_nul_1 ();
  len_gt_3_store_nul_2 ();
  len_gt_3_store_nul_3 ();
  len_gt_3_store_nul_4 ();

  len_1_lt_4_store_nul_0 ();
  len_1_lt_4_store_nul_1 ();
  len_1_lt_4_store_nul_2 ();
  len_1_lt_4_store_nul_3 ();
  len_1_lt_4_store_nul_4 ();
  len_1_lt_4_store_nul_5 ();
  len_1_lt_4_store_nul_6 ();
  len_1_lt_4_store_nul_7 ();

  len_2_lt_4_store_nul_0 ();
  len_2_lt_4_store_nul_1 ();
  len_2_lt_4_store_nul_2 ();
  len_2_lt_4_store_nul_3 ();
  len_2_lt_4_store_nul_4 ();
  len_2_lt_4_store_nul_5 ();
  len_2_lt_4_store_nul_6 ();
  len_2_lt_4_store_nul_7 ();

  len_3_lt_4_store_nul_0 ();
  len_3_lt_4_store_nul_1 ();
  len_3_lt_4_store_nul_2 ();
  len_3_lt_4_store_nul_3 ();
  len_3_lt_4_store_nul_4 ();
  len_3_lt_4_store_nul_5 ();
  len_3_lt_4_store_nul_6 ();
  len_3_lt_4_store_nul_7 ();

  len_7_lt_8_store_nul_0 ();
  len_7_lt_8_store_nul_1 ();
  len_7_lt_8_store_nul_2 ();
  len_7_lt_8_store_nul_3 ();
  len_7_lt_8_store_nul_4 ();
  len_7_lt_8_store_nul_5 ();
  len_7_lt_8_store_nul_6 ();
  len_7_lt_8_store_nul_7 ();

  if (nfails)
    abort ();
}
