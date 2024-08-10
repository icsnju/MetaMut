/* PR middle-end/81384 - built-in form of strnlen missing

   Verify that a strnlen bound in excess of the maximum object size
   is diagnosed regardless of attribute nonstring.  Also verify that
   a bound that's greater than the size of a non-string array is
   diagnosed, even if it's not in excess of the maximum object size.

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

#define STR   /* not non-string */
#define NS    __attribute__ ((nonstring))

#define _CAT(s, n)   s ## n
#define CAT(s, n)    _CAT (s, n)
#define UNIQ(n)      CAT (n, __LINE__)

void sink (size_t);

#define T(attr, N, bound)			\
  do {						\
    extern attr char UNIQ (a)[N];		\
    sink (strnlen (UNIQ (a), bound));		\
  } while (0)

void strnlen_cst (void)
{
  size_t n = DIFF_MAX;

  T (STR, /* [] */, n);
  T (STR, /* [] */, n + 1);    /* { dg-warning "specified bound \[0-9\]+ exceeds maximum object size \[0-9\]+" } */

  T (STR, 1, n);
  T (STR, 2, n + 1);           /* { dg-warning "specified bound \[0-9\]+ exceeds maximum object size \[0-9\]+" } */

  T (NS, /* [] */, n);
  T (NS, /* [] */, n + 1);     /* { dg-warning "specified bound \[0-9\]+ exceeds maximum object size \[0-9\]+" } */

  T (NS, 9, n);                /* { dg-warning "argument 1 declared attribute 'nonstring' is smaller than the specified bound \\d+|specified bound \\d+ exceeds source size 9" } */
  T (NS, 10, n + 1);           /* { dg-warning "specified bound \[0-9\]+ exceeds maximum object size \[0-9\]+" } */
}


void strnlen_range (void)
{
  size_t n = DIFF_MAX;
  n = UR (n, n + 1);

  T (STR, /* [] */, n);
  T (STR, /* [] */, n + 1);    /* { dg-warning "specified bound \\\[\[0-9\]+, \[0-9\]+] exceeds maximum object size \[0-9\]+" } */

  T (STR, 1, n);               /* { dg-warning "specified bound \\\[\[0-9\]+, \[0-9\]+] exceeds source size 1" } */
  T (STR, 2, n + 1);           /* { dg-warning "specified bound \\\[\[0-9\]+, \[0-9\]+] exceeds maximum object size \[0-9\]+" } */

  T (NS, /* [] */, n);
  T (NS, /* [] */, n + 1);     /* { dg-warning "specified bound \\\[\[0-9\]+, \[0-9\]+] exceeds maximum object size \[0-9\]+" } */

  T (NS, 9, n);                /* { dg-warning "argument 1 declared attribute 'nonstring' is smaller than the specified bound \\\[\\d+, \\d+]|specified bound \\\[\\d+, \\d+] exceeds source size 9" } */
  T (NS, 10, n + 1);           /* { dg-warning "specified bound \\\[\[0-9\]+, \[0-9\]+] exceeds maximum object size \[0-9\]+" } */
}
