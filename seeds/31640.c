/* PR middle-end/97373 - missing warning on sprintf into allocated destination
   { dg-do compile }
   { dg-options "-O2 -Wall -ftrack-macro-expansion=0" }
   { dg-require-effective-target alloca } */

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

extern void* alloca (size_t);
extern void* malloc (size_t);

extern int sprintf (char*, const char*, ...);
#define sprintf(d, ...) (sprintf (d, __VA_ARGS__), sink (d))

void sink (void*, ...);

void test_alloca_range (void)
{
  int n1_2 = UR (1, 2);
  int n5_9 = UR (5, 9);

  char *d = (char*)alloca (n5_9);

  sprintf (d, "%i", 12345);

  d += n1_2;
  sprintf (d, "%i", 12345);

  d += n1_2;
  sprintf (d, "%i", 12345);

  d += n1_2;
  sprintf (d, "%i", 12345);

  d += n1_2;
  sprintf (d, "%i", 12345);         // { dg-warning "writing a terminating nul past the end of the destination" }

  d += n1_2;
  sprintf (d, "%i", 12345);         // { dg-warning "'%i' directive writing 5 bytes into a region of size 4" }
}


void test_malloc_range (void)
{
  int n2_3 = UR (2, 3);
  int n5_9 = UR (5, 9);

  char *d = (char*)malloc (n5_9);

  sprintf (d, "%i", 12345);

  d += n2_3;
  sprintf (d, "%i", 12345);

  d += n2_3;
  sprintf (d, "%i", 12345);         // { dg-warning "writing a terminating nul past the end of the destination" }

  d += n2_3;
  sprintf (d, "%i", 12345);         // { dg-warning "'%i' directive writing 5 bytes into a region of size 3" }
}


void test_vla_range (void)
{
  int n3_4 = UR (3, 4);
  int n5_9 = UR (5, 9);

  char vla[n5_9];
  char *d = vla;

  sprintf (d, "%i", 12345);

  d += n3_4;
  sprintf (d, "%i", 12345);

  d += n3_4;
  sprintf (d, "%i", 12345);         // { dg-warning "'%i' directive writing 5 bytes into a region of size 3" }
}
