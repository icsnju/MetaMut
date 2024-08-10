/* Test __builtin_{add,sub,mul}_overflow_p.  */
/* { dg-do run } */
/* { dg-skip-if "" { ! run_expensive_tests }  { "*" } { "-O0" "-O2" } } */

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

#define TESTS(type, min, max) \
T (100, signed type, unsigned type, unsigned type, -1, 0, 0, mul, 0) \
T (101, signed type, unsigned type, unsigned type, -1, 1, (unsigned type) -1, mul, 1) \
T (102, unsigned type, signed type, unsigned type, 12, -3, (unsigned type) -36, mul, 1) \
T (103, signed type, unsigned type, unsigned type, 3, 4, 12, mul, 0) \
T (104, unsigned type, signed type, unsigned type, (unsigned type) -1 / 12, 12, (unsigned type) -1 / 12 * 12, mul, 0) \
T (105, unsigned type, signed type, unsigned type, (unsigned type) -1 / 12, 13, (unsigned type) -1 / 12 * 13, mul, 1) \
T (106, unsigned type, unsigned type, signed type, 0, 0, 0, mul, 0) \
T (107, unsigned type, unsigned type, signed type, max / 31, 31, (signed type) ((unsigned type) max / 31 * 31), mul, 0) \
T (108, unsigned type, unsigned type, signed type, max / 31, 32, (signed type) ((unsigned type) max / 31 * 32), mul, 1) \
T (109, unsigned type, unsigned type, signed type, max / 31, 65, (signed type) ((unsigned type) max / 31 * 65), mul, 1) \
T (110, signed type, unsigned type, signed type, -1, 7, -7, mul, 0) \
T (111, unsigned type, signed type, signed type, 2, min / 2, min, mul, 0) \
T (112, signed type, unsigned type, signed type, max / 12, 13, (signed type) ((unsigned type) max / 12 * 13), mul, 1) \
T (113, unsigned type, signed type, signed type, (unsigned type) max + 19, 0, 0, mul, 0) \
T (114, signed type, unsigned type, signed type, 0, (unsigned type) max + 1, 0, mul, 0) \
T (115, unsigned type, signed type, signed type, (unsigned type) max + 1, -1, min, mul, 0) \
T (116, signed type, unsigned type, signed type, -1, (unsigned type) max + 2, max, mul, 1) \
T (117, signed type, signed type, unsigned type, min / 64, -64, (unsigned type) min, mul, 0) \
T (118, signed type, signed type, unsigned type, min / 32, -33, ((unsigned type) max + 1) / 32 * 33, mul, 0) \
T (119, signed type, signed type, unsigned type, min / 32, -65, ((unsigned type) max + 1) / 32 * 65, mul, 1) \
T (120, signed type, signed type, unsigned type, -1, -1, 1, mul, 0) \
T (121, signed type, signed type, unsigned type, 0, 0, 0, mul, 0) \
T (122, signed type, signed type, unsigned type, 0, -6, 0, mul, 0) \
T (123, signed type, signed type, unsigned type, -15, 0, 0, mul, 0) \
T (124, signed type, signed type, unsigned type, -1, 1, ~(unsigned type) 0, mul, 1) \
T (125, signed type, signed type, unsigned type, -17, 5, (unsigned type) -17 * 5, mul, 1) \
T (126, signed type, signed type, unsigned type, 7, max / 7, max / 7 * 7, mul, 0) \
T (127, signed type, signed type, unsigned type, max / 7, 8, (unsigned type) max / 7 * 8, mul, 0) \
T (128, signed type, signed type, unsigned type, 15, max / 7, (unsigned type) max / 7 * 15, mul, 1) \
T (129, signed type, unsigned type, signed type, min, 5, min + 5, add, 0) \
T (130, unsigned type, signed type, signed type, ~(unsigned type) 0, min, max, add, 0) \
T (131, signed type, unsigned type, signed type, max, 1, min, add, 1) \
T (132, unsigned type, signed type, signed type, max / 2, max / 2 + 1, max, add, 0) \
T (133, signed type, unsigned type, signed type, max / 2 + 1, max / 2 + 1, min, add, 1) \
T (134, signed type, unsigned type, unsigned type, min, ~(unsigned type) 0, max, add, 0) \
T (135, unsigned type, signed type, unsigned type, ~(unsigned type) 0, min + 1, (unsigned type) max + 1, add, 0) \
T (136, signed type, unsigned type, unsigned type, 1, ~(unsigned type) 0, 0, add, 1) \
T (137, unsigned type, signed type, unsigned type, 2, -3, ~(unsigned type) 0, add, 1) \
T (138, signed type, unsigned type, signed type, min, 1, max, sub, 1) \
T (139, signed type, unsigned type, signed type, min + 1, 1, min, sub, 0) \
T (140, signed type, unsigned type, signed type, max, (unsigned type) max + 1, -1, sub, 0) \
T (141, signed type, unsigned type, signed type, max, ~(unsigned type) 0, min, sub, 0) \
T (142, signed type, unsigned type, signed type, max - 1, ~(unsigned type) 0, max, sub, 1) \
T (143, signed type, unsigned type, unsigned type, -1, 0, ~(unsigned type) 0, sub, 1) \
T (144, signed type, unsigned type, unsigned type, -1, ~(unsigned type) 0, 0, sub, 1) \
T (145, signed type, unsigned type, unsigned type, min, 0, min, sub, 1) \
T (146, signed type, unsigned type, unsigned type, max, max, 0, sub, 0) \
T (147, signed type, unsigned type, unsigned type, max, (unsigned type) max + 1, -1, sub, 1) \
T (148, signed type, unsigned type, unsigned type, max - 1, max, -1, sub, 1) \
T (149, unsigned type, signed type, signed type, 0, max, -max, sub, 0) \
T (150, unsigned type, signed type, signed type, (unsigned type) max + 1, 0, min, sub, 1) \
T (151, unsigned type, signed type, signed type, (unsigned type) max + 1, 1, max, sub, 0) \
T (152, unsigned type, unsigned type, signed type, 0, (unsigned type) max + 1, min, add, 1) \
T (153, signed type, signed type, unsigned type, -1, 0, -1, add, 1) \
T (154, unsigned type, signed type, unsigned type, 5, 6, -1, sub, 1) \
T (155, unsigned type, signed type, unsigned type, ~(unsigned type) 0, max, (unsigned type) max + 1, sub, 0) \
T (156, unsigned type, signed type, unsigned type, (unsigned type) max + 1, min, 0, sub, 1) \
T (157, signed type, signed type, unsigned type, 3, -2, 1, add, 0) \
T (158, signed type, signed type, unsigned type, 3, -4, -1, add, 1) \
T (159, signed type, signed type, unsigned type, -3, -4, -7, add, 1) \
T (160, signed type, signed type, unsigned type, -5, 4, -1, add, 1) \
T (161, signed type, signed type, unsigned type, -5, 5, 0, add, 0) \
T (162, signed type, signed type, unsigned type, min, 1, min + 1, add, 1) \
T (163, unsigned type, unsigned type, signed type, max, 1, min, add, 1) \
T (164, unsigned type, unsigned type, signed type, max - 1, 1, max, add, 0) \
T (165, unsigned type, unsigned type, signed type, ~(unsigned type) 0, ~(unsigned type) 0, ~(unsigned type) 0 - 1, add, 1) \
T (166, unsigned type, unsigned type, signed type, (unsigned type) max + 3, 2, min, sub, 1) \
T (167, unsigned type, unsigned type, signed type, (unsigned type) max + 2, 2, max, sub, 0) \
T (168, unsigned type, unsigned type, signed type, (unsigned type) max + 2, (unsigned type) max + 3, -1, sub, 0) \
T (169, unsigned type, unsigned type, signed type, 0, (unsigned type) max + 1, min, sub, 0) \
T (170, unsigned type, unsigned type, signed type, 0, (unsigned type) max + 2, max, sub, 1) \
T (171, signed type, signed type, unsigned type, 3, 2, 1, sub, 0) \
T (172, signed type, signed type, unsigned type, 3, 4, -1, sub, 1) \
T (173, signed type, signed type, unsigned type, -3, 4, -7, sub, 1) \
T (174, signed type, signed type, unsigned type, -5, -4, -1, sub, 1) \
T (175, signed type, signed type, unsigned type, -5, -5, 0, sub, 0) \
T (176, signed type, signed type, unsigned type, min, -1, min + 1, sub, 1)

TESTS (char, SCHAR_MIN, SCHAR_MAX)

#undef T
#define T(n, t1, t2, tr, v1, v2, vr, b, o) t##n##b ();

int
main ()
{
  TESTS (char, SCHAR_MIN, SCHAR_MAX)
  return 0;
}
