/* This is a small test to see if bitfields are working.  It is only a
   few structs and a union and a test to see if they have the correct
   size, if values can be read and written and a couple of argument
   passing tests.  No alignment testing is done.  */

#ifndef DEFINED_DEFINES_H
#define DEFINED_DEFINES_H

typedef unsigned long long ulonglong;
typedef long double ldouble;

/* These defines determines what part of the test should be run.  When
   GCC implements these parts, the defines should be uncommented to
   enable testing.  */

/* Scalar type long double.  */
#define CHECK_LONG_DOUBLE

/* Scalar type __float128.  */
#define CHECK_FLOAT128

/* Returning of complex type.  */
#define CHECK_COMPLEX

/* Structs with size > 8.  */
#define CHECK_LARGER_STRUCTS

/* Checks for passing floats and doubles.  */
#define CHECK_FLOAT_DOUBLE_PASSING

/* Union passing with not-extremely-simple unions.  */
#define CHECK_LARGER_UNION_PASSING

/* Variable args.  */
#define CHECK_VARARGS

/* Check argument passing and returning for scalar types with sizeof > 8.  */
#define CHECK_LARGE_SCALAR_PASSING

/* Defines for sizing and alignment.  */

#define TYPE_SIZE_CHAR         1
#define TYPE_SIZE_SHORT        2
#define TYPE_SIZE_INT          4
#define TYPE_SIZE_LONG         4
#define TYPE_SIZE_LONG_LONG    8
#define TYPE_SIZE_FLOAT        4
#define TYPE_SIZE_DOUBLE       8
#define TYPE_SIZE_LONG_DOUBLE  8
#define TYPE_SIZE_FLOAT128     16
#define TYPE_SIZE_ENUM         4
#define TYPE_SIZE_POINTER      4

#define TYPE_ALIGN_CHAR        1
#define TYPE_ALIGN_SHORT       2
#define TYPE_ALIGN_INT         4
#define TYPE_ALIGN_LONG        4
#define TYPE_ALIGN_LONG_LONG   4
#define TYPE_ALIGN_FLOAT       4
#define TYPE_ALIGN_DOUBLE      4
#define TYPE_ALIGN_LONG_DOUBLE 4
#define TYPE_ALIGN_FLOAT128    4
#define TYPE_ALIGN_ENUM        4
#define TYPE_ALIGN_POINTER     4

/* These defines control the building of the list of types to check. There
   is a string identifying the type (with a comma after), a size of the type
   (also with a comma and an integer for adding to the total amount of types)
   and an alignment of the type (which is currently not really needed since
   the abi specifies that alignof == sizeof for all scalar types).  */
#ifdef CHECK_LONG_DOUBLE
#define CLD_STR "long double",
#define CLD_SIZ TYPE_SIZE_LONG_DOUBLE,
#define CLD_ALI TYPE_ALIGN_LONG_DOUBLE,
#define CLD_RET "???",
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


/* These five bitfields are taken from the System V ABI, Intel 386
   architecture supplement.  */

/* Word aligned, sizeof is 4.  */
struct RightToLeft
{
  int j:5;
  int k:6;
  int m:7;
};

/* Word aligned, sizeof is 12.  */
struct BoundaryAlignment
{
  short s:9;
  int   j:9;
  char  c;
  short t:9;
  short u:9;
  char  d;
};

/* Halfword aligned, sizeof is 2.  */
struct StorageUnitSharing
{
  char  c;
  short s:8;
};

/* Halfword aligned, sizeof is 2.  */
union Allocation
{
  char  c;
  short s:8;
};

/* Byte aligned, sizeof is 9.  */
struct Unnamed
{
  char  c;
  int    :0;
  char  d;
  short  :9;
  char  e;
  char   :0;
};

/* Extra struct testing bitfields in larger types.
   Doubleword aligned, sizeof is 8.  */
struct LargerTypes
{
  long long l:33;
  int       i:31;
};


void
passing1 (struct RightToLeft str, int j, int k, int m)
{
  assert (str.j == j);
  assert (str.k == k);
  assert (str.m == m);
}

void
passing2 (struct BoundaryAlignment str, short s, int j, char c, short t,
	  short u, char d)
{
  assert (str.s == s);
  assert (str.j == j);
  assert (str.c == c);
  assert (str.t == t);
  assert (str.u == u);
  assert (str.d == d);
}

void
passing3 (struct StorageUnitSharing str, char c, short s)
{
  assert (str.c == c);
  assert (str.s == s);
}

void
passing4 (struct Unnamed str, char c, char d, char e)
{
  assert (str.c == c);
  assert (str.d == d);
  assert (str.e == e);
}

void
passing5 (struct LargerTypes str, long long l, int i)
{
  assert (str.l == l);
  assert (str.i == i);
}


void
passingU (union Allocation u, char c)
{
  assert (u.c == c);
  assert (u.s == c);
}


int
main (void)
{
  struct RightToLeft str1;
  struct BoundaryAlignment str2;
  struct StorageUnitSharing str3;
  struct Unnamed str4;
  struct LargerTypes str5;
  union Allocation u;

  /* Check sizeof's.  */
  check_size(str1, 4);
  check_size(str2, 12);
  check_size(str3, 2);
  check_size(str4, 9);
  check_size(str5, 8);
  check_size(u, 2);

  /* Check alignof's.  */
  check_align_lv(str1, 4);
  check_align_lv(str2, 4);
  check_align_lv(str3, 2);
  check_align_lv(str4, 1);
  check_align_lv(str5, 4);
  check_align_lv(u, 2);

  /* Check passing.  */
  str1.j = str2.s = str3.c = str4.c = str5.l = 4;
  str1.k = str2.j = str3.s = str4.d = str5.i = 5;
  str1.m = str2.c = str4.e = 6;
  str2.t = 7;
  str2.u = 8;
  str2.d = 9;
  passing1 (str1, 4, 5, 6);
  passing2 (str2, 4, 5, 6, 7, 8, 9);
  passing3 (str3, 4, 5);
  passing4 (str4, 4, 5, 6);
  passing5 (str5, 4, 5);

  u.c = 5;
  passingU (u, 5);
  u.s = 6;
  passingU (u, 6);

  return 0;
}
