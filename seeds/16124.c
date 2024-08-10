/* Exercise that -Warray-bounds is issued for out-of-bounds offsets
   in calls to built-in functions.
   { dg-do compile }
   { dg-options "-O2 -Warray-bounds=2 -Wno-stringop-overflow -ftrack-macro-expansion=0" }  */

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

#if __cplusplus
#  define restrict __restrict
extern "C" {
#endif

extern void* memcpy (void* restrict, const void* restrict, size_t);
extern void* mempcpy (void* restrict, const void* restrict, size_t);
extern void* memmove (void*, const void*, size_t);

extern char* stpcpy (char* restrict, const char* restrict);

extern char* strcat (char* restrict, const char* restrict);
extern char* strcpy (char* restrict, const char* restrict);
extern char* strncpy (char* restrict, const char* restrict, size_t);

#if __cplusplus
}   /* extern "C" */
#endif

struct MA { char a5[5], a7[7]; };

void sink (void*, ...);

void test_memcpy_bounds_memarray_range (void)
{
#undef TM
#define TM(mem, dst, src, n)			\
  do {						\
    struct MA ma;				\
    sink (&ma);   /* Initialize arrays.  */	\
    memcpy (dst, src, n);			\
    sink (&ma);					\
  } while (0)

  ptrdiff_t j = SR (1, 2);

  TM (ma.a5, ma.a5 + j, ma.a5, 1);
  TM (ma.a5, ma.a5 + j, ma.a5, 3);

  /* The copy below is invalid for two reasons: 1) it overlaps and 2) it
     writes past the end of ma.a5.  The warning is a little cryptic here
     because the GIMPLE is:
       _4 = &ma.a5 + prephitmp_14;
       MEM <unsigned char[5]> [(char * {ref-all})_4]
         = MEM <unsigned char[5]> [(char * {ref-all})&ma];
     and could be improved.  Just verify that one is issued but not its
     full text.  */
  TM (ma.a5, ma.a5 + j, ma.a5, 5);        /* { dg-warning "\\\[-Warray-bounds" "pr101374" { xfail *-*-* } } */

  TM (ma.a5, ma.a5 + j, ma.a5, 7);        /* { dg-warning "offset \\\[5, 7] from the object at .ma. is out of the bounds of referenced subobject .\(MA::\)?a5. with type .char ?\\\[5]. at offset 0" } */
  TM (ma.a5, ma.a5 + j, ma.a5, 9);        /* { dg-warning "offset \\\[5, 9] from the object at .ma. is out of the bounds of referenced subobject .\(MA::\)?a5. with type .char ?\\\[5]. at offset 0" } */
}

#if __i386__ || __x86_64__

/* Disabled for non-x86 targets due to bug 83543.  */

void test_strcpy_bounds_memarray_range (void)
{
#undef TM
#define TM(a5init, a7init, dst, src)		\
  do {						\
    struct MA ma = { a5init, a7init };		\
    strcpy (dst, src);				\
    sink (&ma);					\
  } while (0)

  ptrdiff_t i = SR (1, 2);

  TM ("0", "",     ma.a5 + i, ma.a5);
  TM ("01", "",    ma.a5 + i, ma.a5);
  TM ("012", "",   ma.a5 + i, ma.a5);
  TM ("0123", "",  ma.a5 + i, ma.a5);     /* { dg-warning "offset 5 from the object at .ma. is out of the bounds of referenced subobject .\(MA::\)?a5. with type .char *\\\[5]. at offset 0" "pr83543" { xfail { ! { i?86-*-* x86_64-*-* } } } } */

  TM ("", "012345", ma.a7 + i, ma.a7);    /* { dg-warning "offset 12 from the object at .ma. is out of the bounds of referenced subobject .\(MA::\)?a7. with type .char ?\\\[7]. at offset 5" "pr83543" { xfail { ! { i?86-*-* x86_64-*-* } } } } */
}
#endif
