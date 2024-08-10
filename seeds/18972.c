/* PR middle-end/97023 - missing warning on buffer overflow in chained mempcpy
   Verify that out of bounds writes by built-ins to objects through pointers
   returned by memchr() are diagnosed.
   { dg-do compile }
   { dg-options "-O2" } */

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

void* malloc (size_t);
void* memchr (void*, int, size_t);
void* memset (void*, int, size_t);

void sink (void*, ...);

void nowarn_memchr_cst_memset_cst (const void *s)
{
  char *p = malloc (4);
  sink (p);

  p = memchr (p, '1', 4);
  memset (p, 0, 4);
}

void nowarn_memchr_uint_memset_cst (const void *s, unsigned n)
{
  char *p = malloc (4);
  sink (p);

  p = memchr (p, '1', n);
  memset (p, 0, 4);
}

void nowarn_memchr_sz_memset_cst (const void *s, size_t n)
{
  char *p = malloc (4);
  sink (p);

  p = memchr (p, '1', n);
  memset (p, 0, 4);
}

void nowarn_memchr_anti_range_memset_cst (const void *s, size_t n)
{
  char *p = malloc (4);
  sink (p);

  if (n == 0)
    n = 1;

  p = memchr (p, '1', n);
  memset (p, 0, 4);
}

void warn_memchr_cst_memset_cst (const void *s)
{
  char *p = malloc (4);                 // { dg-message "destination object of size 4 " "note" }
  sink (p);

  p = memchr (p, '1', 4);
  memset (p, 0, 5);                     // { dg-warning "writing 5 bytes into a region of size 4 " }
}

void warn_memchr_var_memset_cst (const void *s, unsigned n)
{
  char *p = malloc (4);                 // { dg-message "destination object of size 4 " "note" }
  sink (p);

  p = memchr (p, '1', n);
  memset (p, 0, 5);                     // { dg-warning "writing 5 bytes into a region of size 4 " }
}

void warn_memchr_var_memset_range (const void *s, unsigned n)
{
  /* The offsets in the first two notes are bounded by the size of
     the allocated object.  The real upper bound of the offset in
     the last note includes the upper bound f the offset of the pointer
     returned from the previous memchr() call, but it ends up getting
     constrained to the bounds of the allocated object so it's the same
     as in the first two notes.  The exact value probably isn't too
     important. */
  char *p0 = malloc (UR (5, 7));
  // { dg-message "at offset \\\[\[01\], 6] into destination object of size \\\[5, 7]" "note 2"  { target *-*-* } .-1 }
  // { dg-message "at offset \\\[2, 7] into destination object of size \\\[5, 7]" "note 3"  { target *-*-* } .-2 }

  sink (p0);
  char *p1 = memchr (p0, '1', n);
  memset (p1, 0, UR (8, 9));            // { dg-warning "writing between 8 and 9 bytes into a region of size 7 " }

  sink (p0);
  p1 = memchr (p0 + 1, '2', n);
  memset (p1, 0, UR (7, 9));            // { dg-warning "writing between 7 and 9 bytes into a region of size 6 " }

  sink (p0);
  char *p2 = memchr (p1 + 1, '3', n);
  memset (p2, 0, UR (6, 9));            // { dg-warning "writing between 6 and 9 bytes into a region of size 5 " }
}
