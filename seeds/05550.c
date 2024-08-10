/* This is an autogenerated file. Do not edit.  */

#ifndef DEFINED_DEFINES_H
#define DEFINED_DEFINES_H

/* Get __m64 and __m128. */
#include <xmmintrin.h>

typedef unsigned long ulong;
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
#define TYPE_SIZE_LONG         8
#define TYPE_SIZE_LONG_LONG    8
#define TYPE_SIZE_INT128       16
#define TYPE_SIZE_FLOAT        4
#define TYPE_SIZE_DOUBLE       8
#define TYPE_SIZE_LONG_DOUBLE  16
#define TYPE_SIZE_FLOAT128     16
#define TYPE_SIZE_M64          8
#define TYPE_SIZE_M128         16
#define TYPE_SIZE_ENUM         4
#define TYPE_SIZE_POINTER      8

#define TYPE_ALIGN_CHAR        1
#define TYPE_ALIGN_SHORT       2
#define TYPE_ALIGN_INT         4
#define TYPE_ALIGN_LONG        8
#define TYPE_ALIGN_LONG_LONG   8
#define TYPE_ALIGN_INT128      16
#define TYPE_ALIGN_FLOAT       4
#define TYPE_ALIGN_DOUBLE      8
#define TYPE_ALIGN_LONG_DOUBLE 16
#define TYPE_ALIGN_FLOAT128    16
#define TYPE_ALIGN_M64         8
#define TYPE_ALIGN_M128        16
#define TYPE_ALIGN_ENUM        4
#define TYPE_ALIGN_POINTER     8

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

