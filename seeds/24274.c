/* { dg-do run { target { lp64 || ilp32 } } } */
/* { dg-options "-fsanitize=float-cast-overflow -Wno-overflow" } */
/* { dg-additional-options "-ffloat-store" { target { ia32 } } } */
/* { dg-additional-options "-mieee" { target { { alpha*-*-* } || { sh*-*-* } } } } */

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
  const double inf = __builtin_inf ();
  const double nan = __builtin_nan ("");
  volatile double d;

  volatile signed char sc;
  d = SCHAR_MIN;
  CHECK_BOUNDARY (sc, d);
  d = 0.0;
  CHECK_BOUNDARY (sc, d);
  d = SCHAR_MAX;
  CHECK_BOUNDARY (sc, d);
  CHECK_NONNUMBERS (sc);

  volatile unsigned char uc;
  d = UCHAR_MAX;
  CHECK_BOUNDARY (uc, d);
  d = 0.0;
  CHECK_BOUNDARY (uc, d);
  CHECK_NONNUMBERS (uc);

  volatile short int s;
  d = SHRT_MIN;
  CHECK_BOUNDARY (s, d);
  d = 0.0;
  CHECK_BOUNDARY (s, d);
  d = SHRT_MAX;
  CHECK_BOUNDARY (s, d);
  CHECK_NONNUMBERS (s);

  volatile unsigned short int us;
  d = USHRT_MAX;
  CHECK_BOUNDARY (us, d);
  d = 0.0;
  CHECK_BOUNDARY (us, d);
  CHECK_NONNUMBERS (us);

  volatile int i;
  d = INT_MIN;
  CHECK_BOUNDARY (i, d);
  d = 0.0;
  CHECK_BOUNDARY (i, d);
  d = INT_MAX;
  CHECK_BOUNDARY (i, d);
  CHECK_NONNUMBERS (i);

  volatile unsigned int u;
  d = UINT_MAX;
  CHECK_BOUNDARY (u, d);
  d = 0.0;
  CHECK_BOUNDARY (u, d);
  CHECK_NONNUMBERS (u);

  volatile long l;
  /* 64-bit vs 32-bit longs matter causes too much of a headache.  */
  d = 0.0;
  CHECK_BOUNDARY (l, d);
  CHECK_NONNUMBERS (l);

  volatile unsigned long ul;
  d = 0.0;
  CHECK_BOUNDARY (ul, d);
  CHECK_NONNUMBERS (ul);

  volatile long long ll;
  d = LLONG_MIN;
  CHECK_BOUNDARY (ll, d);
  d = 0.0;
  CHECK_BOUNDARY (ll, d);
  d = LLONG_MAX;
  CHECK_BOUNDARY (ll, d);
  CHECK_NONNUMBERS (ll);

  volatile unsigned long long ull;
  d = ULLONG_MAX;
  CHECK_BOUNDARY (ull, d);
  d = 0.0;
  CHECK_BOUNDARY (ull, d);
  CHECK_NONNUMBERS (ull);

  return 0;
}

