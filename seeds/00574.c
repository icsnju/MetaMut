/* PR tree-optimization/77357 - strlen of constant strings not folded
   { dg-do compile }
   { dg-options "-O2 -Wall -fdump-tree-gimple -fdump-tree-ccp" } */

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

#define CONCAT(x, y) x ## y
#define CAT(x, y) CONCAT (x, y)
#define FAILNAME(name) CAT (call_ ## name ##_on_line_, __LINE__)

#define FAIL(name) do {				\
    extern void FAILNAME (name) (void);		\
    FAILNAME (name)();				\
  } while (0)

/* Macro to emit a call to funcation named
     call_in_true_branch_not_eliminated_on_line_NNN()
   for each call that's expected to be eliminated.  The dg-final
   scan-tree-dump-time directive at the bottom of the test verifies
   that no such call appears in output.  */
#define ELIM(expr) \
  if (!(expr)) FAIL (in_true_branch_not_eliminated); else (void)0

#define T(s, n) ELIM (strlen (s) == n)


struct S
{
  char a1[1], a2[2], a3[3], a4[4], a5[5], a6[6], a7[7], a8[8], a9[9];
};

#define S0 ""
#define S1 "1"
#define S2 "12"
#define S3 "123"
#define S4 "1234"
#define S5 "12345"
#define S6 "123456"
#define S7 "1234567"
#define S8 "12345678"

const char a9[][9] = { S0, S1, S2, S3, S4, S5, S6, S7, S8 };

const char a_1_9[1][9] = { S8 };
const char a_2_9[2][9] = { S8, S7};

const char a9_9[][9][9] = {
  { S0, S0, S0, S0, S0, S0, S0, S0, S0 },
  { S0, S1, S1, S1, S1, S1, S1, S1, S1 },
  { S0, S1, S2, S2, S2, S2, S2, S2, S2 },
  { S0, S1, S2, S3, S3, S3, S3, S3, S3 },
  { S0, S1, S2, S3, S4, S4, S4, S4, S4 },
  { S0, S1, S2, S3, S4, S5, S5, S5, S5 },
  { S0, S1, S2, S3, S4, S5, S6, S6, S6 },
  { S0, S1, S2, S3, S4, S5, S6, S7, S7 },
  { S0, S1, S2, S3, S4, S5, S6, S7, S8 }
};

const struct S s = { S0, S1, S2, S3, S4, S5, S6, S7, S8 };

const struct S sa[9] = {
  { S0, S0, S0, S0, S0, S0, S0, S0, S0 },
  { S0, S1, S1, S1, S1, S1, S1, S1, S1 },
  { S0, S1, S2, S2, S2, S2, S2, S2, S2 },
  { S0, S1, S2, S3, S3, S3, S3, S3, S3 },
  { S0, S1, S2, S3, S4, S4, S4, S4, S4 },
  { S0, S1, S2, S3, S4, S5, S5, S5, S5 },
  { S0, S1, S2, S3, S4, S5, S6, S6, S6 },
  { S0, S1, S2, S3, S4, S5, S6, S7, S7 },
  { S0, S1, S2, S3, S4, S5, S6, S7, S8 }
};

const struct S sa3_5_7[3][5][7] = {
  [1][2][3].a2 = S1, [1][3][5].a3 = S2, [2][4][5].a4 = S3
};


void test_global_array (void)
{
  T (a9[0], 0);  T (a9[0] + 0, 0);  T (a9[0] + 0, 0);  T (a9[0] + 0, 0);
  T (a9[1], 1);  T (a9[1] + 1, 0);  T (a9[1] + 1, 0);  T (a9[1] + 1, 0);
  T (a9[2], 2);
  T (a9[2] + 1, 1);
  T (a9[2] + 2, 0);
  T (a9[2] + 2, 0);

  T (a9[3], 3);  T (a9[3] + 1, 2);  T (a9[3] + 2, 1);  T (a9[3] + 3, 0);
  T (a9[4], 4);  T (a9[4] + 1, 3);  T (a9[4] + 2, 2);  T (a9[4] + 3, 1);
  T (a9[5], 5);  T (a9[5] + 1, 4);  T (a9[5] + 2, 3);  T (a9[5] + 3, 2);
  T (a9[6], 6);  T (a9[6] + 1, 5);  T (a9[6] + 2, 4);  T (a9[6] + 3, 3);
  T (a9[7], 7);  T (a9[7] + 1, 6);  T (a9[7] + 2, 5);  T (a9[7] + 3, 4);
  T (a9[8], 8);  T (a9[8] + 1, 7);  T (a9[8] + 2, 6);  T (a9[8] + 3, 5);

  T (a_1_9[0], 8);
  T (a_1_9[0] + 1, 7);
  T (a_1_9[0] + 7, 1);
  T (a_1_9[0] + 8, 0);

  T (a_2_9[0], 8);
  T (a_2_9[0] + 1, 7);
  T (a_2_9[0] + 7, 1);
  T (a_2_9[0] + 8, 0);

  T (a_2_9[1], 7);
  T (a_2_9[1] + 1, 6);
  T (a_2_9[1] + 6, 1);
  T (a_2_9[1] + 7, 0);
  T (a_2_9[1] + 8, 0);
}

