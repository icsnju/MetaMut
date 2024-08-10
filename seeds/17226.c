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

void link_error (void);

extern long lround(double);
extern long lrint(double);

extern long long llround(double);
extern long long llrint(double);

extern long lroundf(float);
extern long lrintf(float);

extern long long llroundf(float);
extern long long llrintf(float);

extern long lroundl(long double);
extern long lrintl(long double);

extern long long llroundl(long double);
extern long long llrintl(long double);


void test(double x)
{
#ifdef HAVE_C99_RUNTIME
  if (sizeof(long) != sizeof(long long))
    return;

  if (__builtin_lceil(x) != __builtin_llceil(x))
    link_error();
  if (__builtin_lfloor(x) != __builtin_llfloor(x))
    link_error();
  if (lround(x) != llround(x))
    link_error();
  if (lrint(x) != llrint(x))
    link_error();
#endif
}

void testf(float x)
{
#ifdef HAVE_C99_RUNTIME
  if (sizeof(long) != sizeof(long long))
    return;

  if (__builtin_lceilf(x) != __builtin_llceilf(x))
    link_error();
  if (__builtin_lfloorf(x) != __builtin_llfloorf(x))
    link_error();
  if (lroundf(x) != llroundf(x))
    link_error();
  if (lrintf(x) != llrintf(x))
    link_error();
#endif
}

void testl(long double x)
{
#ifdef HAVE_C99_RUNTIME
  if (sizeof(long) != sizeof(long long))
    return;

  if (__builtin_lceill(x) != __builtin_llceill(x))
    link_error();
  if (__builtin_lfloorl(x) != __builtin_llfloorl(x))
    link_error();
  if (lroundl(x) != llroundl(x))
    link_error();
  if (lrintl(x) != llrintl(x))
    link_error();
#endif
}

int main()
{
  test(0.0);
  testf(0.0);
  testl(0.0);
  return 0;
}

