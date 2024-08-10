/* Copyright (C) 2004  Free Software Foundation.

   Verify that built-in folding of various math "power" functions is
   correctly performed by the compiler.

   Written by Kaveh Ghazi, 2004-03-11.  */

/* { dg-do link } */
/* { dg-options "-ffast-math" } */
/* { dg-skip-if "PR44214" { *-*-* } { "-O0" } { "" } } */

/* Copyright (C) 2003, 2004, 2005, 2006, 2009, 2011, 2012, 2019
   Free Software Foundation.

   Define macros useful in tests for bulitin functions.  */

/* Define HAVE_C99_RUNTIME if the entire C99 runtime is available on
   the target system.  The value of HAVE_C99_RUNTIME should be the
   same as the value of TARGET_C99_FUNCTIONS in the GCC machine
   description.  (Perhaps GCC should predefine a special macro
   indicating whether or not TARGET_C99_FUNCTIONS is set, but it does
   not presently do that.)  */

#if defined(__hppa) && defined(__hpux)
/* PA HP-UX doesn't have the entire C99 runtime.  */
#elif defined(__INTERIX)
/* Interix6 doesn't have the entire C99 runtime.  */
#elif defined(__AVR__)
/* AVR doesn't have the entire C99 runtime.  */
#elif defined(__FreeBSD__) && (__FreeBSD__ < 9)
/* FreeBSD up to version 8 lacks support for cexp and friends.  */
#elif defined(__vxworks)
/* VxWorks doesn't have a full C99 time.  (cabs is missing, for example.)  */
#elif defined (__BPF__)
/* No chance for eBPF to support C99 functions.  */
#elif defined(_WIN32) && !defined(__CYGWIN__)
/* Windows doesn't have the entire C99 runtime.  */
#elif (defined(__APPLE__) && defined(__ppc__) \
       && ! defined (__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__))
/* MacOS versions before 10.3 don't have many C99 functions.  
   But, if you're including this file, you probably want to test the
   newer behavior, so: */
#error forgot to set -mmacosx-version-min.
#elif (defined(__APPLE__) && defined(__ppc__) \
       && __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1030)
/* MacOS versions before 10.3 don't have many C99 functions.  */
#else
/* Newlib has the "f" variants of the math functions, but not the "l"
   variants.  TARGET_C99_FUNCTIONS is only defined if all C99
   functions are present.  Therefore, on systems using newlib, tests
   of builtins will fail the "l" variants, and we should therefore not
   define HAVE_C99_RUNTIME.  Including <sys/types.h> gives us a way of
   seeing if _NEWLIB_VERSION is defined.  Including <math.h> would work
   too, but the GLIBC math inlines cause us to generate inferior code,
   which causes the test to fail, so it is not safe.  Including <limits.h>
   also fails because the include search paths are ordered such that GCC's
   version will be found before the newlib version.  Similarly, uClibc
   lacks the C99 functions.  */
#include <sys/types.h>
#if defined(_NEWLIB_VERSION) || defined(__UCLIBC__)
#elif defined(__sun) && __STDC_VERSION__ - 0 < 199901L
/* If you're including this file, you probably want to test the newer
   behavior, so ensure the right flags were used for each test: */
#error forgot to set -std=c99.
#elif defined(__sun) && ! defined (_STDC_C99)
/* Solaris up to 9 doesn't have the entire C99 runtime.
   Solaris 10 defines _STDC_C99 if __STDC_VERSION__ is >= 199901L.
   This macro is defined in <sys/feature_tests.h> which is included by
   various system headers, in this case <sys/types.h> above.  */
#else
#define HAVE_C99_RUNTIME
#endif
#endif

#ifdef HAVE_C99_RUNTIME
#define C99CODE(CODE) CODE
#else
#define C99CODE(CODE) 0
#endif

#define PROTOTYPE(FN) extern double FN(double); extern float FN##f(float); \
  extern long double FN##l(long double);
#define PROTOTYPE2(FN) extern double FN(double, double); \
  extern float FN##f(float, float); \
  extern long double FN##l(long double, long double);

PROTOTYPE(fabs)
PROTOTYPE(sqrt)
PROTOTYPE(cbrt)
PROTOTYPE2(pow)

