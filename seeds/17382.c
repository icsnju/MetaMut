/* Test variable number of arguments passed to functions. For now this is
   just a simple test to see if it's working.  */

#include <stdarg.h>
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


#define ARG_INT     1
#define ARG_DOUBLE  2
#define ARG_POINTER 3

union types
{
  int ivalue;
  double dvalue;
  void *pvalue;
};

struct arg
{
  int type;
  union types value;
};

struct arg *arglist;

/* This tests the argumentlist to see if it matches the format string which
   is printf-like. Nothing will be printed of course. It can handle ints,
   doubles and void pointers. The given value will be tested against the
   values given in arglist.  
   This test only assures that the variable argument passing is working.
   No attempt is made to see if argument passing is done the right way.
   Since the ABI doesn't say how it's done, checking this is not really
   relevant.  */
void
my_noprintf (char *format, ...)
{
  va_list va_arglist;
  char *c;

  int ivalue;
  double dvalue;
  void *pvalue;
  struct arg *argp = arglist;

  va_start (va_arglist, format);
  for (c = format; *c; c++)
    if (*c == '%')
      {
	switch (*++c)
	  {
	  case 'd':
	    assert (argp->type == ARG_INT);
	    ivalue = va_arg (va_arglist, int);
	    assert (argp->value.ivalue == ivalue);
	    break;
	  case 'f':
	    assert (argp->type == ARG_DOUBLE);
	    dvalue = va_arg (va_arglist, double);
	    assert (argp->value.dvalue == dvalue);
	    break;
	  case 'p':
	    assert (argp->type == ARG_POINTER);
	    pvalue = va_arg (va_arglist, void *);
	    assert (argp->value.pvalue == pvalue);
	    break;
	  default:
	    abort ();
	  }

	argp++;
      }
}

int
main (void)
{
#ifdef CHECK_VARARGS
  struct arg al[5];

  al[0].type = ARG_INT;
  al[0].value.ivalue = 256;
  al[1].type = ARG_DOUBLE;
  al[1].value.dvalue = 257.0;
  al[2].type = ARG_POINTER;
  al[2].value.pvalue = al;
  al[3].type = ARG_DOUBLE;
  al[3].value.dvalue = 258.0;
  al[4].type = ARG_INT;
  al[4].value.ivalue = 259;

  arglist = al;
  my_noprintf("%d%f%p%f%d", 256, 257.0, al, 258.0, 259);
#endif

  return 0;
}
