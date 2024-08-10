/* Copyright (C) 2004  Free Software Foundation.

   Verify that built-in math function conversion into integer rounding
   functions is correctly performed by the compiler.

   Written by Kaveh ghazi, 2004-04-26.  */

/* { dg-do link } */
/* { dg-options "-ffast-math" } */

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

/* Macro to do all FP type combinations.  The second half tests
   narrowing the FP type.  */
#define TEST_FP2FIXED(FN1, FN2) \
  extern void link_error_##FN1##_##FN2(void); \
  extern void link_error_##FN1##f_##FN2##f(void); \
  extern void link_error_##FN1##l_##FN2##l(void); \
  extern void link_error_##FN1##_l##FN2(void); \
  extern void link_error_##FN1##f_l##FN2##f(void); \
  extern void link_error_##FN1##l_l##FN2##l(void); \
  if ((long)__builtin_##FN1(d) != __builtin_##FN2(d)) \
    link_error_##FN1##_##FN2(); \
  if ((long)__builtin_##FN1##f(f) != __builtin_##FN2##f(f)) \
    link_error_##FN1##f_##FN2##f(); \
  if ((long)__builtin_##FN1##l(ld) != __builtin_##FN2##l(ld)) \
    link_error_##FN1##l_##FN2##l(); \
  if ((long long)__builtin_##FN1(d) != __builtin_l##FN2(d)) \
    link_error_##FN1##_l##FN2(); \
  if ((long long)__builtin_##FN1##f(f) != __builtin_l##FN2##f(f)) \
    link_error_##FN1##f_l##FN2##f(); \
  if ((long long)__builtin_##FN1##l(ld) != __builtin_l##FN2##l(ld)) \
    link_error_##FN1##l_l##FN2##l(); \
  extern void link_error_##FN1##_##FN2##f(void); \
  extern void link_error_##FN1##l_##FN2(void); \
  extern void link_error_##FN1##l_##FN2##f(void); \
  extern void link_error_##FN1##_l##FN2##f(void); \
  extern void link_error_##FN1##l_l##FN2(void); \
  extern void link_error_##FN1##l_l##FN2##f(void); \
  if (sizeof(double) > sizeof(float) \
      && (long)__builtin_##FN1(f) != __builtin_##FN2##f(f)) \
    link_error_##FN1##_##FN2##f(); \
  if (sizeof(long double) > sizeof(double) \
      && (long)__builtin_##FN1##l(d) != __builtin_##FN2(d)) \
    link_error_##FN1##l_##FN2(); \
  if (sizeof(long double) > sizeof(float) \
      && (long)__builtin_##FN1##l(f) != __builtin_##FN2##f(f)) \
    link_error_##FN1##l_##FN2##f(); \
  if (sizeof(double) > sizeof(float) \
      && (long long)__builtin_##FN1(f) != __builtin_l##FN2##f(f)) \
    link_error_##FN1##_l##FN2##f(); \
  if (sizeof(long double) > sizeof(double) \
      && (long long)__builtin_##FN1##l(d) != __builtin_l##FN2(d)) \
    link_error_##FN1##l_l##FN2(); \
  if (sizeof(long double) > sizeof(float) \
      && (long long)__builtin_##FN1##l(f) != __builtin_l##FN2##f(f)) \
    link_error_##FN1##l_l##FN2##f()

void __attribute__ ((__noinline__)) foo (double d, float f, long double ld)
{
#ifdef __OPTIMIZE__
# ifdef HAVE_C99_RUNTIME
  /* The resulting transformation functions are all C99.  */
  TEST_FP2FIXED (ceil, lceil);
  TEST_FP2FIXED (floor, lfloor);
  TEST_FP2FIXED (round, lround);
  TEST_FP2FIXED (nearbyint, lrint);
  TEST_FP2FIXED (rint, lrint);
# endif
#endif
}

int main()
{
  foo (1.0, 2.0, 3.0);
  return 0;
}
