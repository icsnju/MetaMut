/* C99 6.5.9 Equality operators.
   Compare decimal float constants against each other. */
/* { dg-additional-options "-fexcess-precision=fast" } */

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
  /* Compare like-typed positive constants. */
  if (2.0df != 2.0df)
    link_error ();

  /* Compare decimal float constants of different types. */
  if (500e-2dl != 0.05e2df)
    link_error ();

  /* Binary floating point introduces errors to decimal values. */
  if (1.4 + 1.4 + 1.4 == 4.2)
    link_error ();

  /* But, this looks more like what one would expect. */
  if (1.4dd + 1.4dd + 1.4dd != 4.2dd)
    link_error ();

  FINISH
}
