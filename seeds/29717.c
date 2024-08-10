/* Test for bug where fold narrowed decimal floating-point
   operations.  */

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

volatile _Decimal32 f = 1.23456DF;
volatile _Decimal64 d = 1.23456DD;

int
main (void)
{
  if ((_Decimal128)((_Decimal64)f * (_Decimal64)f) != (_Decimal128)(d * d))
    FAILURE
  FINISH
}
