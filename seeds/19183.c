
/* PR middle-end/97023 - missing warning on buffer overflow in chained mempcpy
   Verify that writes by built-in functions to objects through pointers
   returned by ordinary (non-built-int) function are assumed to point to
   the beginning of objects.
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

void* memcpy (void*, const void*, size_t);
void* memset (void*, int, size_t);

void sink (void*, ...);

extern char* arrptr[];
extern char* ptr;
extern char* retptr (void);
struct S { char *p; };
extern struct S retstruct (void);

void nowarn_ptr (void)
{
  {
    void *p = arrptr;
    memset (p - 1, 0, 12345);           // { dg-warning "\\\[-Wstringop-overflow" }
    memset (p,0, 12345);
    memset (p,0, DIFF_MAX - 1);
  }

  {
    char *p = arrptr[0];
    memset (p - 1, 0, 12345);
    memset (p - 12345, 0, 12345);
    memset (p - 1234, 0, DIFF_MAX - 1);
    memset (p - DIFF_MAX + 1, 0, 12345);
  }

  {
    char *p = ptr;
    memset (p - 1, 0, 12345);
    memset (p - 12345, 0, 12345);
    memset (p - 1234, 0, DIFF_MAX - 1);
    memset (p - DIFF_MAX + 1, 0, 12345);
  }

  {
    char *p = retptr ();
    memset (p - 1, 0, 12345);
    memset (p - 12345, 0, 12345);
    memset (p - 1234, 0, DIFF_MAX - 1);
    memset (p - DIFF_MAX + 1, 0, 12345);
  }

  {
    char *p = retstruct ().p;
    memset (p - 1, 0, 12345);
    memset (p - 12345, 0, 12345);
    memset (p - 1234, 0, DIFF_MAX - 1);
    memset (p - DIFF_MAX + 1, 0, 12345);
  }
}
