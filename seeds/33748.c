/* This checks sizes of basic types.  */

#ifndef DEFINED_DEFINES_H
#define DEFINED_DEFINES_H

/* Get __m64 and __m128. */
#include <immintrin.h>

typedef unsigned long long ulonglong;
typedef long double ldouble;

/* These defines determines what part of the test should be run.  When
   GCC implements these parts, the defines should be uncommented to
   enable testing.  */

/* Scalar type __int128.  */
/* #define CHECK_INT128 */

/* Scalar type long double.  */
#define CHECK_LONG_DOUBLE

/* Scalar type __float128.  */
/* #define CHECK_FLOAT128 */

/* Scalar types __m64 and __m128.  */
#define CHECK_M64_M128

/* Returning of complex type.  */
#define CHECK_COMPLEX

/* Structs with size >= 16.  */
#define CHECK_LARGER_STRUCTS

/* Checks for passing floats and doubles.  */
#define CHECK_FLOAT_DOUBLE_PASSING

/* Union passing with not-extremely-simple unions.  */
#define CHECK_LARGER_UNION_PASSING

/* Variable args.  */
#define CHECK_VARARGS

/* Check argument passing and returning for scalar types with sizeof = 16.  */
/* TODO: Implement these tests. Don't activate them for now.  */
#define CHECK_LARGE_SCALAR_PASSING

/* Defines for sizing and alignment.  */

#define TYPE_SIZE_CHAR         1
#define TYPE_SIZE_SHORT        2
#define TYPE_SIZE_INT          4
#ifdef __ILP32__
# define TYPE_SIZE_LONG        4
#else
# define TYPE_SIZE_LONG        8
#endif
#define TYPE_SIZE_LONG_LONG    8
#define TYPE_SIZE_INT128       16
#define TYPE_SIZE_FLOAT16      2
#define TYPE_SIZE_FLOAT        4
#define TYPE_SIZE_DOUBLE       8
#define TYPE_SIZE_LONG_DOUBLE  16
#define TYPE_SIZE_FLOAT128     16
#define TYPE_SIZE_M64          8
#define TYPE_SIZE_M128         16
#define TYPE_SIZE_ENUM         4
#ifdef __ILP32__
# define TYPE_SIZE_POINTER     4
#else
# define TYPE_SIZE_POINTER     8
#endif

#define TYPE_ALIGN_CHAR        1
#define TYPE_ALIGN_SHORT       2
#define TYPE_ALIGN_INT         4
#ifdef __ILP32__
# define TYPE_ALIGN_LONG       4
#else
# define TYPE_ALIGN_LONG       8
#endif
#define TYPE_ALIGN_LONG_LONG   8
#define TYPE_ALIGN_INT128      16
#define TYPE_ALIGN_FLOAT16     2
#define TYPE_ALIGN_FLOAT       4
#define TYPE_ALIGN_DOUBLE      8
#define TYPE_ALIGN_LONG_DOUBLE 16
#define TYPE_ALIGN_FLOAT128    16
#define TYPE_ALIGN_M64         8
#define TYPE_ALIGN_M128        16
#define TYPE_ALIGN_ENUM        4
#ifdef __ILP32__
# define TYPE_ALIGN_POINTER    4
#else
# define TYPE_ALIGN_POINTER    8
#endif

/* These defines control the building of the list of types to check. There
   is a string identifying the type (with a comma after), a size of the type
   (also with a comma and an integer for adding to the total amount of types)
   and an alignment of the type (which is currently not really needed since
   the abi specifies that alignof == sizeof for all scalar types).  */
#ifdef CHECK_INT128
#define CI128_STR "__int128",
#define CI128_SIZ TYPE_SIZE_INT128,
#define CI128_ALI TYPE_ALIGN_INT128,
#define CI128_RET "???",
#else
#define CI128_STR
#define CI128_SIZ
#define CI128_ALI
#define CI128_RET
#endif
#ifdef CHECK_LONG_DOUBLE
#define CLD_STR "long double",
#define CLD_SIZ TYPE_SIZE_LONG_DOUBLE,
#define CLD_ALI TYPE_ALIGN_LONG_DOUBLE,
#define CLD_RET "x87_regs[0]._ldouble",
#else
#define CLD_STR
#define CLD_SIZ
#define CLD_ALI
#define CLD_RET
#endif
#ifdef CHECK_FLOAT128
#define CF128_STR "__float128",
#define CF128_SIZ TYPE_SIZE_FLOAT128,
#define CF128_ALI TYPE_ALIGN_FLOAT128, 
#define CF128_RET "???",
#else
#define CF128_STR
#define CF128_SIZ
#define CF128_ALI
#define CF128_RET
#endif
#ifdef CHECK_M64_M128
#define CMM_STR "__m64", "__m128",
#define CMM_SIZ TYPE_SIZE_M64, TYPE_SIZE_M128,
#define CMM_ALI TYPE_ALIGN_M64, TYPE_ALIGN_M128,
#define CMM_RET "???", "???",
#else
#define CMM_STR
#define CMM_SIZ
#define CMM_ALI
#define CMM_RET
#endif

