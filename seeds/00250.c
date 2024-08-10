/* { dg-options "-O0" } */

/* C99 6.5.17: Comma operator.
   Test with decimal float operands.  */

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

volatile _Decimal32 d32a, d32b, d32c;
volatile _Decimal64 d64a, d64b, d64c;
volatile _Decimal128 d128a, d128b, d128c;

void
init ()
{
  d32b = 123.456e94df;
  d64b = 12.3456789012345e383dd;
  d128b = 12345.6789012345678901e4000dl;

  d32c = 1.3df;
  d64c = 1.2dd;
  d128c = 1.1dl;
}

int
main ()
{
  init ();
  
  d32a = (d32b, d32c);
  if (d32a != d32c)
    FAILURE
  d64a = (d64b, 7.89dd, d64c);
  if (d64a != d64c)
    FAILURE
  d128a = (45678.987654dl, d128c, d128b);
  if (d128a != d128b)
    FAILURE
  d128a = (d32b, d64b, d128b);
  if (d128a != d128b)
    FAILURE
  d32a = (d32b, 12, d64c);
  if (d32a != d64c)
    FAILURE;
  d64a = (d64b, d32b, 12);
  if (d64a != 12.0dd)
    FAILURE;

  FINISH
}
