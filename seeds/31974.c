/* PR tree-optimization/84047 - missing -Warray-bounds on an out-of-bounds
   index into an array
   { dg-do compile }
   { dg-options "-O2 -Warray-bounds=2 -ftrack-macro-expansion=0" }
   { dg-skip-if "too many arguments in function call" { bpf-*-* } } */

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

#define MAX DIFF_MAX
#define MIN DIFF_MIN

void sink (int, ...);

#define T(...)   sink (0, __VA_ARGS__)

void test_global_char_array (void)
{
  extern char gcar1[1];
  char *p = gcar1;

  T (p[MIN]);       /* { dg-warning "subscript -\[0-9\]+ is outside array bounds of .char\\\[1]." } */
  T (p[-1]);        /* { dg-warning "subscript -1 is outside array bounds of .char\\\[1]." } */
  T (p[0]);
  T (p[1]);         /* { dg-warning "subscript 1 is outside array bounds of .char\\\[1]." } */
  T (p[MAX]);       /* { dg-warning "subscript \[0-9\]+ is outside array bounds of .char\\\[1]." } */

  T (&p[MIN]);      /* { dg-warning "subscript -\[0-9\]+ is \(below|outside\) array bounds of .char\\\[1]." } */
  T (&p[-1]);       /* { dg-warning "subscript -1 is \(below|outside\) array bounds of .char\\\[1]." } */
  T (&p[0]);
  T (&p[1]);
  T (&p[2]);        /* { dg-warning "subscript 2 is \(above|outside\) array bounds of .char\\\[1]." } */
  T (&p[MAX]);      /* { dg-warning "subscript \[0-9\]+ is \(above|outside\) array bounds of .char\\\[1]." } */

  extern char gcar3[3];
  char *q = gcar3;

  T (q[MIN]);       /* { dg-warning "subscript -\[0-9\]+ is outside array bounds of .char\\\[3]." } */
  T (q[-1]);        /* { dg-warning "subscript -1 is outside array bounds of .char\\\[3]." } */
  T (q[0]);
  T (q[1]);
  T (q[2]);
  T (q[3]);         /* { dg-warning "subscript 3 is outside array bounds of .char\\\[3]." } */
  T (q[MAX]);       /* { dg-warning "subscript \[0-9\]+ is outside array bounds of .char\\\[3]." } */

  T (&q[MIN]);      /* { dg-warning "subscript -\[0-9\]+ is \(below|outside\) array bounds of .char\\\[3]." } */
  T (&q[-1]);       /* { dg-warning "subscript -1 is \(below|outside\) array bounds of .char\\\[3]." } */
  T (&q[0]);
  T (&q[1]);
  T (&q[2]);
  T (&q[3]);
  T (&q[4]);        /* { dg-warning "subscript 4 is \(above|outside\) array bounds of .char\\\[3]." } */
  T (&q[MAX]);      /* { dg-warning "subscript \[0-9\]+ is \(above|outside\) array bounds of .char\\\[3]." } */
}