void test_global_array_array (void)
{
  T (a9_9[0][0], 0);  T (a9_9[1][0], 0);  T (a9_9[2][0], 0);
  T (a9_9[0][1], 0);  T (a9_9[1][1], 1);  T (a9_9[2][1], 1);
  T (a9_9[0][2], 0);  T (a9_9[1][2], 1);  T (a9_9[2][2], 2);
  T (a9_9[0][3], 0);  T (a9_9[1][3], 1);  T (a9_9[2][3], 2);
  T (a9_9[0][4], 0);  T (a9_9[1][4], 1);  T (a9_9[2][4], 2);
  T (a9_9[0][5], 0);  T (a9_9[1][5], 1);  T (a9_9[2][5], 2);
  T (a9_9[0][6], 0);  T (a9_9[1][6], 1);  T (a9_9[2][6], 2);
  T (a9_9[0][7], 0);  T (a9_9[1][7], 1);  T (a9_9[2][7], 2);
  T (a9_9[0][8], 0);  T (a9_9[1][8], 1);  T (a9_9[2][8], 2);

  T (a9_9[3][0], 0);  T (a9_9[4][0], 0);  T (a9_9[5][0], 0);
  T (a9_9[3][1], 1);  T (a9_9[4][1], 1);  T (a9_9[5][1], 1);
  T (a9_9[3][2], 2);  T (a9_9[4][2], 2);  T (a9_9[5][2], 2);
  T (a9_9[3][3], 3);  T (a9_9[4][3], 3);  T (a9_9[5][3], 3);
  T (a9_9[3][4], 3);  T (a9_9[4][4], 4);  T (a9_9[5][4], 4);
  T (a9_9[3][5], 3);  T (a9_9[4][5], 4);  T (a9_9[5][5], 5);
  T (a9_9[3][6], 3);  T (a9_9[4][6], 4);  T (a9_9[5][6], 5);
  T (a9_9[3][7], 3);  T (a9_9[4][7], 4);  T (a9_9[5][7], 5);
  T (a9_9[3][8], 3);  T (a9_9[4][8], 4);  T (a9_9[5][8], 5);

  T (a9_9[6][0], 0);  T (a9_9[7][0], 0);  T (a9_9[8][0], 0);
  T (a9_9[6][1], 1);  T (a9_9[7][1], 1);  T (a9_9[8][1], 1);
  T (a9_9[6][2], 2);  T (a9_9[7][2], 2);  T (a9_9[8][2], 2);
  T (a9_9[6][3], 3);  T (a9_9[7][3], 3);  T (a9_9[8][3], 3);
  T (a9_9[6][4], 4);  T (a9_9[7][4], 4);  T (a9_9[8][4], 4);
  T (a9_9[6][5], 5);  T (a9_9[7][5], 5);  T (a9_9[8][5], 5);
  T (a9_9[6][6], 6);  T (a9_9[7][6], 6);  T (a9_9[8][6], 6);
  T (a9_9[6][7], 6);  T (a9_9[7][7], 7);  T (a9_9[8][7], 7);
  T (a9_9[6][8], 6);  T (a9_9[7][8], 7);  T (a9_9[8][8], 8);


  T (a9_9[0][0] + 1, 0);  T (a9_9[1][0] + 1, 0);  T (a9_9[2][0] + 2, 0);
  T (a9_9[0][1] + 2, 0);  T (a9_9[1][1] + 1, 0);  T (a9_9[2][1] + 2, 0);
  T (a9_9[0][2] + 3, 0);  T (a9_9[1][2] + 1, 0);  T (a9_9[2][2] + 2, 0);
  T (a9_9[0][3] + 4, 0);  T (a9_9[1][3] + 1, 0);  T (a9_9[2][3] + 2, 0);
  T (a9_9[0][4] + 5, 0);  T (a9_9[1][4] + 1, 0);  T (a9_9[2][4] + 2, 0);
  T (a9_9[0][5] + 6, 0);  T (a9_9[1][5] + 1, 0);  T (a9_9[2][5] + 2, 0);
  T (a9_9[0][6] + 7, 0);  T (a9_9[1][6] + 1, 0);  T (a9_9[2][6] + 2, 0);
  T (a9_9[0][7] + 8, 0);  T (a9_9[1][7] + 1, 0);  T (a9_9[2][7] + 2, 0);
}