/* Used in size and alignment tests.  */
enum dummytype { enumtype };

extern void abort (void);

/* Assertion macro.  */
#define assert(test) if (!(test)) abort()

#ifdef __GNUC__
#define ATTRIBUTE_UNUSED __attribute__((__unused__))
#else
#define ATTRIBUTE_UNUSED
#endif

#ifdef __GNUC__
#define PACKED __attribute__((__packed__))
#else
#warning Some tests will fail due to missing __packed__ support
#define PACKED
#endif

#endif /* DEFINED_DEFINES_H */
#ifndef MACROS_H

#define check_size(_t, _size) assert(sizeof(_t) == (_size))

#define check_align(_t, _align) assert(__alignof__(_t) == (_align))

#define check_align_lv(_t, _align) assert(__alignof__(_t) == (_align) \
					  && (((unsigned long)&(_t)) & ((_align) - 1) ) == 0)

#define check_basic_struct_size_and_align(_type, _size, _align) { \
  struct _str { _type dummy; } _t; \
  check_size(_t, _size); \
  check_align_lv(_t, _align); \
}

#define check_array_size_and_align(_type, _size, _align) { \
  _type _a[1]; _type _b[2]; _type _c[16]; \
  struct _str { _type _a[1]; } _s; \
  check_align_lv(_a[0], _align); \
  check_size(_a, _size); \
  check_size(_b, (_size*2)); \
  check_size(_c, (_size*16)); \
  check_size(_s, _size); \
  check_align_lv(_s._a[0], _align); \
}

#define check_basic_union_size_and_align(_type, _size, _align) { \
  union _union { _type dummy; } _u; \
  check_size(_u, _size); \
  check_align_lv(_u, _align); \
}

#define run_signed_tests2(_function, _arg1, _arg2) \
  _function(_arg1, _arg2); \
  _function(signed _arg1, _arg2); \
  _function(unsigned _arg1, _arg2);

#define run_signed_tests3(_function, _arg1, _arg2, _arg3) \
  _function(_arg1, _arg2, _arg3); \
  _function(signed _arg1, _arg2, _arg3); \
  _function(unsigned _arg1, _arg2, _arg3);

/* Check size of a struct and a union of three types.  */

#define check_struct_and_union3(type1, type2, type3, struct_size, align_size) \
{ \
  struct _str { type1 t1; type2 t2; type3 t3; } _t; \
  union _uni { type1 t1; type2 t2; type3 t3; } _u; \
  check_size(_t, struct_size); \
  check_size(_u, align_size); \
}

#endif // MACROS_H


int
main (void)
{
  /* Integral types.  */
  run_signed_tests2(check_size, char, TYPE_SIZE_CHAR);
  run_signed_tests2(check_size, short, TYPE_SIZE_SHORT);
  run_signed_tests2(check_size, int, TYPE_SIZE_INT);
  run_signed_tests2(check_size, long, TYPE_SIZE_LONG);
  run_signed_tests2(check_size, long long, TYPE_SIZE_LONG_LONG);
#ifdef CHECK_INT128
  run_signed_tests2(check_size, __int128, TYPE_SIZE_INT128);
#endif
  check_size(enumtype, TYPE_SIZE_ENUM);

  /* Floating point types.  */
  check_size(_Float16, TYPE_SIZE_FLOAT16);
  check_size(float, TYPE_SIZE_FLOAT);
  check_size(double, TYPE_SIZE_DOUBLE);
#ifdef CHECK_LONG_DOUBLE
  check_size(long double, TYPE_SIZE_LONG_DOUBLE);
#endif
#ifdef CHECK_FLOAT128
  check_size(__float128, TYPE_SIZE_FLOAT128);
#endif

  /* Packed types - MMX, 3DNow!, SSE and SSE2.  */
#ifdef CHECK_M64_M128
  check_size(__m64, TYPE_SIZE_M64);
  check_size(__m128, TYPE_SIZE_M128);
#endif

  /* Pointer types.  */
  check_size(void *, TYPE_SIZE_POINTER);
  check_size(void (*)(), TYPE_SIZE_POINTER);

  return 0;
}
