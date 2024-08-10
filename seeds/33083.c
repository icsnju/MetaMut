/* { dg-options "-O0" } */

/* C99 6.5.9 Equality operators.
   Compare decimal float values against each other at runtime.  */

#define WIDTH 64
/* Basic test of runtime equality comparisons using simple values that
   are not affected by rounding.  */

#include <stdlib.h>
#ifdef __cplusplus
typedef float _Decimal32 __attribute__((mode(SD)));
typedef float _Decimal64 __attribute__((mode(DD)));
typedef float _Decimal128 __attribute__((mode(TD)));

#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

int failures;

#ifdef DBG
extern int printf (const char *, ...);
#define FAILURE { printf ("failed at line %d\n", __LINE__); failures++; }
#define FINISH if (failures != 0) __builtin_abort (); return 0;
#else
#define FAILURE __builtin_abort ();
#define FINISH return 0;
#endif

#define PASTE2(A,B) A ## B
#define PASTE(A,B) PASTE2(A,B)

#undef FAILURE
#ifdef DBG
#define FAILURE(OP,KIND) \
  { printf ("failed at line %d: %s for %s values\n", __LINE__, OP, KIND); \
    failures++; }
#else
#define FAILURE(OP,KIND) __builtin_abort ();
#endif

#ifndef WIDTH
#error define WIDTH as decimal float size in bytes
#endif

#if WIDTH == 32
#define DTYPE _Decimal32
#define SUFFIX DF
#elif WIDTH == 64
#define DTYPE _Decimal64
#define SUFFIX DD
#elif WIDTH == 128
#define DTYPE _Decimal128
#define SUFFIX DL
#elif WIDTH == 0
/* This is for testing the test using a type known to work.  */
#define DTYPE double
#define SUFFIX
#else
#error invalid width for decimal float type
#endif

DTYPE m_two = PASTE(-2.0, SUFFIX);
DTYPE m_one = PASTE(-1.0, SUFFIX);
DTYPE zero  = PASTE(0.0, SUFFIX);
DTYPE one   = PASTE(1.0, SUFFIX);
DTYPE two   = PASTE(2.0, SUFFIX);

void
test_compares (void)
{
  DTYPE x = one;
  DTYPE y = zero;
  DTYPE z = m_one;

  /* Equal to: comparisons against equal values.  */

  if (! (x == one))   FAILURE ("==", "equal")
  if (! (y == zero))  FAILURE ("==", "equal")
  if (! (z == m_one)) FAILURE ("==", "equal")

  /* Equal to: comparisons against lesser values.  */

  if (x == m_one)     FAILURE ("==", "lesser")
  if (x == zero)      FAILURE ("==", "lesser")
  if (y == m_one)     FAILURE ("==", "lesser")
  if (z == m_two)     FAILURE ("==", "lesser")

  /* Equal to: comparisons against greater values.  */

  if (x == two)       FAILURE ("==", "greater")
  if (y == one)       FAILURE ("==", "greater")
  if (z == zero)      FAILURE ("==", "greater")
  if (z == one)       FAILURE ("==", "greater")

  /* Not equal to: comparisons against equal values.  */

  if (x != one)        FAILURE ("!=", "equal")
  if (y != zero)       FAILURE ("!=", "equal")
  if (z != m_one)      FAILURE ("!=", "equal")

  /* Not equal to: comparisons against lesser values.  */

  if (! (x != m_one))  FAILURE ("!=", "lesser")
  if (! (x != zero))   FAILURE ("!=", "lesser")
  if (! (y != m_one))  FAILURE ("!=", "lesser")
  if (! (z != m_two))  FAILURE ("!=", "lesser")

  /* Not equal to: comparisons against greater values.  */

  if (! (x != m_one)) FAILURE ("!=", "greater")
  if (! (x != zero))  FAILURE ("!=", "greater")
  if (! (y != m_one)) FAILURE ("!=", "greater")
  if (! (z != m_two)) FAILURE ("!=", "greater")
}

int main ()
{
  test_compares ();

  FINISH
}
