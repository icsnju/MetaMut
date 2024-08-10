/* Test _Float16 <float.h> macros.  */
/* { dg-do run } */
/* { dg-options "" } */
/* { dg-add-options float16 } */
/* { dg-add-options ieee } */
/* { dg-require-effective-target float16_runtime } */

#define WIDTH 16
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

#ifndef FLT16_MANT_DIG
# error "FLT16_MANT_DIG undefined"
#endif

#ifndef FLT16_DECIMAL_DIG
# error "FLT16_DECIMAL_DIG undefined"
#endif

#ifndef FLT16_DIG
# error "FLT16_DIG undefined"
#endif

#ifndef FLT16_MIN_EXP
# error "FLT16_MIN_EXP undefined"
#endif

#ifndef FLT16_MIN_10_EXP
# error "FLT16_MIN_10_EXP undefined"
#endif

#ifndef FLT16_MAX_EXP
# error "FLT16_MAX_EXP undefined"
#endif

#ifndef FLT16_MAX_10_EXP
# error "FLT16_MAX_10_EXP undefined"
#endif

#ifndef FLT16_MAX
# error "FLT16_MAX undefined"
#endif

#ifndef FLT16_EPSILON
# error "FLT16_EPSILON undefined"
#endif

#ifndef FLT16_MIN
# error "FLT16_MIN undefined"
#endif

#ifndef FLT16_TRUE_MIN
# error "FLT16_TRUE_MIN undefined"
#endif

#if FLT16_MANT_DIG != 11 || FLT16_MAX_EXP != 16 || FLT16_MIN_EXP != -13
# error "_Float16 bad format"
#endif
