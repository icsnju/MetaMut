/* Test for bug where fold wrongly removed conversions to double and
   replaced them by conversions to float.  */

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

volatile float f = __builtin_inff ();
volatile _Decimal32 d32 = 1e40DF;

int
main (void)
{
  if ((double) f == (double) d32)
    FAILURE

  FINISH
}
