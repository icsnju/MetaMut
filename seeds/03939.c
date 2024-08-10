/* { dg-do compile } */

/* Test various conversions involving decimal floating types. */

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

/* Assertion that constant C is of type T.  */
#define ASSERT_CONST_TYPE(C, T)                 \
        do {                                    \
          typedef T type;                       \
          typedef type **typepp;                \
          typedef __typeof__((C)) ctype;        \
          typedef ctype **ctypepp;              \
          typepp x = 0;                         \
          ctypepp y = 0;                        \
          x = y;                                \
          y = x;                                \
        } while (0)

int
main ()
{
  ASSERT_CONST_TYPE (3 + 2.1df, _Decimal32); /* { dg-bogus "assignment from incompatible pointer type" } */
  ASSERT_CONST_TYPE (1.3df + 2, _Decimal32); /* { dg-bogus "assignment from incompatible pointer type" } */
  ASSERT_CONST_TYPE (56U - 55.0dd, _Decimal64); /* { dg-bogus "assignment from incompatible pointer type" } */
  ASSERT_CONST_TYPE (5 * .2DL, _Decimal128); /* { dg-bogus "assignment from incompatible pointer type" } */
  ASSERT_CONST_TYPE (.88dl / 2L, _Decimal128); /* { dg-bogus "assignment from incompatible pointer type" } */
  ASSERT_CONST_TYPE (.114df - 1.6dd, _Decimal64); /* { dg-bogus "assignment from incompatible pointer type" } */
  ASSERT_CONST_TYPE (3L - 1 + .55df, _Decimal32); /* { dg-bogus "assignment from incompatible pointer type" } */

  return 0;
}
