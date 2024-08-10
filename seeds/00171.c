/* Check that conversion functions link correctly with -ffast-math.  */

/* { dg-do link } */
/* { dg-options "-ffast-math -lm" }  */
/* Bionic doesn't have rintl */
/* { dg-require-effective-target non_bionic } */

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

double floor (double);
float floorf (float);
long double floorl (long double);

double ceil (double);
float ceilf (float);
long double ceill (long double);

double round (double);
float roundf (float);
long double roundl (long double);

double rint (double);
float rintf (float);
long double rintl (long double);

int ifloor (double a) { return (int) floor (a); }
#ifdef HAVE_C99_RUNTIME
int ifloorf (float a) { return (int) floorf (a); }
int ifloorl (long double a) { return (int) floorl (a); }
#endif

long lfloor (double a) { return (long) floor (a); }
#ifdef HAVE_C99_RUNTIME
long lfloorf (float a) { return (long) floorf (a); }
long lfloorl (long double a) { return (long) floorl (a); }
#endif

long long llfloor (double a) { return (long long) floor (a); }
#ifdef HAVE_C99_RUNTIME
long long llfloorf (float a) { return (long long) floorf (a); }
long long llfloorl (long double a) { return (long long) floorl (a); }
#endif

int iceil (double a) { return (int) ceil (a); }
#ifdef HAVE_C99_RUNTIME
int iceilf (float a) { return (int) ceilf (a); }
int iceill (long double a) { return (int) ceill (a); }
#endif

long lceil (double a) { return (long) ceil (a); }
#ifdef HAVE_C99_RUNTIME
long lceilf (float a) { return (long) ceilf (a); }
long lceill (long double a) { return (long) ceill (a); }
#endif

long long llceil (double a) { return (long long) ceil (a); }
#ifdef HAVE_C99_RUNTIME
long long llceilf (float a) { return (long long) ceilf (a); }
long long llceill (long double a) { return (long long) ceill (a); }
#endif

#ifdef HAVE_C99_RUNTIME
int iround (double a) { return (int) round (a); }
int iroundf (float a) { return (int) roundf (a); }
int iroundl (long double a) { return (int) roundl (a); }
#endif

#ifdef HAVE_C99_RUNTIME
int irint (double a) { return (int) rint (a); }
int irintf (float a) { return (int) rintf (a); }
int irintl (long double a) { return (int) rintl (a); }
#endif

int main () { return 0; }
