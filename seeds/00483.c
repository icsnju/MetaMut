/* PR tree-optimization/83640 - ice in generic_overlap, at
   gimple-ssa-warn-restrict.c:814
   Test to verify that a pointer offset range whose upper bound is less
   than its lower bound (when interpreted as a signed integer) is handled
   correctly.
   { dg-do compile }
   { dg-options "-O2 -Wrestrict" }  */

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

extern char* strcpy (char*, const char*);
extern char* stpcpy (char*, const char*);

void sink (void*);

void warn_2_smax_p2 (void)
{
  char a[7] = "01234";

  char *d = a;

  ptrdiff_t i = UR (2, DIFF_MAX + (size_t)2);

  strcpy (d, d + i);          /* { dg-warning "accessing between 1 and 4 bytes at offsets 0 and \\\[2, 7] may overlap up to 2 bytes at offset \\\[2, 3]" } */

  sink (d);
}

void nowarn_3_smax_p2 (void)
{
  char a[7] = "12345";

  char *d = a;

  ptrdiff_t i = UR (3, DIFF_MAX + (size_t)2);

  strcpy (d, d + i);

  sink (d);
}

void warn_2u_smax_p2 (void)
{
  char a[7] = "23456";

  char *d = a;

  size_t i = UR (2, DIFF_MAX + (size_t)2);

  strcpy (d, d + i);          /* { dg-warning "accessing between 1 and 4 bytes at offsets 0 and \\\[2, 7] may overlap up to 2 bytes at offset \\\[2, 3]" } */

  sink (d);
}

void nowarn_3u_smax_p2 (void)
{
  char a[7] = "34567";

  char *d = a;

  size_t i = UR (3, DIFF_MAX + (size_t)2);

  strcpy (d, d + i);

  sink (d);
}