void test_global_int_array (void)
{
  /* Use smaller values to prevent false negatives due to undetected
     integer overflow/wrapping.  */
  ptrdiff_t min = MIN / sizeof (int);
  ptrdiff_t max = MAX / sizeof (int);

  extern int giar1[1];
  extern int giar3[3];

  int *p = giar1;

  T (p[min]);       /* { dg-warning "subscript -\[0-9\]+ is outside array bounds of .int\\\[1]." } */
  T (p[-1]);        /* { dg-warning "subscript -1 is outside array bounds of .int\\\[1]." } */
  T (p[0]);
  T (p[1]);         /* { dg-warning "subscript 1 is outside array bounds of .int\\\[1]." } */
  T (p[max]);       /* { dg-warning "subscript \[0-9\]+ is outside array bounds of .int\\\[1]." } */

  T (&p[min]);      /* { dg-warning "subscript -\[0-9\]+ is \(below|outside\) array bounds of .int\\\[1]." } */
  T (&p[-1]);       /* { dg-warning "subscript -1 is \(below|outside\) array bounds of .int\\\[1]." } */
  T (&p[0], &p[1]);
  T (&p[2]);        /* { dg-warning "subscript 2 is \(above|outside\) array bounds of .int\\\[1]." } */
  T (&p[max]);      /* { dg-warning "subscript \[0-9\]+ is \(above|outside\) array bounds of .int\\\[1]." } */

  int *q = giar3;

  T (q[min]);       /* { dg-warning "subscript -\[0-9\]+ is outside array bounds of .int\\\[3]." } */
  T (q[-1]);        /* { dg-warning "subscript -1 is outside array bounds of .int\\\[3]." } */
  T (q[0]);
  T (q[1]);
  T (q[2]);
  T (q[3]);         /* { dg-warning "subscript 3 is outside array bounds of .int\\\[3]." } */
  T (q[max]);       /* { dg-warning "subscript \[0-9\]+ is outside array bounds of .int\\\[3]." } */

  T (&q[min]);      /* { dg-warning "subscript -\[0-9\]+ is \(below|outside\) array bounds of .int\\\[3]." } */
  T (&q[-1]);       /* { dg-warning "subscript -1 is \(below|outside\) array bounds of .int\\\[3]." } */
  T (&q[0]);
  T (&q[1]);
  T (&q[2]);
  T (&q[3]);
  T (&q[4]);        /* { dg-warning "subscript 4 is \(above|outside\) array bounds of .int\\\[3]." } */
  T (&q[max]);      /* { dg-warning "subscript \[0-9\]+ is \(above|outside\) array bounds of .int\\\[3]." } */
}


void test_global_short_2dim_array (void)
{
  extern short giar3_5[3][5];

  short *p = giar3_5[0];

  /* The access below isn't diagnosed because the reference is transformed
     into MEM_REF (short*, &giar3_5, 0), i.e., *giar3_5[0][0].  */
  T (p[MIN]);       /* { dg-warning "subscript -\[0-9\]+ is outside array bounds of .short int\\\[3]" "bug" { xfail *-*-*} } */
  T (p[-1]);        /* { dg-warning "subscript -1 is outside array bounds of .short int\\\[3]" } */
  T (p[0]);
  T (p[1]);
  T (p[2]);
  T (p[15]);        /* { dg-warning "subscript 15 is outside array bounds of .short int\\\[3]" } */
  T (p[MAX]);       /* { dg-warning "subscript -?\[0-9\]+ is outside array bounds of .short int\\\[3]" } */

  T (&p[MIN]);      /* { dg-warning "subscript -\[0-9\]+ is \(below|outside\) array bounds of .short int\\\[3]" "bug" { xfail *-*-* } } */
  T (&p[-1]);       /* { dg-warning "subscript -1 is \(below|outside\) array bounds of .short int\\\[3]" } */
  T (&p[0]);
  T (&p[1]);
  T (&p[2]);
  T (&p[3]);
  T (&p[16]);       /* { dg-warning "subscript 16 is \(above|outside\) array bounds of .short int\\\[3]" "pr??????" { xfail *-*-* } } */
  T (&p[MAX]);      /* { dg-warning "subscript -?\[0-9\]+ is \(above|outside\) array bounds of .short int\\\[3]" } */
}


