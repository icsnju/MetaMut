/* PR c/102989 */
/* { dg-do run { target bitint } } */
/* { dg-options "-std=gnu2x" } */
/* { dg-skip-if "" { ! run_expensive_tests }  { "*" } { "-O0" "-O2" } } */
/* { dg-skip-if "" { ! run_expensive_tests } { "-flto" } { "" } } */

#if __BITINT_MAXWIDTH__ >= 575
#define OVFP
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
T (100, signed _BitInt(38), signed char, unsigned _BitInt(162), -1wb, 0, -1wb, add, 1) \
T (101, unsigned char, unsigned _BitInt(38), unsigned _BitInt(162), 5, 5wb, 10wb, add, 0) \
T (102, signed _BitInt(38), unsigned short, unsigned _BitInt(162), 5wb, 5, 0wb, sub, 0) \
T (103, signed _BitInt(38), unsigned _BitInt(72), unsigned _BitInt(162), 5wb, 6wb, -1wb, sub, 1) \
T (104, signed _BitInt(38), signed _BitInt(38), unsigned _BitInt(162), -1wb, -1wb, 1wb, mul, 0) \
T (105, unsigned _BitInt(38), signed _BitInt(38), unsigned _BitInt(162), 17wb, -2wb, -34wb, mul, 1) \
T (106, unsigned _BitInt(162), signed _BitInt(162), signed _BitInt(38), 5wb, -2wb, -10wb, mul, 0) \
T (107, signed _BitInt(321), signed _BitInt(321), unsigned _BitInt(38), -3wb, 5wb, 2wb, add, 0) \
T (108, signed _BitInt(321), int, unsigned _BitInt(38), -5wb, 3, -2wb, add, 1) \
T (109, int, _BitInt(162), unsigned _BitInt(38), -3, 5wb, 2wb, add, 0) \
T (110, unsigned _BitInt(38), unsigned _BitInt(38), unsigned _BitInt(162), 137438953471wb - 1, (unsigned _BitInt(38)) 137438953471wb + 4, -5wb, sub, 1) \
T (111, signed _BitInt(315), signed _BitInt(315), unsigned, -1wb, 0wb, -1, add, 1) \
T (112, signed _BitInt(315), signed _BitInt(315), unsigned, 5wb, 42wb, 47, add, 0) \
T (113, signed _BitInt(315), signed _BitInt(315), unsigned WTYPE, -1wb, 0wb, -1, add, 1) \
T (114, signed _BitInt(315), signed _BitInt(315), unsigned WTYPE, 15wb, 5wb, 20, add, 0)
T (115, signed _BitInt(315), signed _BitInt(315), unsigned _BitInt(135), -1wb, 0wb, -1wb, add, 1) \
T (116, signed _BitInt(315), signed _BitInt(315), unsigned _BitInt(135), 5wb, 125wb, 130wb, add, 0) \
T (117, signed _BitInt(275), signed _BitInt(275), unsigned _BitInt(125), -1wb, 0wb, -1wb, add, 1) \
T (118, signed _BitInt(275), signed _BitInt(275), unsigned _BitInt(125), 5wb, 125wb, 130wb, add, 0) \
T (119, signed _BitInt(415), signed _BitInt(415), signed _BitInt(178), 95780971304118053647396689196894323976171195136475135wb, 95780971304118053647396689196894323976171195136475137wb, -191561942608236107294793378393788647952342390272950271wb - 1, add, 1) \
T (120, signed _BitInt(415), signed _BitInt(415), signed _BitInt(178), 95780971304118053647396689196894323976171195136475135wb, 95780971304118053647396689196894323976171195136475136wb, 191561942608236107294793378393788647952342390272950271wb, add, 0) \
T (121, signed _BitInt(439), signed _BitInt(439), signed _BitInt(192), 1569275433846670190958947355801916604025588861116008628223wb, 1569275433846670190958947355801916604025588861116008628225wb, -3138550867693340381917894711603833208051177722232017256447wb - 1, add, 1) \
T (122, signed _BitInt(439), signed _BitInt(439), signed _BitInt(192), 1569275433846670190958947355801916604025588861116008628223wb, 1569275433846670190958947355801916604025588861116008628224wb, 3138550867693340381917894711603833208051177722232017256447wb, add, 0) \
T (123, signed _BitInt(575), signed _BitInt(575), signed _BitInt(193), 3138550867693340381917894711603833208051177722232017256447wb, 3138550867693340381917894711603833208051177722232017256449wb, -6277101735386680763835789423207666416102355444464034512895wb - 1, add, 1) \
T (124, signed _BitInt(575), signed _BitInt(575), signed _BitInt(193), 3138550867693340381917894711603833208051177722232017256447wb, 3138550867693340381917894711603833208051177722232017256448wb, 6277101735386680763835789423207666416102355444464034512895wb, add, 0)

TESTS

#undef T
#define T(n, t1, t2, tr, v1, v2, vr, b, o) t##n##b ();
#endif

int
main ()
{
#if __BITINT_MAXWIDTH__ >= 575
  TESTS
#endif
  return 0;
}
