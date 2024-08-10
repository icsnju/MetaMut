/* Test variable number of arguments passed to functions.  */

#include <stdarg.h>
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
   No attempt is made to see if argument passing is done the right way.  */
void
__attribute__ ((noinline))
noprintf (char *format, ...)
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

extern void iamcu_noprintf (char *, ...);

int
main (void)
{
#ifdef CHECK_VARARGS
  float f = 258.0;
  struct arg al[5];

  al[0].type = ARG_INT;
  al[0].value.ivalue = 256;
  al[1].type = ARG_DOUBLE;
  al[1].value.dvalue = 257.0;
  al[2].type = ARG_POINTER;
  al[2].value.pvalue = al;
  al[3].type = ARG_DOUBLE;
  al[3].value.dvalue = f;
  al[4].type = ARG_INT;
  al[4].value.ivalue = 259;

  arglist = al;
  noprintf("%d%f%p%f%d", 256, 257.0, al, f, 259);

  iamcu_noprintf ((char *) 0xabadbeef, 256, 257.0,
		  (void *) 0xbbadbeef, f, 259);
#endif

  return 0;
}
