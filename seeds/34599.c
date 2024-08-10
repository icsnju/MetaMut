/* { dg-do run } */

#include <stdarg.h>
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

void
f (int a, ...)
{
  va_list ap;
  if (a != 0)
    FAILURE
  va_start (ap, a);
  if (va_arg (ap, _Decimal128) != 1.2DL)
    FAILURE
  if (va_arg (ap, _Decimal128) != 2.34DL)
    FAILURE
  if (va_arg (ap, _Decimal128) != 3.456DL)
    FAILURE
  if (va_arg (ap, _Decimal128) != 4.567DL)
    FAILURE
  if (va_arg (ap, double) != 5.125)
    FAILURE
  va_end (ap);
}

int
main (void)
{
  f (0, 1.2DL, 2.34DL, 3.456DL, 4.567DL, 5.125);

  FINISH
}
