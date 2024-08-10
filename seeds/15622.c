/* PR middle-end/91582 - missing heap overflow detection for strcpy

   The -Warray-bounds instances here probably should be replaced by
   -Wstringop-overflow when it detects these overflows (see also
   the xfails in Wstringop-overflow-25.c).

   { dg-do compile }
   { dg-options "-O2 -Wall -Wno-stringop-overflow -ftrack-macro-expansion=0" } */

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

#define INT_MAX     __INT_MAX__
#define INT_MIN     (-INT_MAX - 1)

#define ATTR(...)   __attribute__ ((__VA_ARGS__))
#define NOIPA       ATTR (noipa)

extern void* malloc (size_t);
extern char* strcpy (char*, const char*);

void sink (void*);

#define S36 "0123456789abcdefghijklmnopqrstuvwxyz"
#define S(N) (S36 + sizeof S36 - N - 1)

struct Flex
{
  char n, ax[];
};

extern struct Flex fx;
struct Flex f1 = { 1, { 1 } };
struct Flex f2 = { 2, { 1, 2 } };
struct Flex f3 = { 3, { 1, 2, 3 } };

#define T(src, f) do {				\
    char *s = src;				\
    char *d = f.ax;				\
    strcpy (d, s);				\
    sink (&f);					\
  } while (0)

NOIPA void test_strcpy_flexarray (void)
{
  T (S (0), fx);                // { dg-bogus "\\\[-Warray-bounds" "pr92815" }
  T (S (9), fx);                // { dg-bogus "\\\[-Warray-bounds" "pr92815" }

  T (S (0), f1);
  T (S (1), f1);                // { dg-warning "\\\[-Warray-bounds" }

  T (S (0), f2);
  T (S (1), f2);
  T (S (2), f2);                // { dg-warning "\\\[-Warray-bounds" }

  T (S (0), f3);
  T (S (2), f3);
  T (S (3), f3);                // { dg-warning "\\\[-Warray-bounds" }
  T (S (9), f3);                // { dg-warning "\\\[-Warray-bounds" }
}

#undef T
#define T(T, src, n) do {			\
    char *s = src;				\
    typedef struct { T n, ax[]; } Flex;		\
    Flex *p = (Flex*)malloc (sizeof *p + n);	\
    char *d = (char*)p->ax;			\
    strcpy (d, s);				\
    sink (p);					\
  } while (0)

NOIPA void test_strcpy_malloc_flexarray (void)
{
  size_t r_0_1 = UR (0, 1);

  T (char, S (0), r_0_1);
  T (char, S (1), r_0_1);       // { dg-warning "\\\[-Warray-bounds" }

  size_t r_1_2 = UR (1, 2);

  T (char, S (0), r_1_2);
  T (char, S (1), r_1_2);
  T (char, S (2), r_1_2);       // { dg-warning "\\\[-Warray-bounds" }

  size_t r_2_3 = UR (2, 3);

  T (char, S (0), r_2_3);
  T (char, S (2), r_2_3);
  T (char, S (3), r_2_3);       // { dg-warning "\\\[-Warray-bounds" }
  T (char, S (9), r_2_3);       // { dg-warning "\\\[-Warray-bounds" }
}
