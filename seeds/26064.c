/* C99 6.5.2.2 Function calls.  */

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

struct S1
{
  struct
  {
    _Decimal64 e;
  } b[0];
};

/* Test handling vararg parameters whose size is 0.  */

int check_var(int z,...)
{
  double d;
  struct S1 s1;
  long long result;
  va_list ap;
  va_start (ap, z);
  d = va_arg (ap, double);
  s1 = va_arg (ap, struct S1);
  result = va_arg (ap, long long);
  va_end (ap);
  return (result == 2LL);

}

int
main ()
{
  struct S1 s1;
  struct S1 a1[5];

  if (check_var(5, 1.0, s1, 2LL, a1[2], a1[2]) == 0)
    FAILURE

  FINISH
}
