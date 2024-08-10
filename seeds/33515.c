/* { dg-do run { target { lp64 || ilp32 } } } */
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
  volatile float f;

  volatile signed char s;
  f = SCHAR_MIN;
  CHECK_BOUNDARY (s, f);
  f = 0.0;
  CHECK_BOUNDARY (s, f);
  f = SCHAR_MAX;
  CHECK_BOUNDARY (s, f);

  volatile unsigned char u;
  f = UCHAR_MAX;
  CHECK_BOUNDARY (u, f);
  f = 0.0;
  CHECK_BOUNDARY (u, f);

  return 0;
}

/* { dg-output " -133* is outside the range of representable values of type\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -129.5 is outside the range of representable values of type\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -129 is outside the range of representable values of type\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 128 is outside the range of representable values of type\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 128.5 is outside the range of representable values of type\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 132 is outside the range of representable values of type\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 256 is outside the range of representable values of type\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 256.5 is outside the range of representable values of type\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 260 is outside the range of representable values of type\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -5 is outside the range of representable values of type\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -1.5 is outside the range of representable values of type\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -1 is outside the range of representable values of type" } */
