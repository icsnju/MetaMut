/* C99 6.5.4 Cast operators.
   Test valid casts involving decimal float.  */

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

static float f = 2.f;
static double d = 2.l;

int
main (void)
{
  /* Casts between DFP types.  */
  d32 = 1.2df;
  d64 = 1.2dd;
  d128 = 1.2dl;

  if (d32 != (_Decimal32) d64)
    FAILURE
  if (d32 != (_Decimal32) d128)
    FAILURE

  if (d64 != (_Decimal64) d32)
    FAILURE
  if (d64 != (_Decimal64) d128)
    FAILURE

  if (d128 != (_Decimal128) d32)
    FAILURE
  if (d128 != (_Decimal128) d64)
    FAILURE

  /* Casts between generic and decimal floating point types.  Use a
     value that we can assume can be represented exactly in all
     representations. */
  
  d32 = 2.0df;
  d64 = 2.0dd;
  d128 = 2.0dl;

  /* To generic floating types.  */
  if ((float) d32 != 2.0f)
    FAILURE
  if ((double) d32 != 2.0l)
    FAILURE
  if ((float) d64 != 2.0f)
    FAILURE
  if ((double) d64 != 2.0l)
    FAILURE
  if ((float) d128 != 2.0f)
    FAILURE
  if ((double) d128 != 2.0l)
    FAILURE

  /* float to decimal floating types.  */
  if (d32 != (_Decimal32) f)
    FAILURE
  if (d64 != (_Decimal64) f)
    FAILURE
  if (d128 != (_Decimal128) f)
    FAILURE

  /* double to decimal floating types.  */
  if (d32 != (_Decimal32) d)
    FAILURE
  if (d64 != (_Decimal64) d)
    FAILURE
  if (d128 != (_Decimal128) d)
    FAILURE

  FINISH
}
