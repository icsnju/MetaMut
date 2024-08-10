/* Test __builtin_{add,sub,mul}_overflow.  */
/* { dg-do run } */
/* { dg-skip-if "" { ! run_expensive_tests }  { "*" } { "-O0" "-O2" } } */

#define SCHAR_MAX __SCHAR_MAX__
#define SCHAR_MIN (-__SCHAR_MAX__ - 1)
#define SHRT_MAX __SHRT_MAX__
#define SHRT_MIN (-__SHRT_MAX__ - 1)
#define INT_MAX __INT_MAX__
#define INT_MIN (-__INT_MAX__ - 1)
#define LONG_MAX __LONG_MAX__
#define LONG_MIN (-__LONG_MAX__ - 1)
#define LLONG_MAX __LONG_LONG_MAX__
#define LLONG_MIN (-__LONG_LONG_MAX__ - 1)

int v;

__attribute__((noinline, noclone)) void
bar (void)
{
  v++;
}

#define T(n, t1, t2, tr, v1, v2, vr, b, o) \
__attribute__((noinline, noclone)) tr		\
t##n##_1##b (t1 x, t2 y)			\
{						\
  tr r;						\
  if (__builtin_##b##_overflow (x, y, &r))	\
    bar ();					\
  return r;					\
}						\
						\
__attribute__((noinline, noclone)) tr		\
t##n##_2##b (t2 y)				\
{						\
  t1 x = (v1);					\
  tr r;						\
  if (__builtin_##b##_overflow (x, y, &r))	\
    bar ();					\
  return r;					\
}						\
						\
__attribute__((noinline, noclone)) tr		\
t##n##_3##b (t2 y)				\
{						\
  tr r;						\
  if (__builtin_##b##_overflow ((t1) (v1), y,	\
				&r))		\
    bar ();					\
  return r;					\
}						\
						\
__attribute__((noinline, noclone)) tr		\
t##n##_4##b (t1 x)				\
{						\
  t2 y = (v2);					\
  tr r;						\
  if (__builtin_##b##_overflow (x, y, &r))	\
    bar ();					\
  return r;					\
}						\
						\
__attribute__((noinline, noclone)) tr		\
t##n##_5##b (t1 x)				\
{						\
  tr r;						\
  if (__builtin_##b##_overflow (x, (t2) (v2),	\
				&r))		\
    bar ();					\
  return r;					\
}						\
						\
__attribute__((noinline, noclone)) void		\
t##n##b (void)					\
{						\
  t1 x = (v1);					\
  t2 y = (v2);					\
  tr r1, r2;					\
  v = 0;					\
  if (t##n##_1##b (x, y) != (tr) (vr)		\
      || t##n##_2##b (y) != (tr) (vr)		\
      || t##n##_3##b (y) != (tr) (vr)		\
      || t##n##_4##b (x) != (tr) (vr)		\
      || t##n##_5##b (x) != (tr) (vr))		\
    __builtin_abort ();				\
  if (__builtin_##b##_overflow (x, y, &r1))	\
    bar ();					\
  if (r1 != (tr) (vr))				\
    __builtin_abort ();				\
  if (__builtin_##b##_overflow ((t1) (v1),	\
				(t2) (v2), &r2))\
    bar ();					\
  if (r2 != (tr) (vr) || v != 7 * o)		\
    __builtin_abort ();				\
}
#define TP(n, t1, t2, er, v1, v2, b, o) \
__attribute__((noinline, noclone)) void		\
t##n##_1##b (t1 x, t2 y)			\
{						\
  if (__builtin_##b##_overflow_p (x, y, er))	\
    bar ();					\
}						\
						\
__attribute__((noinline, noclone)) void		\
t##n##_2##b (t2 y)				\
{						\
  t1 x = (v1);					\
  if (__builtin_##b##_overflow_p (x, y, er))	\
    bar ();					\
}						\
						\
__attribute__((noinline, noclone)) void		\
t##n##_3##b (t2 y)				\
{						\
  if (__builtin_##b##_overflow_p ((t1) (v1), y,	\
				  er))		\
    bar ();					\
}						\
						\
__attribute__((noinline, noclone)) void		\
t##n##_4##b (t1 x)				\
{						\
  t2 y = (v2);					\
  if (__builtin_##b##_overflow_p (x, y, er))	\
    bar ();					\
}						\
						\
__attribute__((noinline, noclone)) void		\
t##n##_5##b (t1 x)				\
{						\
  if (__builtin_##b##_overflow_p (x, (t2) (v2),	\
				  er))		\
    bar ();					\
}						\
						\
__attribute__((noinline, noclone)) void		\
t##n##b (void)					\
{						\
  t1 x = (v1);					\
  t2 y = (v2);					\
  v = 0;					\
  t##n##_1##b (x, y);				\
  t##n##_2##b (y);				\
  t##n##_3##b (y);				\
  t##n##_4##b (x);				\
  t##n##_5##b (x);				\
  if (__builtin_##b##_overflow_p (x, y, er))	\
    bar ();					\
  if (__builtin_##b##_overflow_p ((t1) (v1),	\
				  (t2) (v2),	\
				  er))		\
    bar ();					\
  if (v != 7 * o)				\
    __builtin_abort ();				\
}
#ifdef OVFP
#undef T
#define T(n, t1, t2, tr, v1, v2, vr, b, o) \
TP(n, t1, t2, (tr) 0, v1, v2, b, o)
#endif
#define ST(n, t, v1, v2, vr, b, o) \
T (n, t, t, t, v1, v2, vr, b, o)

#ifdef __SIZEOF_INT128__
#define WTYPE __int128
#else
#define WTYPE long long int
#endif

#define TESTS \
T (100, signed char, signed char, unsigned WTYPE, -1, 0, -1, add, 1) \
T (101, unsigned char, unsigned char, unsigned WTYPE, 5, 5, 10, add, 0) \
T (102, signed char, unsigned short, unsigned WTYPE, 5, 5, 0, sub, 0) \
T (103, signed char, unsigned short, unsigned WTYPE, 5, 6, -1, sub, 1) \
T (104, signed char, signed char, unsigned WTYPE, -1, -1, 1, mul, 0) \
T (105, unsigned char, signed char, unsigned WTYPE, 17, -2, -34, mul, 1) \
T (106, unsigned WTYPE, signed WTYPE, signed char, 5, -2, -10, mul, 0) \
T (107, long long int, long long int, unsigned char, -3, 5, 2, add, 0) \
T (108, long long int, int, unsigned char, -5, 3, -2, add, 1) \
T (109, int, WTYPE, unsigned char, -3, 5, 2, add, 0) \
T (110, unsigned char, unsigned char, unsigned WTYPE, SCHAR_MAX - 1, (unsigned char) SCHAR_MAX + 4, -5, sub, 1)

TESTS

#undef T
#define T(n, t1, t2, tr, v1, v2, vr, b, o) t##n##b ();

int
main ()
{
  TESTS
  return 0;
}
