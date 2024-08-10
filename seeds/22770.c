/* Copyright (C) 2004  Free Software Foundation.

   Verify that built-in math function conversion to smaller FP types
   is correctly performed by the compiler.

   Written by Kaveh Ghazi, 2004-03-17.  */

/* { dg-do link } */
/* { dg-options "-ffast-math" } */
/* { dg-require-effective-target c99_runtime } */

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

/* This check is necessary when converting to a C99 function.  */
#ifdef HAVE_C99_RUNTIME
#define C99CODE(CODE) (CODE)
#define MAYBEC99(CODE, C99) (CODE)
#else
#define C99CODE(CODE) 0
#define MAYBEC99(CODE, C99) (!(C99) && (CODE))
#endif

void test(double d1, float f1, long double ld1)
{
  /* Test converting math builtins to narrower FP types based on a
     narrowing cast on the outside of the call.  MATHFN is the
     function to test, and C99 is 0/1 depending on whether the
     `double' version of MATHFN is a C99 function.  The optimization
     is only performed if the replacement function is actually
     narrower in width, so check that first.  */
#define OUTER_CAST1(MATHFN, C99) \
 extern void link_failure_outer_##MATHFN##l_##MATHFN##_1(void); \
 extern void link_failure_outer_##MATHFN##l_##MATHFN##_2(void); \
 extern void link_failure_outer_##MATHFN##l_##MATHFN##f_1(void); \
 extern void link_failure_outer_##MATHFN##l_##MATHFN##f_2(void); \
 extern void link_failure_outer_##MATHFN##_##MATHFN##f_1(void); \
 extern void link_failure_outer_##MATHFN##_##MATHFN##f_2(void); \
 if (sizeof (long double) > sizeof (double) \
     && MAYBEC99 ((double) __builtin_##MATHFN##l((double)ld1) != __builtin_##MATHFN(ld1), C99)) \
    link_failure_outer_##MATHFN##l_##MATHFN##_1(); \
 if (sizeof (long double) > sizeof (double) \
     && MAYBEC99 ((double) __builtin_##MATHFN##l(d1) != __builtin_##MATHFN(d1), C99)) \
    link_failure_outer_##MATHFN##l_##MATHFN##_1(); \
 if (sizeof (long double) > sizeof (double) \
     && MAYBEC99 ((double) __builtin_##MATHFN##l(f1) != __builtin_##MATHFN(f1), C99)) \
    link_failure_outer_##MATHFN##l_##MATHFN##_2(); \
 if (sizeof (long double) > sizeof (float) \
     && C99CODE ((float) __builtin_##MATHFN##l((float) ld1) != __builtin_##MATHFN##f(ld1))) \
    link_failure_outer_##MATHFN##l_##MATHFN##f_1(); \
 if (sizeof (long double) > sizeof (float) \
     && C99CODE ((float) __builtin_##MATHFN##l((float) d1) != __builtin_##MATHFN##f(d1))) \
    link_failure_outer_##MATHFN##l_##MATHFN##f_1(); \
 if (sizeof (long double) > sizeof (float) \
     && C99CODE ((float) __builtin_##MATHFN##l(f1) != __builtin_##MATHFN##f(f1))) \
    link_failure_outer_##MATHFN##l_##MATHFN##f_2(); \
 if (sizeof (double) > sizeof (float) \
     && C99CODE ((float) __builtin_##MATHFN((float) ld1) != __builtin_##MATHFN##f(ld1))) \
    link_failure_outer_##MATHFN##_##MATHFN##f_1(); \
 if (sizeof (double) > sizeof (float) \
     && C99CODE ((float) __builtin_##MATHFN((float) d1) != __builtin_##MATHFN##f(d1))) \
    link_failure_outer_##MATHFN##_##MATHFN##f_1(); \
 if (sizeof (double) > sizeof (float) \
     && C99CODE ((float) __builtin_##MATHFN(f1) != __builtin_##MATHFN##f(f1))) \
    link_failure_outer_##MATHFN##_##MATHFN##f_2()

  /* Test converting math builtins to narrower FP types based on if
     the argument is a narrower type (perhaps implicitly) cast to a
     wider one.  */
#define INNER_CAST1(MATHFN, C99) \
 extern void link_failure_inner_##MATHFN##l_##MATHFN(void); \
 extern void link_failure_inner_##MATHFN##l_##MATHFN##f(void); \
 extern void link_failure_inner_##MATHFN##_##MATHFN##f(void); \
 if (sizeof (long double) > sizeof (double) \
     && MAYBEC99 (__builtin_##MATHFN##l(d1) != (long double) __builtin_##MATHFN(d1), C99)) \
    link_failure_inner_##MATHFN##l_##MATHFN(); \
 if (sizeof (long double) > sizeof (float) \
     && C99CODE (__builtin_##MATHFN##l(f1) != (long double) __builtin_##MATHFN##f(f1))) \
    link_failure_inner_##MATHFN##l_##MATHFN##f(); \
 if (sizeof (long double) > sizeof (float) \
     && C99CODE (__builtin_##MATHFN##l((double)f1) != (long double) __builtin_##MATHFN##f(f1))) \
    link_failure_inner_##MATHFN##l_##MATHFN##f(); \
 if (sizeof (double) > sizeof (float) \
     && C99CODE (__builtin_##MATHFN(f1) != (double) __builtin_##MATHFN##f(f1))) \
    link_failure_inner_##MATHFN##_##MATHFN##f()


#ifdef __OPTIMIZE__
  OUTER_CAST1 (acos, /*C99=*/ 0);
  OUTER_CAST1 (acosh, /*C99=*/ 1);
  OUTER_CAST1 (asin, /*C99=*/ 1);
  OUTER_CAST1 (asinh, /*C99=*/ 1);
  OUTER_CAST1 (atan, /*C99=*/ 0);
  OUTER_CAST1 (atanh, /*C99=*/ 1);
  OUTER_CAST1 (cbrt, /*C99=*/ 1);
  OUTER_CAST1 (cos, /*C99=*/ 0);
  OUTER_CAST1 (cosh, /*C99=*/ 0);
  OUTER_CAST1 (erf, /*C99=*/ 1);
  OUTER_CAST1 (erfc, /*C99=*/ 1);
  OUTER_CAST1 (exp, /*C99=*/ 0);
  OUTER_CAST1 (exp2, /*C99=*/ 1);
  OUTER_CAST1 (expm1, /*C99=*/ 1);
  OUTER_CAST1 (fabs, /*C99=*/ 0);
  OUTER_CAST1 (log, /*C99=*/ 0);
  OUTER_CAST1 (log10, /*C99=*/ 0);
  OUTER_CAST1 (log1p, /*C99=*/ 1);
  OUTER_CAST1 (log2, /*C99=*/ 1);
  OUTER_CAST1 (logb, /*C99=*/ 1);
  OUTER_CAST1 (sin, /*C99=*/ 0);
  OUTER_CAST1 (sinh, /*C99=*/ 0);
  OUTER_CAST1 (sqrt, /*C99=*/ 0);
  OUTER_CAST1 (tan, /*C99=*/ 0);
  OUTER_CAST1 (tanh, /*C99=*/ 0);
  OUTER_CAST1 (tgamma, /*C99=*/ 1);

  INNER_CAST1 (ceil, /*C99=*/ 0);
  OUTER_CAST1 (ceil, /*C99=*/ 0);
  INNER_CAST1 (floor, /*C99=*/ 0);
  OUTER_CAST1 (floor, /*C99=*/ 0);
  INNER_CAST1 (lceil, /*C99=*/ 1);
  OUTER_CAST1 (lceil, /*C99=*/ 1);
  INNER_CAST1 (lfloor, /*C99=*/ 1);
  OUTER_CAST1 (lfloor, /*C99=*/ 1);
  INNER_CAST1 (lrint, /*C99=*/ 1);
  OUTER_CAST1 (lrint, /*C99=*/ 1);
  INNER_CAST1 (lround, /*C99=*/ 1);
  OUTER_CAST1 (lround, /*C99=*/ 1);
  INNER_CAST1 (llceil, /*C99=*/ 1);
  OUTER_CAST1 (llceil, /*C99=*/ 1);
  INNER_CAST1 (llfloor, /*C99=*/ 1);
  OUTER_CAST1 (llfloor, /*C99=*/ 1);
  INNER_CAST1 (llrint, /*C99=*/ 1);
  OUTER_CAST1 (llrint, /*C99=*/ 1);
  INNER_CAST1 (llround, /*C99=*/ 1);
  OUTER_CAST1 (llround, /*C99=*/ 1);
  INNER_CAST1 (nearbyint, /*C99=*/ 1);
  OUTER_CAST1 (nearbyint, /*C99=*/ 1);
  INNER_CAST1 (rint, /*C99=*/ 1);
  OUTER_CAST1 (rint, /*C99=*/ 1);
  INNER_CAST1 (round, /*C99=*/ 1);
  OUTER_CAST1 (round, /*C99=*/ 1);
  INNER_CAST1 (trunc, /*C99=*/ 1);
  OUTER_CAST1 (trunc, /*C99=*/ 1);
#endif /* __OPTIMIZE__ */
}

int main (void)
{
  return 0;
}
