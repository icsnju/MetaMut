/* { dg-do run } */
/* { dg-require-effective-target int128 } */
/* { dg-options "-fsanitize=float-cast-overflow -Wno-overflow" } */

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
  const double inf = __builtin_inf ();
  const double nan = __builtin_nan ("");
  volatile double d;

  __int128 i;
  d = INT128_MIN;
  CHECK_BOUNDARY (i, d);
  d = 0.0;
  CHECK_BOUNDARY (i, d);
  d = INT128_MAX;
  CHECK_BOUNDARY (i, d);
  CHECK_NONNUMBERS (i);

  unsigned __int128 u;
  d = UINT128_MAX;
  CHECK_BOUNDARY (u, d);
  d = 0.0;
  CHECK_BOUNDARY (u, d);
  CHECK_NONNUMBERS (u);

  return 0;
}

/* { dg-output "runtime error: 1.70141e\\\+38 is outside the range of representable values of type '__int128'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 1.70141e\\\+38 is outside the range of representable values of type '__int128'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 1.70141e\\\+38 is outside the range of representable values of type '__int128'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 1.70141e\\\+38 is outside the range of representable values of type '__int128'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 1.70141e\\\+38 is outside the range of representable values of type '__int128'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 1.70141e\\\+38 is outside the range of representable values of type '__int128'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 1.70141e\\\+38 is outside the range of representable values of type '__int128'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 1.70141e\\\+38 is outside the range of representable values of type '__int128'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 1.70141e\\\+38 is outside the range of representable values of type '__int128'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 1.70141e\\\+38 is outside the range of representable values of type '__int128'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 1.70141e\\\+38 is outside the range of representable values of type '__int128'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 1.70141e\\\+38 is outside the range of representable values of type '__int128'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 1.70141e\\\+38 is outside the range of representable values of type '__int128'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 1.70141e\\\+38 is outside the range of representable values of type '__int128'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 1.70141e\\\+38 is outside the range of representable values of type '__int128'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: nan is outside the range of representable values of type '__int128'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: -?nan is outside the range of representable values of type '__int128'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: inf is outside the range of representable values of type '__int128'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: -inf is outside the range of representable values of type '__int128'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 3.40282e\\\+38 is outside the range of representable values of type '__int128 unsigned'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 3.40282e\\\+38 is outside the range of representable values of type '__int128 unsigned'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 3.40282e\\\+38 is outside the range of representable values of type '__int128 unsigned'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 3.40282e\\\+38 is outside the range of representable values of type '__int128 unsigned'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 3.40282e\\\+38 is outside the range of representable values of type '__int128 unsigned'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 3.40282e\\\+38 is outside the range of representable values of type '__int128 unsigned'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 3.40282e\\\+38 is outside the range of representable values of type '__int128 unsigned'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 3.40282e\\\+38 is outside the range of representable values of type '__int128 unsigned'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 3.40282e\\\+38 is outside the range of representable values of type '__int128 unsigned'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 3.40282e\\\+38 is outside the range of representable values of type '__int128 unsigned'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 3.40282e\\\+38 is outside the range of representable values of type '__int128 unsigned'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 3.40282e\\\+38 is outside the range of representable values of type '__int128 unsigned'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 3.40282e\\\+38 is outside the range of representable values of type '__int128 unsigned'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 3.40282e\\\+38 is outside the range of representable values of type '__int128 unsigned'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: 3.40282e\\\+38 is outside the range of representable values of type '__int128 unsigned'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: -5 is outside the range of representable values of type '__int128 unsigned'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: -1.5 is outside the range of representable values of type '__int128 unsigned'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: -1 is outside the range of representable values of type '__int128 unsigned'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: nan is outside the range of representable values of type '__int128 unsigned'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: -?nan is outside the range of representable values of type '__int128 unsigned'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: inf is outside the range of representable values of type '__int128 unsigned'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*runtime error: -inf is outside the range of representable values of type '__int128 unsigned'" } */
