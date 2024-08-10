/* { dg-do run { target i?86-*-* x86_64-*-* ia64-*-* } } */
/* { dg-options "-fsanitize=float-cast-overflow" } */

#include <limits.h>
/* Various macros for -fsanitize=float-cast-overflow testing.  */

/* E.g. on CentOS 5 these aren't defined in limits.h.  */
#ifndef LLONG_MAX
# define LLONG_MAX __LONG_LONG_MAX__
#endif
#ifndef LLONG_MIN
# define LLONG_MIN (-__LONG_LONG_MAX__ - 1LL)
#endif
#ifndef ULLONG_MAX
# define ULLONG_MAX (__LONG_LONG_MAX__ * 2ULL + 1ULL)
#endif

#define INT128_MAX (__int128) (((unsigned __int128) 1 << ((__SIZEOF_INT128__ * __CHAR_BIT__) - 1)) - 1)
#define INT128_MIN (-INT128_MAX - 1)
#define UINT128_MAX ((2 * (unsigned __int128) INT128_MAX) + 1)

#define CHECK_BOUNDARY(VAR, VAL)	\
  (VAR) = (VAL) - 5.0;			\
  (VAR) = (VAL) - 1.5;			\
  (VAR) = (VAL) - 1.0;			\
  (VAR) = (VAL) - 0.75;			\
  (VAR) = (VAL) - 0.5;			\
  (VAR) = (VAL) - 0.0000001;		\
  (VAR) = (VAL) - 0.0;			\
  (VAR) = (VAL);			\
  (VAR) = (VAL) + 0.0;			\
  (VAR) = (VAL) + 0.0000001;		\
  (VAR) = (VAL) + 0.5;			\
  (VAR) = (VAL) + 0.75;			\
  (VAR) = (VAL) + 1.0;			\
  (VAR) = (VAL) + 1.5;			\
  (VAR) = (VAL) + 5.0;

#define CHECK_NONNUMBERS(VAR)		\
  (VAR) = nan;				\
  (VAR) = -nan;				\
  (VAR) = inf;				\
  (VAR) = -inf;

int
main (void)
{
  volatile __float128 f;

  volatile signed char s;
  f = SCHAR_MIN;
  CHECK_BOUNDARY (s, f);
  f = 0.0q;
  CHECK_BOUNDARY (s, f);
  f = SCHAR_MAX;
  CHECK_BOUNDARY (s, f);

  volatile unsigned char u;
  f = UCHAR_MAX;
  CHECK_BOUNDARY (u, f);
  f = 0.0q;
  CHECK_BOUNDARY (u, f);

  return 0;
}

/* { dg-output " \[^\n\r]* is outside the range of representable values of type\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* \[^\n\r]* is outside the range of representable values of type\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* \[^\n\r]* is outside the range of representable values of type\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* \[^\n\r]* is outside the range of representable values of type\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* \[^\n\r]* is outside the range of representable values of type\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* \[^\n\r]* is outside the range of representable values of type\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* \[^\n\r]* is outside the range of representable values of type\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* \[^\n\r]* is outside the range of representable values of type\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* \[^\n\r]* is outside the range of representable values of type\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* \[^\n\r]* is outside the range of representable values of type\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* \[^\n\r]* is outside the range of representable values of type\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* \[^\n\r]* is outside the range of representable values of type" } */
