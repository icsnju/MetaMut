/* C99 6.9.1(9) Function definitions; parameter has automatic storage.

   Test that actual parameters are passed by value and that modifications
   made within functions are lost on function return.  */

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

void foo32 (_Decimal32 z)
{
  z = z + 1.0df;
}

void foo64 (_Decimal64 z)
{
  z = z + 1.0dd;
}

void foo128 (_Decimal128 z)
{
  z = z + 1.0dl;
}

int
main ()
{
  _Decimal32 d32 = 1.1df;
  _Decimal64 d64 = 1.2dd;
  _Decimal128 d128 = 1.3dl;

  foo32 (d32);
  if (d32 != 1.1df)
    FAILURE

  foo64 (d64);
  if (d64 != 1.2dd)
    FAILURE

  foo128 (d128);
  if (d128 != 1.3dl)
    FAILURE

  FINISH
}
