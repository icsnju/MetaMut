/* Copyright (C) 2004  Free Software Foundation.

   Verify that builtin math functions (with fixed point return types)
   are converted to smaller FP types correctly by the compiler.

   Written by Kaveh Ghazi, 2004-05-01.  */

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

#define PROTOTYPE1_RET(FN, RET) \
  extern RET FN(double); \
  extern RET FN##f(float); \
  extern RET FN##l(long double);

/* Test converting math builtins to narrower FP types based on if the
   argument is a narrower type (perhaps implicitly) cast to a wider
   one.  */
#define INNER_CAST1(MATHFN, RET) \
 PROTOTYPE1_RET (MATHFN, RET); \
 extern void link_failure_inner_##MATHFN##l_##MATHFN(void); \
 extern void link_failure_inner_##MATHFN##l_##MATHFN##f(void); \
 extern void link_failure_inner_##MATHFN##_##MATHFN##f(void); \
 if (sizeof (long double) > sizeof (double) \
     && MATHFN##l(d1) != MATHFN(d1)) \
    link_failure_inner_##MATHFN##l_##MATHFN(); \
 if (sizeof (long double) > sizeof (float) \
     && MATHFN##l(f1) != MATHFN##f(f1)) \
    link_failure_inner_##MATHFN##l_##MATHFN##f(); \
 if (sizeof (long double) > sizeof (float) \
     && MATHFN##l((double)f1) != MATHFN##f(f1)) \
    link_failure_inner_##MATHFN##l_##MATHFN##f(); \
 if (sizeof (double) > sizeof (float) \
     && MATHFN(f1) != MATHFN##f(f1)) \
    link_failure_inner_##MATHFN##_##MATHFN##f()

void __attribute__ ((__noinline__)) test (double d1, float f1)
{
#ifdef __OPTIMIZE__
#ifdef HAVE_C99_RUNTIME
  /* We're converting to implicitly generated C99 functions.  */
  INNER_CAST1 (__builtin_lceil, long);
  INNER_CAST1 (__builtin_llceil, long long);
  INNER_CAST1 (__builtin_lfloor, long);
  INNER_CAST1 (__builtin_llfloor, long long);
  INNER_CAST1 (lround, long);
  INNER_CAST1 (llround, long long);
  INNER_CAST1 (lrint, long);
  INNER_CAST1 (llrint, long long);
#endif /* HAVE_C99_RUNTIME */
#endif /* __OPTIMIZE__ */
}

int main (void)
{
  test (1, 2);
  return 0;
}
