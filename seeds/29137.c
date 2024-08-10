/* { dg-options "-O0" } */

/* Test decimal fp conversions of zero.  */

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

volatile _Decimal32 d32a, d32c;
volatile _Decimal64 d64a, d64c;
volatile _Decimal128 d128a, d128c;

int
main ()
{
  d32a = d32c;
  if (d32a)
    FAILURE
  d32a = d64c;
  if (d32a)
    FAILURE
  d32a = d128c;
  if (d32a)
    FAILURE

  d64a = d32c;
  if (d64a)
    FAILURE
  d64a = d64c;
  if (d64a)
    FAILURE
  d64a = d128c;
  if (d64a)
    FAILURE
  
  d128a = d32c;
  if (d128a)
    FAILURE
  d128a = d64c;
  if (d128a)
    FAILURE
  d128a = d128c;
  if (d128a)
    FAILURE
  
  FINISH
}
