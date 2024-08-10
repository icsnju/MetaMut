/* Verify that calls to strnlen with an unterminated array and
   an excessive non-constant bound are diagnosed.
   { dg-do compile }
   { dg-options "-O2 -Wall -ftrack-macro-expansion=0" } */

#ifndef RANGE_H

/* Definitions of helper functions and macros to create expressions
   in a specified range.  Not all the symbols declared here are
   defined.  */

#define SIZE_MAX __SIZE_MAX__
#define DIFF_MAX __PTRDIFF_MAX__
#define DIFF_MIN (-DIFF_MAX - 1)

typedef __INT32_TYPE__   int32_t;
typedef __PTRDIFF_TYPE__ ptrdiff_t;
typedef __SIZE_TYPE__    size_t;

static inline ptrdiff_t signed_value (void)
{
  extern volatile ptrdiff_t signed_value_source;
  return signed_value_source;
}

static inline size_t unsigned_value (void)
{
  extern volatile size_t unsigned_value_source;
  return unsigned_value_source;
}

static inline ptrdiff_t signed_range (ptrdiff_t min, ptrdiff_t max)
{
  ptrdiff_t val = signed_value ();
  return val < min || max < val ? min : val;
}

static inline ptrdiff_t signed_anti_range (ptrdiff_t min, ptrdiff_t max)
{
  ptrdiff_t val = signed_value ();
  return min <= val && val <= max ? min == DIFF_MIN ? max + 1 : min - 1 : val;
}

static inline size_t unsigned_range (size_t min, size_t max)
{
  size_t val = unsigned_value ();
  return val < min || max < val ? min : val;
}

static inline size_t unsigned_anti_range (size_t min, size_t max)
{
  size_t val = unsigned_value ();
  return min <= val && val <= max ? min == 0 ? max + 1 : min - 1 : val;
}

#define SR(min, max) signed_range ((min), (max))
#define UR(min, max) unsigned_range ((min), (max))

#define SAR(min, max) signed_anti_range ((min), (max))
#define UAR(min, max) unsigned_anti_range ((min), (max))

#endif /* RANGE_H */

extern size_t strnlen (const char*, size_t);

const char a[5] = "12345";   /* { dg-message "declared here" } */
enum { asz = sizeof a };

int v0 = 0;
int v1 = 1;

void sink (int, ...);

#define CONCAT(a, b)   a ## b
#define CAT(a, b)      CONCAT(a, b)

#define T(str, n)					\
  __attribute__ ((noipa))				\
  void CAT (test_, __LINE__) (void) {			\
    int i0 = 0, i1 = i0 + 1, i2 = i1 + 1, i3 = i2 + 1;	\
    sink (strnlen (str, n), i0, i1, i2, i3);		\
  } typedef void dummy_type


T (a, UR (asz, -1));
T (a, UR (asz - 1, -1));
T (a, UR (asz - 2, -1));
T (a, UR (asz - 5, -1));
T (&a[0], UR (asz, -1));
T (&a[0] + 1, UR (asz, asz + 1)); /* { dg-warning "specified bound \\\[5, 6] exceeds the size 4 of unterminated array" } */
T (&a[1], UR (asz, 6));           /* { dg-warning "specified bound \\\[5, 6] exceeds the size 4 of unterminated array" } */
T (&a[1], UR (asz - 1, 7));
T (&a[v0], UR (asz, 8));          /* { dg-warning "specified bound \\\[5, 8] may exceed the size of at most 5 of unterminated array" } */
T (&a[v0] + 1, UR (asz, 9));      /* { dg-warning "specified bound \\\[5, 9] may exceed the size of at most 5 of unterminated array" } */

T (a, UR (asz + 1, asz + 2));     /* { dg-warning "specified bound \\\[6, 7] exceeds the size 5 " } */
T (&a[0], UR (asz + 1, 10));      /* { dg-warning "unterminated" } */
T (&a[0] + 1, UR (asz - 1, 11));
T (&a[0] + 1, UR (asz + 1, 12));  /* { dg-warning "unterminated" } */
T (&a[1], UR (asz + 1, 13));      /* { dg-warning "unterminated" } */
T (&a[v0], UR (asz + 1, 14));     /* { dg-warning "unterminated" } */
T (&a[v0] + 1, UR (asz + 1, 15)); /* { dg-warning "unterminated" } */

T (&a[v0] + 1, UR (DIFF_MAX, SIZE_MAX)); /* { dg-warning "unterminated" } */

T (&a[v0] + 1, UR (DIFF_MAX + (size_t)1, SIZE_MAX)); /* { dg-warning "specified bound \\\[\[0-9\]+, \[0-9\]+] exceeds maximum object size " } */


const char c[4] = "1234";

void test (int n0)
{
  char a[] = "123";

  if (n0 < 4)
    n0 = 4;
  int n1 = __builtin_strlen (a);

  int n = n0 < n1 ? n1 : n0;

  /* N is at least 4 and c[4] is out-of-bounds.  This could trigger
     either -Warray-bounds or -Wstringop-overread.  -Warray-bounds
     only diagnoses past-the-end accesses by modifying functions
     (in gimple-ssa-warn-restrict.c) and even for those, either
     -Wstringop-overflow or -Wstringop-overread would be more
     appropriate.  */
  sink (strnlen (c + n, n + 1));    /* { dg-warning "specified bound \\\[5, \[0-9\]+] exceeds the size of at most 4 of unterminated array" } */
}
