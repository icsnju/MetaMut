/* N1150 5.1 Conversion between decimal floating integer.
   C99 6.3.1.4(1a) New.
   Test integer saturation.  */

#ifndef	__STDC_WANT_DEC_FP__
#define __STDC_WANT_DEC_FP__ 1
#endif

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
#include <float.h>
#include <limits.h>

volatile _Decimal32 d32;
volatile _Decimal64 d64;
volatile _Decimal128 d128;

volatile signed int si;
volatile unsigned int usi;
volatile unsigned long long udi;

int
main ()
{

  /* Unsigned.  */
  usi = DEC32_MAX;  /* { dg-warning "overflow in conversion" } */
  if (usi != UINT_MAX)
    FAILURE

  usi = DEC64_MAX;  /* { dg-warning "overflow in conversion" } */
  if (usi != UINT_MAX)
    FAILURE

  usi = DEC128_MAX; /* { dg-warning "overflow in conversion" } */
  if (usi != UINT_MAX)
    FAILURE

  /* Signed.  */
  si = DEC32_MAX;	/* { dg-warning "overflow in conversion" } */
  if (si != INT_MAX)
    FAILURE

  si = DEC64_MAX;   /* { dg-warning "overflow in conversion" } */
  if (si != INT_MAX)
    FAILURE

  si = DEC128_MAX;  /* { dg-warning "overflow in conversion" } */
  if (si != INT_MAX)
    FAILURE

  si = - DEC32_MAX; /* { dg-warning "overflow in conversion" } */
  if (si != INT_MIN)
    FAILURE

  si = - DEC64_MAX; /* { dg-warning "overflow in conversion" } */
  if (si != INT_MIN)
    FAILURE

  si = - DEC128_MAX; /* { dg-warning "overflow in conversion" } */
  if (si != INT_MIN)
    FAILURE

  FINISH
}
