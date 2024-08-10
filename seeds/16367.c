/* Test _Float128 <float.h> macros.  */
/* { dg-do run } */
/* { dg-options "" } */
/* { dg-add-options float128 } */
/* { dg-add-options ieee } */
/* { dg-require-effective-target float128_runtime } */

#define WIDTH 128
#define EXT 0
/* Tests for _FloatN / _FloatNx types: compile and execution tests for
   <float.h>.  Before including this file, define WIDTH as the value
   N; define EXT to 1 for _FloatNx and 0 for _FloatN.  */

#define __STDC_WANT_IEC_60559_TYPES_EXT__
#include <float.h>

#define CONCATX(X, Y) X ## Y
#define CONCAT(X, Y) CONCATX (X, Y)
#define CONCAT3(X, Y, Z) CONCAT (CONCAT (X, Y), Z)
#define CONCAT4(W, X, Y, Z) CONCAT (CONCAT (CONCAT (W, X), Y), Z)

#if EXT
# define TYPE CONCAT3 (_Float, WIDTH, x)
# define CST(C) CONCAT4 (C, f, WIDTH, x)
# define FMAC(M) CONCAT4 (FLT, WIDTH, X_, M)
#else
# define TYPE CONCAT (_Float, WIDTH)
# define CST(C) CONCAT3 (C, f, WIDTH)
# define FMAC(M) CONCAT4 (FLT, WIDTH, _, M)
#endif

extern void exit (int);
extern void abort (void);

int
main (void)
{
  volatile TYPE a = CST (1.0);
  for (int i = 0; i >= FMAC (MIN_EXP); i--)
    a *= CST (0.5);
  if (a != FMAC (MIN))
    abort ();
  for (int i = 0; i < FMAC (MANT_DIG) - 1; i++)
    a *= CST (0.5);
  if (a != FMAC (TRUE_MIN))
    abort ();
  a *= CST (0.5);
  if (a != CST (0.0))
    abort ();
  a = FMAC (EPSILON);
  for (int i = 0; i < FMAC (MANT_DIG) - 1; i++)
    a *= CST (2.0);
  if (a != CST (1.0))
    abort ();
  a = FMAC (MAX);
  for (int i = 0; i < FMAC (MAX_EXP); i++)
    a *= CST (0.5);
  if (a != CST (1.0) - FMAC (EPSILON) * CST (0.5))
    abort ();
  exit (0);
}

#ifndef FLT128_MANT_DIG
# error "FLT128_MANT_DIG undefined"
#endif

#ifndef FLT128_DECIMAL_DIG
# error "FLT128_DECIMAL_DIG undefined"
#endif

#ifndef FLT128_DIG
# error "FLT128_DIG undefined"
#endif

#ifndef FLT128_MIN_EXP
# error "FLT128_MIN_EXP undefined"
#endif

#ifndef FLT128_MIN_10_EXP
# error "FLT128_MIN_10_EXP undefined"
#endif

#ifndef FLT128_MAX_EXP
# error "FLT128_MAX_EXP undefined"
#endif

#ifndef FLT128_MAX_10_EXP
# error "FLT128_MAX_10_EXP undefined"
#endif

#ifndef FLT128_MAX
# error "FLT128_MAX undefined"
#endif

#ifndef FLT128_EPSILON
# error "FLT128_EPSILON undefined"
#endif

#ifndef FLT128_MIN
# error "FLT128_MIN undefined"
#endif

#ifndef FLT128_TRUE_MIN
# error "FLT128_TRUE_MIN undefined"
#endif

#if FLT128_MANT_DIG != 113 || FLT128_MAX_EXP != 16384 || FLT128_MIN_EXP != -16381
# error "_Float128 bad format"
#endif