void test_global_struct (void)
{
  T (s.a1, 0);
  T (s.a2, 1);
  T (s.a3, 2);
  T (s.a4, 3);
  T (s.a5, 4);
  T (s.a6, 5);
  T (s.a7, 6);
  T (s.a8, 7);
  T (s.a9, 8);
}

void test_global_struct_array (void)
{
  T (sa[0].a1, 0);  T (sa[1].a1, 0);  T (sa[2].a1, 0);  T (sa[3].a1, 0);
  T (sa[0].a2, 0);  T (sa[1].a2, 1);  T (sa[2].a2, 1);  T (sa[3].a2, 1);
  T (sa[0].a3, 0);  T (sa[1].a3, 1);  T (sa[2].a3, 2);  T (sa[3].a3, 2);
  T (sa[0].a4, 0);  T (sa[1].a4, 1);  T (sa[2].a4, 2);  T (sa[3].a4, 3);
  T (sa[0].a5, 0);  T (sa[1].a5, 1);  T (sa[2].a5, 2);  T (sa[3].a5, 3);
  T (sa[0].a6, 0);  T (sa[1].a6, 1);  T (sa[2].a6, 2);  T (sa[3].a6, 3);
  T (sa[0].a7, 0);  T (sa[1].a7, 1);  T (sa[2].a7, 2);  T (sa[3].a7, 3);
  T (sa[0].a8, 0);  T (sa[1].a8, 1);  T (sa[2].a8, 2);  T (sa[3].a8, 3);
  T (sa[0].a9, 0);  T (sa[1].a9, 1);  T (sa[2].a9, 2);  T (sa[3].a9, 3);

  T (sa[4].a1, 0);  T (sa[5].a1, 0);  T (sa[6].a1, 0);  T (sa[7].a1, 0);
  T (sa[4].a2, 1);  T (sa[5].a2, 1);  T (sa[6].a2, 1);  T (sa[7].a2, 1);
  T (sa[4].a3, 2);  T (sa[5].a3, 2);  T (sa[6].a3, 2);  T (sa[7].a3, 2);
  T (sa[4].a4, 3);  T (sa[5].a4, 3);  T (sa[6].a4, 3);  T (sa[7].a4, 3);
  T (sa[4].a5, 4);  T (sa[5].a5, 4);  T (sa[6].a5, 4);  T (sa[7].a5, 4);
  T (sa[4].a6, 4);  T (sa[5].a6, 5);  T (sa[6].a6, 5);  T (sa[7].a6, 5);
  T (sa[4].a7, 4);  T (sa[5].a7, 5);  T (sa[6].a7, 6);  T (sa[7].a7, 6);
  T (sa[4].a8, 4);  T (sa[5].a8, 5);  T (sa[6].a8, 6);  T (sa[7].a8, 7);
  T (sa[4].a9, 4);  T (sa[5].a9, 5);  T (sa[6].a9, 6);  T (sa[7].a9, 7);

  T (sa[8].a1, 0);
  T (sa[8].a2, 1);  T (sa[8].a2 + 1, 0);
  T (sa[8].a3, 2);  T (sa[8].a3 + 1, 1);  T (sa[8].a3 + 2, 0);
  T (sa[8].a4, 3);  T (sa[8].a4 + 1, 2);  T (sa[8].a4 + 2, 1);
  T (sa[8].a5, 4);  T (sa[8].a5 + 1, 3);  T (sa[8].a5 + 2, 2);
  T (sa[8].a6, 5);  T (sa[8].a6 + 1, 4);  T (sa[8].a6 + 2, 3);
  T (sa[8].a7, 6);  T (sa[8].a7 + 1, 5);  T (sa[8].a7 + 2, 4);
  T (sa[8].a8, 7);  T (sa[8].a8 + 1, 6);  T (sa[8].a8 + 2, 5);
  T (sa[8].a9, 8);  T (sa[8].a9 + 1, 7);  T (sa[8].a9 + 2, 6);


  T (sa3_5_7[1][2][3].a2, 1);
  T (sa3_5_7[1][3][5].a3, 2);
  T (sa3_5_7[2][4][5].a4, 3);

  T (sa3_5_7[0][0][0].a1, 0);
  T (sa3_5_7[0][0][0].a2, 0);
  T (sa3_5_7[0][0][0].a3, 0);
  T (sa3_5_7[0][0][0].a4, 0);
  T (sa3_5_7[0][0][0].a5, 0);
  T (sa3_5_7[0][0][0].a6, 0);
  T (sa3_5_7[0][0][0].a7, 0);
  T (sa3_5_7[0][0][0].a8, 0);
  T (sa3_5_7[0][0][0].a9, 0);

  T (sa3_5_7[0][0][1].a1, 0);
  T (sa3_5_7[0][0][1].a2, 0);
  T (sa3_5_7[0][0][1].a3, 0);
  T (sa3_5_7[0][0][1].a4, 0);
  T (sa3_5_7[0][0][1].a5, 0);
  T (sa3_5_7[0][0][1].a6, 0);
  T (sa3_5_7[0][0][1].a7, 0);
  T (sa3_5_7[0][0][1].a8, 0);
  T (sa3_5_7[0][0][1].a9, 0);

  T (sa3_5_7[0][1][0].a1, 0);
  T (sa3_5_7[0][1][0].a2, 0);
  T (sa3_5_7[0][1][0].a3, 0);
  T (sa3_5_7[0][1][0].a4, 0);
  T (sa3_5_7[0][1][0].a5, 0);
  T (sa3_5_7[0][1][0].a6, 0);
  T (sa3_5_7[0][1][0].a7, 0);
  T (sa3_5_7[0][1][0].a8, 0);
  T (sa3_5_7[0][1][0].a9, 0);

  T (sa3_5_7[1][0][0].a1, 0);
  T (sa3_5_7[1][0][0].a2, 0);
  T (sa3_5_7[1][0][0].a3, 0);
  T (sa3_5_7[1][0][0].a4, 0);
  T (sa3_5_7[1][0][0].a5, 0);
  T (sa3_5_7[1][0][0].a6, 0);
  T (sa3_5_7[1][0][0].a7, 0);
  T (sa3_5_7[1][0][0].a8, 0);
  T (sa3_5_7[1][0][0].a9, 0);
}


