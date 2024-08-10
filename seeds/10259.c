/* Copyright (C) 2003 Free Software Foundation.

   Check that built-in cabs, cabsf and cabsl functions don't
   break anything and produces the expected results.

   Written by Roger Sayle, 1st June 2003.  */

/* { dg-do link } */
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

extern void link_error(void);

extern float cabsf (float _Complex);
extern double cabs (double _Complex);
extern long double cabsl (long double _Complex);

int
main (void)
{
  /* For each type, test both runtime and compile time (constant folding)
     optimization.  */
  float _Complex fc = 3.0F + 4.0iF;
  double _Complex dc = 3.0 + 4.0i;
  long double _Complex ldc = 3.0L + 4.0iL;

#ifdef HAVE_C99_RUNTIME
  /* Test floats.  */
  if (cabsf (fc) != 5.0F)
    link_error ();
  if (__builtin_cabsf (fc) != 5.0F)
    link_error ();
  if (cabsf (3.0F + 4.0iF) != 5.0F)
    link_error ();
  if (__builtin_cabsf (3.0F + 4.0iF) != 5.0F)
    link_error ();
#endif

  /* Test doubles.  */
  if (cabs (dc) != 5.0)
    link_error ();
  if (__builtin_cabs (dc) != 5.0)
    link_error ();
  if (cabs (3.0 + 4.0i) != 5.0)
    link_error ();
  if (__builtin_cabs (3.0 + 4.0i) != 5.0)
    link_error ();

#ifdef HAVE_C99_RUNTIME
  /* Test long doubles.  */
  if (cabsl (ldc) != 5.0L)
    link_error ();
  if (__builtin_cabsl (ldc) != 5.0L)
    link_error ();
  if (cabsl (3.0L + 4.0iL) != 5.0L)
    link_error ();
  if (__builtin_cabsl (3.0L + 4.0iL) != 5.0L)
    link_error ();
#endif

  return 0;
}