void test_local_char_array (void)
{
  char ar1[1] = { 1 };
  char *p = ar1;

  T (p[MIN]);      /* { dg-warning "subscript -\[0-9\]+ is outside array bounds of .char\\\[1]." } */
  T (p[-1]);       /* { dg-warning "subscript -1 is outside array bounds of .char\\\[1]." } */
  T (p[0]);
  T (p[1]);         /* { dg-warning "subscript 1 is outside array bounds of .char\\\[1]." } */
  T (p[MAX]);       /* { dg-warning "subscript \[0-9\]+ is outside array bounds of .char\\\[1]." } */

  T (&p[MIN]);     /* { dg-warning "subscript -\[0-9\]+ is \(below|outside\) array bounds of .char\\\[1]." } */
  T (&p[-1]);      /* { dg-warning "subscript -1 is \(below|outside\) array bounds of .char\\\[1]." } */
  T (&p[0]);
  T (&p[1]);
  T (&p[2]);        /* { dg-warning "subscript 2 is \(above|outside\) array bounds of .char\\\[1]." } */
  T (&p[MAX]);      /* { dg-warning "subscript \[0-9\]+ is \(above|outside\) array bounds of .char\\\[1]." } */

  char ar3[3] = { 1, 2, 3 };
  p = ar3;

  T (p[MIN]);       /* { dg-warning "subscript -\[0-9\]+ is outside array bounds of .char\\\[3]." } */
  T (p[-1]);        /* { dg-warning "subscript -1 is outside array bounds of .char\\\[3]." } */
  T (p[0]);
  T (p[1]);
  T (p[2]);
  T (p[3]);         /* { dg-warning "subscript 3 is outside array bounds of .char\\\[3]." } */
  T (p[MAX]);       /* { dg-warning "subscript \[0-9\]+ is outside array bounds of .char\\\[3]." } */

  T (&p[MIN]);      /* { dg-warning "subscript -\[0-9\]+ is \(below|outside\) array bounds of .char\\\[3]." } */
  T (&p[-1]);       /* { dg-warning "subscript -1 is \(below|outside\) array bounds of .char\\\[3]." } */
  T (&p[0]);
  T (&p[1]);
  T (&p[2]);
  T (&p[3]);
  T (&p[4]);        /* { dg-warning "subscript 4 is \(above|outside\) array bounds of .char\\\[3]." } */
  T (&p[MAX]);      /* { dg-warning "subscript \[0-9\]+ is \(above|outside\) array bounds of .char\\\[3]." } */
}

struct S
{
  int a[2], b[3];
} s [4];

void test_struct_array_cst (void)
{
  T (s[0].a[0] + s[0].a[1] + s[0].b[0] + s[0].b[1] + s[0].b[2] + s[0].b[2]
     + s[1].a[0] + s[1].a[1] + s[1].b[0] + s[1].b[1] + s[1].b[2] + s[1].b[2]
     + s[2].a[0] + s[2].a[1] + s[2].b[0] + s[2].b[1] + s[2].b[2] + s[2].b[2]
     + s[3].a[0] + s[3].a[1] + s[3].b[0] + s[3].b[1] + s[3].b[2] + s[3].b[2]);

  T (&s[0].a[2],
     &s[0].b[3],
     &s[1].a[2],
     &s[1].b[3],
     &s[2].a[2],
     &s[2].b[3],
     &s[3].a[2],
     &s[3].b[3]);

  T (s[0].a[2]);    /* { dg-warning "subscript 2 is above array bounds of .int\\\[2\\\]." } */
  T (s[0].b[3]);    /* { dg-warning "subscript 3 is above array bounds of .int\\\[3\\\]." } */
  T (s[1].a[2]);    /* { dg-warning "subscript 2 is above array bounds of .int\\\[2\\\]." } */
  T (s[1].b[3]);    /* { dg-warning "subscript 3 is above array bounds of .int\\\[3\\\]." } */
  T (s[2].a[2]);    /* { dg-warning "subscript 2 is above array bounds of .int\\\[2\\\]." } */
  T (s[2].b[3]);    /* { dg-warning "subscript 3 is above array bounds of .int\\\[3\\\]." } */
  T (s[3].a[2]);    /* { dg-warning "subscript 2 is above array bounds of .int\\\[2\\\]." } */
  T (s[3].b[3]);    /* { dg-warning "subscript 3 is above array bounds of .int\\\[3\\\]." } */

  T (s[4].a[0]);    /* { dg-warning "subscript 4 is above array bounds of .struct S\\\[4\\\]." } */
  T (s[4].a[2]);    /* { dg-warning "subscript 4 is above array bounds of .struct S\\\[4\\\]." } */
                    /* { dg-warning "subscript 2 is above array bounds of .int\\\[2\\\]." "" { target *-*-* } .-1 } */
}
