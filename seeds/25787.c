/* Test _Float64x <float.h> macros.  */
/* { dg-do run } */
/* { dg-options "" } */
/* { dg-add-options float64x } */
/* { dg-add-options ieee } */
/* { dg-require-effective-target float64x_runtime } */

#define WIDTH 64
#define EXT 1
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

#ifndef FLT64X_MANT_DIG
# error "FLT64X_MANT_DIG undefined"
#endif

#ifndef FLT64X_DECIMAL_DIG
# error "FLT64X_DECIMAL_DIG undefined"
#endif

#ifndef FLT64X_DIG
# error "FLT64X_DIG undefined"
#endif

#ifndef FLT64X_MIN_EXP
# error "FLT64X_MIN_EXP undefined"
#endif

#ifndef FLT64X_MIN_10_EXP
# error "FLT64X_MIN_10_EXP undefined"
#endif

#ifndef FLT64X_MAX_EXP
# error "FLT64X_MAX_EXP undefined"
#endif

#ifndef FLT64X_MAX_10_EXP
# error "FLT64X_MAX_10_EXP undefined"
#endif

#ifndef FLT64X_MAX
# error "FLT64X_MAX undefined"
#endif

#ifndef FLT64X_EPSILON
# error "FLT64X_EPSILON undefined"
#endif

#ifndef FLT64X_MIN
# error "FLT64X_MIN undefined"
#endif

#ifndef FLT64X_TRUE_MIN
# error "FLT64X_TRUE_MIN undefined"
#endif

#if FLT64X_MANT_DIG < 64 || FLT64X_MAX_EXP < 16384 || FLT64X_MIN_EXP + FLT64X_MAX_EXP != 3
# error "_Float64x bad format"
#endif
