/* { dg-options "-O0" } */

/* C99 6.5.8 Relational operators.
   Compare decimal float values against variables of different types.  */

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

_Decimal32 d32;
_Decimal64 d64;
_Decimal128 d128;

/* Use some typedefs of decimal float types, too.  */
typedef _Decimal32 SDtype;
typedef _Decimal64 DDtype;
typedef _Decimal128 TDtype;

SDtype d32b;
DDtype d64b;
TDtype d128b;

void
inits (void)
{
  d32 = 1.0df;
  d64 = 3.0dd;
  d128 = 5.0dl;
  d32b = -1.0df;
  d64b = -4.0dd;
  d128b = -6.0dl;
}

void
compare_dfp (void)
{
  if ((d32 > d64) != 0) FAILURE
  if ((d32 >= d128b) != 1) FAILURE

  if ((d64 < d32) != 0) FAILURE
  if ((d64 <= d128) != 1) FAILURE

  if ((d128 > d32) != 1) FAILURE
  if ((d128 >= d64) != 1) FAILURE
}

int
main ()
{
  inits ();

  compare_dfp ();

  FINISH
}
