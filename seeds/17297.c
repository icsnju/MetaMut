/* PR middle-end/92341 - missing -Warray-bounds indexing past the end
   of a compound literal
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

#define INT_MAX    __INT_MAX__
#define INT_MIN    (-__INT_MAX__ - 1)

void sink (int, ...);


#define T(...) sink (__LINE__, (__VA_ARGS__))


void direct_idx_cst (void)
{
  T ((int[]){ }[-1]);           // { dg-warning "array subscript -1 is outside array bounds of 'int\\\[0]'" }
  T ((int[]){ }[0]);            // { dg-warning "array subscript 0 is outside array bounds of 'int\\\[0]'" }
  T ((int[]){ }[1]);            // { dg-warning "array subscript 1 is outside array bounds of 'int\\\[0]'" }

  T ((int[]){ 1 }[-1]);         // { dg-warning "array subscript -1 is below array bounds of 'int\\\[1]'" }
  T ((int[]){ 1 }[0]);
  T ((int[]){ 1 }[1]);          // { dg-warning "array subscript 1 is above array bounds of 'int\\\[1]'" }
  T ((int[]){ 1 }[INT_MIN]);    // { dg-warning "array subscript -\[0-9\]+ is below array bounds of 'int\\\[1]'" }
  T ((int[]){ 1 }[INT_MAX]);    // { dg-warning "array subscript \[0-9\]+ is above array bounds of 'int\\\[1]'" }
  T ((int[]){ 1 }[SIZE_MAX]);   // { dg-warning "array subscript \[0-9\]+ is above array bounds of 'int\\\[1]'" }
}


void direct_idx_var (int i)
{
  T ((char[]){ }[i]);           // { dg-warning "array subscript i is outside array bounds of 'char\\\[0]'" }
  T ((int[]){ }[i]);            // { dg-warning "array subscript i is outside array bounds of 'int\\\[0]'" }
}


void direct_idx_range (void)
{
  ptrdiff_t i = SR (-2, -1);

  T ((int[]){ 1 }[i]);          // { dg-warning "array subscript \[ \n\r]+ is outside array bounds of 'int\\\[0]'" "pr?????" { xfail *-*-* } }
}


#undef T
#define T(idx, ...) do {			\
    int *p = (__VA_ARGS__);			\
    sink (p[idx]);				\
  } while (0)

void ptr_idx_cst (void)
{
  T (-1, (int[]){ });           // { dg-warning "array subscript -1 is outside array bounds of 'int\\\[0]'" }
  T ( 0, (int[]){ });           // { dg-warning "array subscript 0 is outside array bounds of 'int\\\[0]'" }
  T (+1, (int[]){ });           // { dg-warning "array subscript 1 is outside array bounds of 'int\\\[0]'" }

  T (-1, (int[]){ 1 });         // { dg-warning "array subscript -1 is outside array bounds of 'int\\\[1]'" }
  T ( 0, (int[]){ 1 });
  T (+1, (int[]){ 1 });         // { dg-warning "array subscript 1 is outside array bounds of 'int\\\[1]'" }
  T (INT_MIN, (int[]){ 1 });    // { dg-warning "array subscript -\[0-9\]+ is outside array bounds of 'int\\\[1]'" "pr92381" { xfail ilp32 } }
  T (INT_MAX, (int[]){ 1 });    // { dg-warning "array subscript \[0-9\]+ is outside array bounds of 'int\\\[1]'" "pr92381" { xfail ilp32 } }
  // { dg-warning "array subscript -\[0-9\]+ is outside array bounds of 'int\\\[1]'" "" { target ilp32 } .-1 }
  T (SIZE_MAX, (int[]){ 1 });   // { dg-warning "array subscript -?\[0-9\]+ is outside array bounds of 'int\\\[1]'" }
}


void ptr_idx_var (int i)
{
  T (i, (int[]){ });            // { dg-warning "array subscript \[^\n\r\]+ is outside array bounds of 'int\\\[0]'" }
  T (i, (int[]){ 1 });
  T (i, (int[]){ i, 1 });
}

void ptr_idx_range (void)
{
  ptrdiff_t i = SR (-2, -1);

  T (i, (int[]){ });            // { dg-warning "array subscript \\\[-2, -1] is outside array bounds of 'int\\\[0]'" }
  T (i, (int[]){ 1 });          // { dg-warning "array subscript \\\[-2, -1] is outside array bounds of 'int\\\[1]'" }
  T (i, (int[]){ i });          // { dg-warning "array subscript \\\[-2, -1] is outside array bounds of 'int\\\[1]'" }

  i = SR (0, 1);

  T (i, (int[]){ });            // { dg-warning "array subscript 0 is outside array bounds of 'int\\\[0]'" }
  T (i, (int[]){ 1 });

  i = SR (1, 2);
  T (i, (int[]){ 1 });          // { dg-warning "array subscript 1 is outside array bounds of 'int\\\[1]'" }

  i = SR (2, 3);
  T (i, (int[]){ 1, 2, 3 });

  i = SR (3, 4);
  T (i, (int[]){ 2, 3, 4 });          // { dg-warning "array subscript 3 is outside array bounds of 'int\\\[3]'" }
}

/* Some of the invalid accesses above also trigger -Wuninitialized.
   { dg-prune-output "\\\[-Wuninitialized" }  */
