/* { dg-options "-O2" } */

/* C99 6.5.5: Multiplicative operators.
   C99 6.5.6: Additive operators.  */

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

extern void link_error (void);

int
main ()
{
  _Decimal32 d32_1, d32_2;

  /* Compare like-typed positive constants. */
  if (2.99df + 5.1df != 8.09df)
    link_error ();

  if (5.77df - 2.22dd != 3.55df)
    link_error ();

  if (2.0dl * 3.7dd * -2 != -14.8df)
    link_error ();

  if (.18df / -.2df + 1 != 1.e-1dd)
    link_error ();

  d32_1 = 3.0df;
  d32_2 = 1.0df;

  if (!__builtin_constant_p (d32_1 + 0.2df))
    link_error ();

  if (!__builtin_constant_p (1.0df / 3.0df))
    link_error ();

  if (!__builtin_constant_p (d32_2 / d32_1))
    link_error ();

  d32_2 = 2.0df;
  if (!__builtin_constant_p (d32_2 / d32_1))
    link_error ();

  return 0;
}