struct SS {
  char a9[9][9];
  struct S sa9[9];
};

const struct SS ssa[] = {
  [1] = {
    .a9 = { [3] = S3, [7] = S7 },
    .sa9 = { [5] = { .a5 = S4, .a7 = S6 } }
  },
  [5] = {
    .a9 = { [1] = S8, [5] = S4 },
    .sa9 = { [3] = { .a3 = S2, .a6 = S3 } }
  }
};

void test_global_struct_struct_array (void)
{
  T (ssa[0].a9[0], 0);
  T (ssa[0].a9[3], 0);
  T (ssa[0].sa9[5].a5, 0);
  T (ssa[0].sa9[5].a7, 0);

  T (ssa[1].a9[0], 0);

  T (ssa[1].a9[3], 3);
  T (ssa[1].a9[7], 7);
  T (ssa[1].sa9[5].a5, 4);
  T (ssa[1].sa9[5].a7, 6);

  T (ssa[2].a9[3], 0);
  T (ssa[2].a9[7], 0);
  T (ssa[2].sa9[5].a5, 0);
  T (ssa[2].sa9[5].a7, 0);

  T (ssa[5].a9[1], 8);
  T (ssa[5].a9[5], 4);
  T (ssa[5].sa9[3].a3, 2);
  T (ssa[5].sa9[3].a6, 3);
}

/* { dg-final { scan-tree-dump-times "strlen1" 0 "gimple" } }
   { dg-final { scan-tree-dump-times "call_in_true_branch_not_eliminated" 0 "ccp1" } } */