/* { dg-output " -133 is outside the range of representable values of type 'signed char'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -129.5 is outside the range of representable values of type 'signed char'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -129 is outside the range of representable values of type 'signed char'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 128 is outside the range of representable values of type 'signed char'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 128.5 is outside the range of representable values of type 'signed char'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 132 is outside the range of representable values of type 'signed char'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* nan is outside the range of representable values of type 'signed char'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -?nan is outside the range of representable values of type 'signed char'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* inf is outside the range of representable values of type 'signed char'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -inf is outside the range of representable values of type 'signed char'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 256 is outside the range of representable values of type 'unsigned char'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 256.5 is outside the range of representable values of type 'unsigned char'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 260 is outside the range of representable values of type 'unsigned char'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -5 is outside the range of representable values of type 'unsigned char'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -1.5 is outside the range of representable values of type 'unsigned char'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -1 is outside the range of representable values of type 'unsigned char'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* nan is outside the range of representable values of type 'unsigned char'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -?nan is outside the range of representable values of type 'unsigned char'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* inf is outside the range of representable values of type 'unsigned char'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -inf is outside the range of representable values of type 'unsigned char'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -32773 is outside the range of representable values of type 'short int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -32769.5 is outside the range of representable values of type 'short int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -32769 is outside the range of representable values of type 'short int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 32768 is outside the range of representable values of type 'short int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 32768.5 is outside the range of representable values of type 'short int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 32772 is outside the range of representable values of type 'short int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* nan is outside the range of representable values of type 'short int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -?nan is outside the range of representable values of type 'short int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* inf is outside the range of representable values of type 'short int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -inf is outside the range of representable values of type 'short int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 65536 is outside the range of representable values of type 'short unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 65536.5 is outside the range of representable values of type 'short unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 65540 is outside the range of representable values of type 'short unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -5 is outside the range of representable values of type 'short unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -1.5 is outside the range of representable values of type 'short unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -1 is outside the range of representable values of type 'short unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* nan is outside the range of representable values of type 'short unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -?nan is outside the range of representable values of type 'short unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* inf is outside the range of representable values of type 'short unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -inf is outside the range of representable values of type 'short unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -2.14748e\\\+09 is outside the range of representable values of type 'int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -2.14748e\\\+09 is outside the range of representable values of type 'int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -2.14748e\\\+09 is outside the range of representable values of type 'int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 2.14748e\\\+09 is outside the range of representable values of type 'int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 2.14748e\\\+09 is outside the range of representable values of type 'int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 2.14748e\\\+09 is outside the range of representable values of type 'int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* nan is outside the range of representable values of type 'int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -?nan is outside the range of representable values of type 'int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* inf is outside the range of representable values of type 'int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -inf is outside the range of representable values of type 'int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 4.29497e\\\+09 is outside the range of representable values of type 'unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 4.29497e\\\+09 is outside the range of representable values of type 'unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 4.29497e\\\+09 is outside the range of representable values of type 'unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -5 is outside the range of representable values of type 'unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -1.5 is outside the range of representable values of type 'unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -1 is outside the range of representable values of type 'unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* nan is outside the range of representable values of type 'unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -?nan is outside the range of representable values of type 'unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* inf is outside the range of representable values of type 'unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -inf is outside the range of representable values of type 'unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* nan is outside the range of representable values of type 'long int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -?nan is outside the range of representable values of type 'long int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* inf is outside the range of representable values of type 'long int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -inf is outside the range of representable values of type 'long int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -5 is outside the range of representable values of type 'long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -1.5 is outside the range of representable values of type 'long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -1 is outside the range of representable values of type 'long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* nan is outside the range of representable values of type 'long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -?nan is outside the range of representable values of type 'long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* inf is outside the range of representable values of type 'long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -inf is outside the range of representable values of type 'long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 9.22337e\\\+18 is outside the range of representable values of type 'long long int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 9.22337e\\\+18 is outside the range of representable values of type 'long long int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 9.22337e\\\+18 is outside the range of representable values of type 'long long int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 9.22337e\\\+18 is outside the range of representable values of type 'long long int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 9.22337e\\\+18 is outside the range of representable values of type 'long long int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 9.22337e\\\+18 is outside the range of representable values of type 'long long int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 9.22337e\\\+18 is outside the range of representable values of type 'long long int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 9.22337e\\\+18 is outside the range of representable values of type 'long long int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 9.22337e\\\+18 is outside the range of representable values of type 'long long int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 9.22337e\\\+18 is outside the range of representable values of type 'long long int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 9.22337e\\\+18 is outside the range of representable values of type 'long long int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 9.22337e\\\+18 is outside the range of representable values of type 'long long int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 9.22337e\\\+18 is outside the range of representable values of type 'long long int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 9.22337e\\\+18 is outside the range of representable values of type 'long long int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 9.22337e\\\+18 is outside the range of representable values of type 'long long int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* nan is outside the range of representable values of type 'long long int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -?nan is outside the range of representable values of type 'long long int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* inf is outside the range of representable values of type 'long long int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -inf is outside the range of representable values of type 'long long int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 1.84467e\\\+19 is outside the range of representable values of type 'long long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 1.84467e\\\+19 is outside the range of representable values of type 'long long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 1.84467e\\\+19 is outside the range of representable values of type 'long long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 1.84467e\\\+19 is outside the range of representable values of type 'long long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 1.84467e\\\+19 is outside the range of representable values of type 'long long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 1.84467e\\\+19 is outside the range of representable values of type 'long long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 1.84467e\\\+19 is outside the range of representable values of type 'long long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 1.84467e\\\+19 is outside the range of representable values of type 'long long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 1.84467e\\\+19 is outside the range of representable values of type 'long long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 1.84467e\\\+19 is outside the range of representable values of type 'long long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 1.84467e\\\+19 is outside the range of representable values of type 'long long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 1.84467e\\\+19 is outside the range of representable values of type 'long long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 1.84467e\\\+19 is outside the range of representable values of type 'long long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 1.84467e\\\+19 is outside the range of representable values of type 'long long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* 1.84467e\\\+19 is outside the range of representable values of type 'long long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -5 is outside the range of representable values of type 'long long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -1.5 is outside the range of representable values of type 'long long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -1 is outside the range of representable values of type 'long long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* nan is outside the range of representable values of type 'long long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -?nan is outside the range of representable values of type 'long long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* inf is outside the range of representable values of type 'long long unsigned int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]* -inf is outside the range of representable values of type 'long long unsigned int'" } */
