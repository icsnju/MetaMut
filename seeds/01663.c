/* Test __builtin_{add,sub,mul,{s,u}add,{s,u}sub,{s,u}mul}_overflow.  */
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

#define TESTS(type, min, max) \
ST (100, signed type, 2, 3, 5, U(s, add), 0) \
ST (101, signed type, max, -1, max - 1, U(s, add), 0) \
ST (102, signed type, max, 0, max, U(s, add), 0) \
ST (103, signed type, 1, max, min, U(s, add), 1) \
ST (104, signed type, 0, min, min, U(s, sub), 1) \
ST (110, signed type, 2, 3, -1, U(s, sub), 0) \
ST (111, signed type, max, -1, min, U(s, sub), 1) \
ST (112, signed type, max, 0, max, U(s, sub), 0) \
ST (113, signed type, 1, max, min + 2, U(s, sub), 0) \
ST (114, signed type, max, -1, min, U(s, sub), 1) \
ST (120, signed type, 2, 3, 6, U(s, mul), 0) \
ST (122, signed type, min, -1, min, U(s, mul), 1) \
ST (123, signed type, max, 0, 0, U(s, mul), 0) \
ST (124, signed type, 1, max, max, U(s, mul), 0) \
ST (125, signed type, max, 2, -2, U(s, mul), 1) \
ST (126, signed type, max / 25, 25, max / 25 * 25, U(s, mul), 0) \
ST (127, signed type, max / 25 + 1, 25, max / 25 * 25 + (unsigned type) 25, U(s, mul), 1) \
ST (150, unsigned type, 2, 3, 5, U(u, add), 0) \
ST (151, unsigned type, -1, -1, -2, U(u, add), 1) \
ST (152, unsigned type, -1, 0, -1, U(u, add), 0) \
ST (153, unsigned type, 1, -1, 0, U(u, add), 1) \
ST (154, unsigned type, 0, min, min, U(u, sub), 1) \
ST (160, unsigned type, 2, 3, -1, U(u, sub), 1) \
ST (161, unsigned type, -1, -1, 0, U(u, sub), 0) \
ST (162, unsigned type, -1, 0, -1, U(u, sub), 0) \
ST (163, unsigned type, 1, -1, 2, U(u, sub), 1) \
ST (164, unsigned type, 15, 14, 1, U(u, sub), 0) \
ST (170, unsigned type, 2, 3, 6, U(u, mul), 0) \
ST (171, unsigned type, max, 3, 3 * (unsigned type) max, U(u, mul), 1) \
ST (172, unsigned type, -1, 0, 0, U(u, mul), 0) \
ST (173, unsigned type, 1, -1, -1, U(u, mul), 0) \
ST (174, unsigned type, -1, 2, -2, U(u, mul), 1) \
ST (175, unsigned type, ((unsigned type) -1) / 25, 25, ((unsigned type) -1) / 25 * 25, U(u, mul), 0) \
ST (176, unsigned type, ((unsigned type) -1) / 25 + 1, 25, ((unsigned type) -1) / 25 * 25 + (unsigned type) 25, U(u, mul), 1)

#define U(s, op) s##op
TESTS (int, INT_MIN, INT_MAX)
#undef U
#define U(s, op) op
TESTS (int, INT_MIN, INT_MAX)

#undef T
#define T(n, t1, t2, tr, v1, v2, vr, b, o) t##n##b ();

int
main ()
{
  TESTS (int, INT_MIN, INT_MAX)
#undef U
#define U(s, op) s##op
  TESTS (int, INT_MIN, INT_MAX)
  return 0;
}