/* Check structs and unions of all permutations of 3 basic types.  */
int
main (void)
{
  check_struct_and_union3(char, char, char, 3, 1);
  check_struct_and_union3(char, char, short, 4, 2);
  check_struct_and_union3(char, char, int, 8, 4);
  check_struct_and_union3(char, char, long, 16, 8);
  check_struct_and_union3(char, char, long long, 16, 8);
  check_struct_and_union3(char, char, float, 8, 4);
  check_struct_and_union3(char, char, double, 16, 8);
  check_struct_and_union3(char, char, long double, 32, 16);
  check_struct_and_union3(char, short, char, 6, 2);
  check_struct_and_union3(char, short, short, 6, 2);
  check_struct_and_union3(char, short, int, 8, 4);
  check_struct_and_union3(char, short, long, 16, 8);
  check_struct_and_union3(char, short, long long, 16, 8);
  check_struct_and_union3(char, short, float, 8, 4);
  check_struct_and_union3(char, short, double, 16, 8);
  check_struct_and_union3(char, short, long double, 32, 16);
  check_struct_and_union3(char, int, char, 12, 4);
  check_struct_and_union3(char, int, short, 12, 4);
  check_struct_and_union3(char, int, int, 12, 4);
  check_struct_and_union3(char, int, long, 16, 8);
  check_struct_and_union3(char, int, long long, 16, 8);
  check_struct_and_union3(char, int, float, 12, 4);
  check_struct_and_union3(char, int, double, 16, 8);
  check_struct_and_union3(char, int, long double, 32, 16);
  check_struct_and_union3(char, long, char, 24, 8);
  check_struct_and_union3(char, long, short, 24, 8);
  check_struct_and_union3(char, long, int, 24, 8);
  check_struct_and_union3(char, long, long, 24, 8);
  check_struct_and_union3(char, long, long long, 24, 8);
  check_struct_and_union3(char, long, float, 24, 8);
  check_struct_and_union3(char, long, double, 24, 8);
  check_struct_and_union3(char, long, long double, 32, 16);
  check_struct_and_union3(char, long long, char, 24, 8);
  check_struct_and_union3(char, long long, short, 24, 8);
  check_struct_and_union3(char, long long, int, 24, 8);
  check_struct_and_union3(char, long long, long, 24, 8);
  check_struct_and_union3(char, long long, long long, 24, 8);
  check_struct_and_union3(char, long long, float, 24, 8);
  check_struct_and_union3(char, long long, double, 24, 8);
  check_struct_and_union3(char, long long, long double, 32, 16);
  check_struct_and_union3(char, float, char, 12, 4);
  check_struct_and_union3(char, float, short, 12, 4);
  check_struct_and_union3(char, float, int, 12, 4);
  check_struct_and_union3(char, float, long, 16, 8);
  check_struct_and_union3(char, float, long long, 16, 8);
  check_struct_and_union3(char, float, float, 12, 4);
  check_struct_and_union3(char, float, double, 16, 8);
  check_struct_and_union3(char, float, long double, 32, 16);
  check_struct_and_union3(char, double, char, 24, 8);
  check_struct_and_union3(char, double, short, 24, 8);
  check_struct_and_union3(char, double, int, 24, 8);
  check_struct_and_union3(char, double, long, 24, 8);
  check_struct_and_union3(char, double, long long, 24, 8);
  check_struct_and_union3(char, double, float, 24, 8);
  check_struct_and_union3(char, double, double, 24, 8);
  check_struct_and_union3(char, double, long double, 32, 16);
  check_struct_and_union3(char, long double, char, 48, 16);
  check_struct_and_union3(char, long double, short, 48, 16);
  check_struct_and_union3(char, long double, int, 48, 16);
  check_struct_and_union3(char, long double, long, 48, 16);
  check_struct_and_union3(char, long double, long long, 48, 16);
  check_struct_and_union3(char, long double, float, 48, 16);
  check_struct_and_union3(char, long double, double, 48, 16);
  check_struct_and_union3(char, long double, long double, 48, 16);
  check_struct_and_union3(short, char, char, 4, 2);
  check_struct_and_union3(short, char, short, 6, 2);
  check_struct_and_union3(short, char, int, 8, 4);
  check_struct_and_union3(short, char, long, 16, 8);
  check_struct_and_union3(short, char, long long, 16, 8);
  check_struct_and_union3(short, char, float, 8, 4);
  check_struct_and_union3(short, char, double, 16, 8);
  check_struct_and_union3(short, char, long double, 32, 16);
  check_struct_and_union3(short, short, char, 6, 2);
  check_struct_and_union3(short, short, short, 6, 2);
  check_struct_and_union3(short, short, int, 8, 4);
  check_struct_and_union3(short, short, long, 16, 8);
  check_struct_and_union3(short, short, long long, 16, 8);
  check_struct_and_union3(short, short, float, 8, 4);
  check_struct_and_union3(short, short, double, 16, 8);
  check_struct_and_union3(short, short, long double, 32, 16);
  check_struct_and_union3(short, int, char, 12, 4);
  check_struct_and_union3(short, int, short, 12, 4);
  check_struct_and_union3(short, int, int, 12, 4);
  check_struct_and_union3(short, int, long, 16, 8);
  check_struct_and_union3(short, int, long long, 16, 8);
  check_struct_and_union3(short, int, float, 12, 4);
  check_struct_and_union3(short, int, double, 16, 8);
  check_struct_and_union3(short, int, long double, 32, 16);
  check_struct_and_union3(short, long, char, 24, 8);
  check_struct_and_union3(short, long, short, 24, 8);
  check_struct_and_union3(short, long, int, 24, 8);
  check_struct_and_union3(short, long, long, 24, 8);
  check_struct_and_union3(short, long, long long, 24, 8);
  check_struct_and_union3(short, long, float, 24, 8);
  check_struct_and_union3(short, long, double, 24, 8);
  check_struct_and_union3(short, long, long double, 32, 16);
  check_struct_and_union3(short, long long, char, 24, 8);
  check_struct_and_union3(short, long long, short, 24, 8);
  check_struct_and_union3(short, long long, int, 24, 8);
  check_struct_and_union3(short, long long, long, 24, 8);
  check_struct_and_union3(short, long long, long long, 24, 8);
  check_struct_and_union3(short, long long, float, 24, 8);
  check_struct_and_union3(short, long long, double, 24, 8);
  check_struct_and_union3(short, long long, long double, 32, 16);
  check_struct_and_union3(short, float, char, 12, 4);
  check_struct_and_union3(short, float, short, 12, 4);
  check_struct_and_union3(short, float, int, 12, 4);
  check_struct_and_union3(short, float, long, 16, 8);
  check_struct_and_union3(short, float, long long, 16, 8);
  check_struct_and_union3(short, float, float, 12, 4);
  check_struct_and_union3(short, float, double, 16, 8);
  check_struct_and_union3(short, float, long double, 32, 16);
  check_struct_and_union3(short, double, char, 24, 8);
  check_struct_and_union3(short, double, short, 24, 8);
  check_struct_and_union3(short, double, int, 24, 8);
  check_struct_and_union3(short, double, long, 24, 8);
  check_struct_and_union3(short, double, long long, 24, 8);
  check_struct_and_union3(short, double, float, 24, 8);
  check_struct_and_union3(short, double, double, 24, 8);
  check_struct_and_union3(short, double, long double, 32, 16);
  check_struct_and_union3(short, long double, char, 48, 16);
  check_struct_and_union3(short, long double, short, 48, 16);
  check_struct_and_union3(short, long double, int, 48, 16);
  check_struct_and_union3(short, long double, long, 48, 16);
  check_struct_and_union3(short, long double, long long, 48, 16);
  check_struct_and_union3(short, long double, float, 48, 16);
  check_struct_and_union3(short, long double, double, 48, 16);
  check_struct_and_union3(short, long double, long double, 48, 16);
  check_struct_and_union3(int, char, char, 8, 4);
  check_struct_and_union3(int, char, short, 8, 4);
  check_struct_and_union3(int, char, int, 12, 4);
  check_struct_and_union3(int, char, long, 16, 8);
  check_struct_and_union3(int, char, long long, 16, 8);
  check_struct_and_union3(int, char, float, 12, 4);
  check_struct_and_union3(int, char, double, 16, 8);
  check_struct_and_union3(int, char, long double, 32, 16);
  check_struct_and_union3(int, short, char, 8, 4);
  check_struct_and_union3(int, short, short, 8, 4);
  check_struct_and_union3(int, short, int, 12, 4);
  check_struct_and_union3(int, short, long, 16, 8);
  check_struct_and_union3(int, short, long long, 16, 8);
  check_struct_and_union3(int, short, float, 12, 4);
  check_struct_and_union3(int, short, double, 16, 8);
  check_struct_and_union3(int, short, long double, 32, 16);
  check_struct_and_union3(int, int, char, 12, 4);
  check_struct_and_union3(int, int, short, 12, 4);
  check_struct_and_union3(int, int, int, 12, 4);
  check_struct_and_union3(int, int, long, 16, 8);
  check_struct_and_union3(int, int, long long, 16, 8);
  check_struct_and_union3(int, int, float, 12, 4);
  check_struct_and_union3(int, int, double, 16, 8);
  check_struct_and_union3(int, int, long double, 32, 16);
  check_struct_and_union3(int, long, char, 24, 8);
  check_struct_and_union3(int, long, short, 24, 8);
  check_struct_and_union3(int, long, int, 24, 8);
  check_struct_and_union3(int, long, long, 24, 8);
  check_struct_and_union3(int, long, long long, 24, 8);
  check_struct_and_union3(int, long, float, 24, 8);
  check_struct_and_union3(int, long, double, 24, 8);
  check_struct_and_union3(int, long, long double, 32, 16);
  check_struct_and_union3(int, long long, char, 24, 8);
  check_struct_and_union3(int, long long, short, 24, 8);
  check_struct_and_union3(int, long long, int, 24, 8);
  check_struct_and_union3(int, long long, long, 24, 8);
  check_struct_and_union3(int, long long, long long, 24, 8);
  check_struct_and_union3(int, long long, float, 24, 8);
  check_struct_and_union3(int, long long, double, 24, 8);
  check_struct_and_union3(int, long long, long double, 32, 16);
  check_struct_and_union3(int, float, char, 12, 4);
  check_struct_and_union3(int, float, short, 12, 4);
  check_struct_and_union3(int, float, int, 12, 4);
  check_struct_and_union3(int, float, long, 16, 8);
  check_struct_and_union3(int, float, long long, 16, 8);
  check_struct_and_union3(int, float, float, 12, 4);
  check_struct_and_union3(int, float, double, 16, 8);
  check_struct_and_union3(int, float, long double, 32, 16);
  check_struct_and_union3(int, double, char, 24, 8);
  check_struct_and_union3(int, double, short, 24, 8);
  check_struct_and_union3(int, double, int, 24, 8);
  check_struct_and_union3(int, double, long, 24, 8);
  check_struct_and_union3(int, double, long long, 24, 8);
  check_struct_and_union3(int, double, float, 24, 8);
  check_struct_and_union3(int, double, double, 24, 8);
  check_struct_and_union3(int, double, long double, 32, 16);
  check_struct_and_union3(int, long double, char, 48, 16);
  check_struct_and_union3(int, long double, short, 48, 16);
  check_struct_and_union3(int, long double, int, 48, 16);
  check_struct_and_union3(int, long double, long, 48, 16);
  check_struct_and_union3(int, long double, long long, 48, 16);
  check_struct_and_union3(int, long double, float, 48, 16);
  check_struct_and_union3(int, long double, double, 48, 16);
  check_struct_and_union3(int, long double, long double, 48, 16);
  check_struct_and_union3(long, char, char, 16, 8);
  check_struct_and_union3(long, char, short, 16, 8);
  check_struct_and_union3(long, char, int, 16, 8);
  check_struct_and_union3(long, char, long, 24, 8);
  check_struct_and_union3(long, char, long long, 24, 8);
  check_struct_and_union3(long, char, float, 16, 8);
  check_struct_and_union3(long, char, double, 24, 8);
  check_struct_and_union3(long, char, long double, 32, 16);
  check_struct_and_union3(long, short, char, 16, 8);
  check_struct_and_union3(long, short, short, 16, 8);
  check_struct_and_union3(long, short, int, 16, 8);
  check_struct_and_union3(long, short, long, 24, 8);
  check_struct_and_union3(long, short, long long, 24, 8);
  check_struct_and_union3(long, short, float, 16, 8);
  check_struct_and_union3(long, short, double, 24, 8);
  check_struct_and_union3(long, short, long double, 32, 16);
  check_struct_and_union3(long, int, char, 16, 8);
  check_struct_and_union3(long, int, short, 16, 8);
  check_struct_and_union3(long, int, int, 16, 8);
  check_struct_and_union3(long, int, long, 24, 8);
  check_struct_and_union3(long, int, long long, 24, 8);
  check_struct_and_union3(long, int, float, 16, 8);
  check_struct_and_union3(long, int, double, 24, 8);
  check_struct_and_union3(long, int, long double, 32, 16);
  check_struct_and_union3(long, long, char, 24, 8);
  check_struct_and_union3(long, long, short, 24, 8);
  check_struct_and_union3(long, long, int, 24, 8);
  check_struct_and_union3(long, long, long, 24, 8);
  check_struct_and_union3(long, long, long long, 24, 8);
  check_struct_and_union3(long, long, float, 24, 8);
  check_struct_and_union3(long, long, double, 24, 8);
  check_struct_and_union3(long, long, long double, 32, 16);
  check_struct_and_union3(long, long long, char, 24, 8);
  check_struct_and_union3(long, long long, short, 24, 8);
  check_struct_and_union3(long, long long, int, 24, 8);
  check_struct_and_union3(long, long long, long, 24, 8);
  check_struct_and_union3(long, long long, long long, 24, 8);
  check_struct_and_union3(long, long long, float, 24, 8);
  check_struct_and_union3(long, long long, double, 24, 8);
  check_struct_and_union3(long, long long, long double, 32, 16);
  check_struct_and_union3(long, float, char, 16, 8);
  check_struct_and_union3(long, float, short, 16, 8);
  check_struct_and_union3(long, float, int, 16, 8);
  check_struct_and_union3(long, float, long, 24, 8);
  check_struct_and_union3(long, float, long long, 24, 8);
  check_struct_and_union3(long, float, float, 16, 8);
  check_struct_and_union3(long, float, double, 24, 8);
  check_struct_and_union3(long, float, long double, 32, 16);
  check_struct_and_union3(long, double, char, 24, 8);
  check_struct_and_union3(long, double, short, 24, 8);
  check_struct_and_union3(long, double, int, 24, 8);
  check_struct_and_union3(long, double, long, 24, 8);
  check_struct_and_union3(long, double, long long, 24, 8);
  check_struct_and_union3(long, double, float, 24, 8);
  check_struct_and_union3(long, double, double, 24, 8);
  check_struct_and_union3(long, double, long double, 32, 16);
  check_struct_and_union3(long, long double, char, 48, 16);
  check_struct_and_union3(long, long double, short, 48, 16);
  check_struct_and_union3(long, long double, int, 48, 16);
  check_struct_and_union3(long, long double, long, 48, 16);
  check_struct_and_union3(long, long double, long long, 48, 16);
  check_struct_and_union3(long, long double, float, 48, 16);
  check_struct_and_union3(long, long double, double, 48, 16);
  check_struct_and_union3(long, long double, long double, 48, 16);
  check_struct_and_union3(long long, char, char, 16, 8);
  check_struct_and_union3(long long, char, short, 16, 8);
  check_struct_and_union3(long long, char, int, 16, 8);
  check_struct_and_union3(long long, char, long, 24, 8);
  check_struct_and_union3(long long, char, long long, 24, 8);
  check_struct_and_union3(long long, char, float, 16, 8);
  check_struct_and_union3(long long, char, double, 24, 8);
  check_struct_and_union3(long long, char, long double, 32, 16);
  check_struct_and_union3(long long, short, char, 16, 8);
  check_struct_and_union3(long long, short, short, 16, 8);
  check_struct_and_union3(long long, short, int, 16, 8);
  check_struct_and_union3(long long, short, long, 24, 8);
  check_struct_and_union3(long long, short, long long, 24, 8);
  check_struct_and_union3(long long, short, float, 16, 8);
  check_struct_and_union3(long long, short, double, 24, 8);
  check_struct_and_union3(long long, short, long double, 32, 16);
  check_struct_and_union3(long long, int, char, 16, 8);
  check_struct_and_union3(long long, int, short, 16, 8);
  check_struct_and_union3(long long, int, int, 16, 8);
  check_struct_and_union3(long long, int, long, 24, 8);
  check_struct_and_union3(long long, int, long long, 24, 8);
  check_struct_and_union3(long long, int, float, 16, 8);
  check_struct_and_union3(long long, int, double, 24, 8);
  check_struct_and_union3(long long, int, long double, 32, 16);
  check_struct_and_union3(long long, long, char, 24, 8);
  check_struct_and_union3(long long, long, short, 24, 8);
  check_struct_and_union3(long long, long, int, 24, 8);
  check_struct_and_union3(long long, long, long, 24, 8);
  check_struct_and_union3(long long, long, long long, 24, 8);
  check_struct_and_union3(long long, long, float, 24, 8);
  check_struct_and_union3(long long, long, double, 24, 8);
  check_struct_and_union3(long long, long, long double, 32, 16);
  check_struct_and_union3(long long, long long, char, 24, 8);
  check_struct_and_union3(long long, long long, short, 24, 8);
  check_struct_and_union3(long long, long long, int, 24, 8);
  check_struct_and_union3(long long, long long, long, 24, 8);
  check_struct_and_union3(long long, long long, long long, 24, 8);
  check_struct_and_union3(long long, long long, float, 24, 8);
  check_struct_and_union3(long long, long long, double, 24, 8);
  check_struct_and_union3(long long, long long, long double, 32, 16);
  check_struct_and_union3(long long, float, char, 16, 8);
  check_struct_and_union3(long long, float, short, 16, 8);
  check_struct_and_union3(long long, float, int, 16, 8);
  check_struct_and_union3(long long, float, long, 24, 8);
  check_struct_and_union3(long long, float, long long, 24, 8);
  check_struct_and_union3(long long, float, float, 16, 8);
  check_struct_and_union3(long long, float, double, 24, 8);
  check_struct_and_union3(long long, float, long double, 32, 16);
  check_struct_and_union3(long long, double, char, 24, 8);
  check_struct_and_union3(long long, double, short, 24, 8);
  check_struct_and_union3(long long, double, int, 24, 8);
  check_struct_and_union3(long long, double, long, 24, 8);
  check_struct_and_union3(long long, double, long long, 24, 8);
  check_struct_and_union3(long long, double, float, 24, 8);
  check_struct_and_union3(long long, double, double, 24, 8);
  check_struct_and_union3(long long, double, long double, 32, 16);
  check_struct_and_union3(long long, long double, char, 48, 16);
  check_struct_and_union3(long long, long double, short, 48, 16);
  check_struct_and_union3(long long, long double, int, 48, 16);
  check_struct_and_union3(long long, long double, long, 48, 16);
  check_struct_and_union3(long long, long double, long long, 48, 16);
  check_struct_and_union3(long long, long double, float, 48, 16);
  check_struct_and_union3(long long, long double, double, 48, 16);
  check_struct_and_union3(long long, long double, long double, 48, 16);
  check_struct_and_union3(float, char, char, 8, 4);
  check_struct_and_union3(float, char, short, 8, 4);
  check_struct_and_union3(float, char, int, 12, 4);
  check_struct_and_union3(float, char, long, 16, 8);
  check_struct_and_union3(float, char, long long, 16, 8);
  check_struct_and_union3(float, char, float, 12, 4);
  check_struct_and_union3(float, char, double, 16, 8);
  check_struct_and_union3(float, char, long double, 32, 16);
  check_struct_and_union3(float, short, char, 8, 4);
  check_struct_and_union3(float, short, short, 8, 4);
  check_struct_and_union3(float, short, int, 12, 4);
  check_struct_and_union3(float, short, long, 16, 8);
  check_struct_and_union3(float, short, long long, 16, 8);
  check_struct_and_union3(float, short, float, 12, 4);
  check_struct_and_union3(float, short, double, 16, 8);
  check_struct_and_union3(float, short, long double, 32, 16);
  check_struct_and_union3(float, int, char, 12, 4);
  check_struct_and_union3(float, int, short, 12, 4);
  check_struct_and_union3(float, int, int, 12, 4);
  check_struct_and_union3(float, int, long, 16, 8);
  check_struct_and_union3(float, int, long long, 16, 8);
  check_struct_and_union3(float, int, float, 12, 4);
  check_struct_and_union3(float, int, double, 16, 8);
  check_struct_and_union3(float, int, long double, 32, 16);
  check_struct_and_union3(float, long, char, 24, 8);
  check_struct_and_union3(float, long, short, 24, 8);
  check_struct_and_union3(float, long, int, 24, 8);
  check_struct_and_union3(float, long, long, 24, 8);
  check_struct_and_union3(float, long, long long, 24, 8);
  check_struct_and_union3(float, long, float, 24, 8);
  check_struct_and_union3(float, long, double, 24, 8);
  check_struct_and_union3(float, long, long double, 32, 16);
  check_struct_and_union3(float, long long, char, 24, 8);
  check_struct_and_union3(float, long long, short, 24, 8);
  check_struct_and_union3(float, long long, int, 24, 8);
  check_struct_and_union3(float, long long, long, 24, 8);
  check_struct_and_union3(float, long long, long long, 24, 8);
  check_struct_and_union3(float, long long, float, 24, 8);
  check_struct_and_union3(float, long long, double, 24, 8);
  check_struct_and_union3(float, long long, long double, 32, 16);
  check_struct_and_union3(float, float, char, 12, 4);
  check_struct_and_union3(float, float, short, 12, 4);
  check_struct_and_union3(float, float, int, 12, 4);
  check_struct_and_union3(float, float, long, 16, 8);
  check_struct_and_union3(float, float, long long, 16, 8);
  check_struct_and_union3(float, float, float, 12, 4);
  check_struct_and_union3(float, float, double, 16, 8);
  check_struct_and_union3(float, float, long double, 32, 16);
  check_struct_and_union3(float, double, char, 24, 8);
  check_struct_and_union3(float, double, short, 24, 8);
  check_struct_and_union3(float, double, int, 24, 8);
  check_struct_and_union3(float, double, long, 24, 8);
  check_struct_and_union3(float, double, long long, 24, 8);
  check_struct_and_union3(float, double, float, 24, 8);
  check_struct_and_union3(float, double, double, 24, 8);
  check_struct_and_union3(float, double, long double, 32, 16);
  check_struct_and_union3(float, long double, char, 48, 16);
  check_struct_and_union3(float, long double, short, 48, 16);
  check_struct_and_union3(float, long double, int, 48, 16);
  check_struct_and_union3(float, long double, long, 48, 16);
  check_struct_and_union3(float, long double, long long, 48, 16);
  check_struct_and_union3(float, long double, float, 48, 16);
  check_struct_and_union3(float, long double, double, 48, 16);
  check_struct_and_union3(float, long double, long double, 48, 16);
  check_struct_and_union3(double, char, char, 16, 8);
  check_struct_and_union3(double, char, short, 16, 8);
  check_struct_and_union3(double, char, int, 16, 8);
  check_struct_and_union3(double, char, long, 24, 8);
  check_struct_and_union3(double, char, long long, 24, 8);
  check_struct_and_union3(double, char, float, 16, 8);
  check_struct_and_union3(double, char, double, 24, 8);
  check_struct_and_union3(double, char, long double, 32, 16);
  check_struct_and_union3(double, short, char, 16, 8);
  check_struct_and_union3(double, short, short, 16, 8);
  check_struct_and_union3(double, short, int, 16, 8);
  check_struct_and_union3(double, short, long, 24, 8);
  check_struct_and_union3(double, short, long long, 24, 8);
  check_struct_and_union3(double, short, float, 16, 8);
  check_struct_and_union3(double, short, double, 24, 8);
  check_struct_and_union3(double, short, long double, 32, 16);
  check_struct_and_union3(double, int, char, 16, 8);
  check_struct_and_union3(double, int, short, 16, 8);
  check_struct_and_union3(double, int, int, 16, 8);
  check_struct_and_union3(double, int, long, 24, 8);
  check_struct_and_union3(double, int, long long, 24, 8);
  check_struct_and_union3(double, int, float, 16, 8);
  check_struct_and_union3(double, int, double, 24, 8);
  check_struct_and_union3(double, int, long double, 32, 16);
  check_struct_and_union3(double, long, char, 24, 8);
  check_struct_and_union3(double, long, short, 24, 8);
  check_struct_and_union3(double, long, int, 24, 8);
  check_struct_and_union3(double, long, long, 24, 8);
  check_struct_and_union3(double, long, long long, 24, 8);
  check_struct_and_union3(double, long, float, 24, 8);
  check_struct_and_union3(double, long, double, 24, 8);
  check_struct_and_union3(double, long, long double, 32, 16);
  check_struct_and_union3(double, long long, char, 24, 8);
  check_struct_and_union3(double, long long, short, 24, 8);
  check_struct_and_union3(double, long long, int, 24, 8);
  check_struct_and_union3(double, long long, long, 24, 8);
  check_struct_and_union3(double, long long, long long, 24, 8);
  check_struct_and_union3(double, long long, float, 24, 8);
  check_struct_and_union3(double, long long, double, 24, 8);
  check_struct_and_union3(double, long long, long double, 32, 16);
  check_struct_and_union3(double, float, char, 16, 8);
  check_struct_and_union3(double, float, short, 16, 8);
  check_struct_and_union3(double, float, int, 16, 8);
  check_struct_and_union3(double, float, long, 24, 8);
  check_struct_and_union3(double, float, long long, 24, 8);
  check_struct_and_union3(double, float, float, 16, 8);
  check_struct_and_union3(double, float, double, 24, 8);
  check_struct_and_union3(double, float, long double, 32, 16);
  check_struct_and_union3(double, double, char, 24, 8);
  check_struct_and_union3(double, double, short, 24, 8);
  check_struct_and_union3(double, double, int, 24, 8);
  check_struct_and_union3(double, double, long, 24, 8);
  check_struct_and_union3(double, double, long long, 24, 8);
  check_struct_and_union3(double, double, float, 24, 8);
  check_struct_and_union3(double, double, double, 24, 8);
  check_struct_and_union3(double, double, long double, 32, 16);
  check_struct_and_union3(double, long double, char, 48, 16);
  check_struct_and_union3(double, long double, short, 48, 16);
  check_struct_and_union3(double, long double, int, 48, 16);
  check_struct_and_union3(double, long double, long, 48, 16);
  check_struct_and_union3(double, long double, long long, 48, 16);
  check_struct_and_union3(double, long double, float, 48, 16);
  check_struct_and_union3(double, long double, double, 48, 16);
  check_struct_and_union3(double, long double, long double, 48, 16);
  check_struct_and_union3(long double, char, char, 32, 16);
  check_struct_and_union3(long double, char, short, 32, 16);
  check_struct_and_union3(long double, char, int, 32, 16);
  check_struct_and_union3(long double, char, long, 32, 16);
  check_struct_and_union3(long double, char, long long, 32, 16);
  check_struct_and_union3(long double, char, float, 32, 16);
  check_struct_and_union3(long double, char, double, 32, 16);
  check_struct_and_union3(long double, char, long double, 48, 16);
  check_struct_and_union3(long double, short, char, 32, 16);
  check_struct_and_union3(long double, short, short, 32, 16);
  check_struct_and_union3(long double, short, int, 32, 16);
  check_struct_and_union3(long double, short, long, 32, 16);
  check_struct_and_union3(long double, short, long long, 32, 16);
  check_struct_and_union3(long double, short, float, 32, 16);
  check_struct_and_union3(long double, short, double, 32, 16);
  check_struct_and_union3(long double, short, long double, 48, 16);
  check_struct_and_union3(long double, int, char, 32, 16);
  check_struct_and_union3(long double, int, short, 32, 16);
  check_struct_and_union3(long double, int, int, 32, 16);
  check_struct_and_union3(long double, int, long, 32, 16);
  check_struct_and_union3(long double, int, long long, 32, 16);
  check_struct_and_union3(long double, int, float, 32, 16);
  check_struct_and_union3(long double, int, double, 32, 16);
  check_struct_and_union3(long double, int, long double, 48, 16);
  check_struct_and_union3(long double, long, char, 32, 16);
  check_struct_and_union3(long double, long, short, 32, 16);
  check_struct_and_union3(long double, long, int, 32, 16);
  check_struct_and_union3(long double, long, long, 32, 16);
  check_struct_and_union3(long double, long, long long, 32, 16);
  check_struct_and_union3(long double, long, float, 32, 16);
  check_struct_and_union3(long double, long, double, 32, 16);
  check_struct_and_union3(long double, long, long double, 48, 16);
  check_struct_and_union3(long double, long long, char, 32, 16);
  check_struct_and_union3(long double, long long, short, 32, 16);
  check_struct_and_union3(long double, long long, int, 32, 16);
  check_struct_and_union3(long double, long long, long, 32, 16);
  check_struct_and_union3(long double, long long, long long, 32, 16);
  check_struct_and_union3(long double, long long, float, 32, 16);
  check_struct_and_union3(long double, long long, double, 32, 16);
  check_struct_and_union3(long double, long long, long double, 48, 16);
  check_struct_and_union3(long double, float, char, 32, 16);
  check_struct_and_union3(long double, float, short, 32, 16);
  check_struct_and_union3(long double, float, int, 32, 16);
  check_struct_and_union3(long double, float, long, 32, 16);
  check_struct_and_union3(long double, float, long long, 32, 16);
  check_struct_and_union3(long double, float, float, 32, 16);
  check_struct_and_union3(long double, float, double, 32, 16);
  check_struct_and_union3(long double, float, long double, 48, 16);
  check_struct_and_union3(long double, double, char, 32, 16);
  check_struct_and_union3(long double, double, short, 32, 16);
  check_struct_and_union3(long double, double, int, 32, 16);
  check_struct_and_union3(long double, double, long, 32, 16);
  check_struct_and_union3(long double, double, long long, 32, 16);
  check_struct_and_union3(long double, double, float, 32, 16);
  check_struct_and_union3(long double, double, double, 32, 16);
  check_struct_and_union3(long double, double, long double, 48, 16);
  check_struct_and_union3(long double, long double, char, 48, 16);
  check_struct_and_union3(long double, long double, short, 48, 16);
  check_struct_and_union3(long double, long double, int, 48, 16);
  check_struct_and_union3(long double, long double, long, 48, 16);
  check_struct_and_union3(long double, long double, long long, 48, 16);
  check_struct_and_union3(long double, long double, float, 48, 16);
  check_struct_and_union3(long double, long double, double, 48, 16);
  check_struct_and_union3(long double, long double, long double, 48, 16);
  return 0;
}