void test(double d1, double d2, double d3,
	  float f1, float f2, float f3,
	  long double ld1, long double ld2, long double ld3)
{
  /* Test N1root(N2root(x)) -> pow(x,1/(N1*N2)).  */
  /* E.g. sqrt(cbrt(x)) -> pow(x,1/6).  */
  /* The `ABS' argument is `fabs' when the transformation only works
     for nonnegative arguments.  Otherwise it's blank.  */
#define ROOT_ROOT(FN1,N1,FN2,N2,ABS) \
 extern void link_failure_##FN1##_##FN2(void); \
 if (FN1(FN2(ABS(d1))) != pow(ABS(d1),1.0/(N1*N2)) \
     || C99CODE (FN1##f(FN2##f(ABS(f1))) != powf(ABS(f1),1.0F/(N1*N2))) \
     || C99CODE (FN1##l(FN2##l(ABS(ld1))) != powl(ABS(ld1),1.0L/(N1*N2)))) \
    link_failure_##FN1##_##FN2()

  ROOT_ROOT(sqrt,2,sqrt,2,);
  ROOT_ROOT(sqrt,2,cbrt,3,);
  ROOT_ROOT(cbrt,3,sqrt,2,);
  ROOT_ROOT(cbrt,3,cbrt,3,fabs);

  /* Test pow(Nroot(x),y) -> pow(x,y/N).  */
  /* The `ABS' argument is `fabs' when the transformation only works
     for nonnegative arguments.  Otherwise it's blank.  */
#define POW_ROOT(FN,N,ABS) \
 extern void link_failure_pow_##FN(void); \
 if (pow(FN(ABS(d1)), d2) != pow(ABS(d1),d2/N) \
     || powf(FN##f(ABS(f1)),f2) != powf(ABS(f1),f2/N) \
     || powl(FN##l(ABS(ld1)),ld2) != powl(ABS(ld1),ld2/N)) \
    link_failure_pow_##FN()

  POW_ROOT(sqrt,2,);
  POW_ROOT(cbrt,3,fabs);

  /* Test Nroot(pow(x,y)) -> pow(x,y/N).  */
  /* The `ABS' argument is `fabs' when the transformation only works
     for nonnegative arguments.  Otherwise it's blank.  */
#define ROOT_POW(FN,N,ABS) \
 extern void link_failure_##FN##_pow(void); \
 if (FN(pow(ABS(d1), d2)) != pow(ABS(d1),d2/N) \
     || FN##f(powf(ABS(f1),f2)) != powf(ABS(f1),f2/N) \
     || FN##l(powl(ABS(ld1),ld2)) != powl(ABS(ld1),ld2/N)) \
    link_failure_##FN##_pow()

  ROOT_POW(sqrt,2,fabs);
  ROOT_POW(cbrt,3,fabs);

  /* Test pow(pow(x,y),z) -> pow(x,y*z).  */
#define POW_POW \
 extern void link_failure_pow_pow(void); \
 if (pow(pow(fabs(d1), d2), d3) != pow(fabs(d1),d2*d3) \
     || powf(powf(fabs(f1),f2),f3) != powf(fabs(f1),f2*f3) \
     || powl(powl(fabs(ld1),ld2),ld3) != powl(fabs(ld1),ld2*ld3)) \
    link_failure_pow_pow()

  POW_POW;

  /* Test Nroot(x)*Nroot(y) -> Nroot(x*y).  */
#define ROOT_X_ROOT(FN) \
 extern void link_failure_root_x_root(void); \
 if (FN(d1)*FN(d2) != FN(d1*d2) \
     || FN##f(f1)*FN##f(f2) != FN##f(f1*f2) \
     || FN##l(ld1)*FN##l(ld2) != FN##l(ld1*ld2)) \
    link_failure_root_x_root()

  ROOT_X_ROOT(sqrt);
  ROOT_X_ROOT(cbrt);
  
  /* Test pow(x,y)*pow(x,z) -> pow(x,y+z).  */
#define POW_X_POW \
 extern void link_failure_pow_x_pow(void); \
 if (pow(d1,d2)*pow(d1,d3) != pow(d1,d2+d3) \
     || powf(f1,f2)*powf(f1,f3) != powf(f1,f2+f3) \
     || powl(ld1,ld2)*powl(ld1,ld3) != powl(ld1,ld2+ld3)) \
    link_failure_pow_x_pow()

  POW_X_POW;
  
}

int main (void)
{
  return 0;
}
