/* Copyright (C) 2005 Free Software Foundation.

   Check that (long)floor, (long)floorf, (long)floorl,
   (long long)floor, (long long)floorf, (long long)floorl,
   (long)ceil, (long)ceilf, (long)ceill,
   (long long)ceil, (long long)ceilf, (long long)ceill
   built-in functions compile.

   Written by Uros Bizjak, 5th April 2005.  */

/* { dg-do compile } */
/* { dg-options "-O2 -ffast-math" } */

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

extern double floor(double);
extern double ceil(double);
extern double trunc(double);

extern float floorf(float);
extern float ceilf(float);
extern float truncf(float);

extern long double floorl(long double);
extern long double ceill(long double);
extern long double truncl(long double);


long int test1(double x)
{
  return floor(x);
}

long long int test2(double x)
{
  return floor(x);
}

long int test3(double x)
{
  return ceil(x);
}

long long int test4(double x)
{
  return ceil(x);
}

long int test5(double x)
{
  return trunc(x);
}

long long int test6(double x)
{
  return trunc(x);
}

#ifdef HAVE_C99_RUNTIME
long int test1f(float x)
{
  return floorf(x);
}

long long int test2f(float x)
{
  return floorf(x);
}

long int test3f(float x)
{
  return ceilf(x);
}

long long int test4f(float x)
{
  return ceilf(x);
}
#endif

long int test5f(float x)
{
  return truncf(x);
}

long long int test6f(float x)
{
  return truncf(x);
}

#ifdef HAVE_C99_RUNTIME
long int test1l(long double x)
{
  return floorl(x);
}

long long int test2l(long double x)
{
  return floorl(x);
}

long int test3l(long double x)
{
  return ceill(x);
}

long long int test4l(long double x)
{
  return ceill(x);
}
#endif

long int test5l(long double x)
{
  return truncl(x);
}

long long int test6l(long double x)
{
  return truncl(x);
}
