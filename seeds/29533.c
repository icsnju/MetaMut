/* { dg-options "-O2 -fno-tree-loop-distribute-patterns" } */
/* { dg-final { check-function-bodies "**" "" "" { target lp64 } } } */

#include <stdint.h>

#define PRAGMA(X) _Pragma (#X)
#define UNROLL(COUNT) PRAGMA (GCC unroll (COUNT))

#define CONST_FN(COUNT, TYPE, VAL)		\
  void						\
  const_##COUNT##_##TYPE##_##VAL (TYPE *x)	\
  {						\
    UNROLL (COUNT)				\
    for (int i = 0; i < COUNT; ++i)		\
      x[i] = VAL;				\
  }

#define DUP_FN(COUNT, TYPE)			\
  void						\
  dup_##COUNT##_##TYPE (TYPE *x, TYPE val)	\
  {						\
    UNROLL (COUNT)				\
    for (int i = 0; i < COUNT; ++i)		\
      x[i] = val;				\
  }

#define CONS2_FN(COUNT, TYPE)					\
  void								\
  cons2_##COUNT##_##TYPE (TYPE *x, TYPE val0, TYPE val1)	\
  {								\
    UNROLL (COUNT)						\
    for (int i = 0; i < COUNT * 2; i += 2)			\
      {								\
	x[i + 0] = val0;					\
	x[i + 1] = val1;					\
      }								\
  }

#define CONS4_FN(COUNT, TYPE)					\
  void								\
  cons4_##COUNT##_##TYPE (TYPE *x, TYPE val0, TYPE val1,	\
			  TYPE val2, TYPE val3)			\
  {								\
    UNROLL (COUNT)						\
    for (int i = 0; i < COUNT * 4; i += 4)			\
      {								\
	x[i + 0] = val0;					\
	x[i + 1] = val1;					\
	x[i + 2] = val2;					\
	x[i + 3] = val3;					\
      }								\
  }


/*
** const_2_int64_t_0:
**	stp	xzr, xzr, \[x0\]
**	ret
*/
CONST_FN (2, int64_t, 0);

/* No preference between vectorizing or not vectorizing here.  */
CONST_FN (4, int64_t, 0);

/*
** const_8_int64_t_0:
**	movi	v([0-9]+)\.4s, .*
**	stp	q\1, q\1, \[x0\]
**	stp	q\1, q\1, \[x0, #?32\]
**	ret
*/
CONST_FN (8, int64_t, 0);

/*
** dup_2_int64_t:
**	stp	x1, x1, \[x0\]
**	ret
*/
DUP_FN (2, int64_t);

/*
** dup_4_int64_t:
**	stp	x1, x1, \[x0\]
**	stp	x1, x1, \[x0, #?16\]
**	ret
*/
DUP_FN (4, int64_t);

/* No preference between vectorizing or not vectorizing here.  */
DUP_FN (8, int64_t);

/*
** dup_16_int64_t:
**	dup	v([0-9]+)\.2d, x1
**	stp	q\1, q\1, \[x0\]
**	stp	q\1, q\1, \[x0, #?32\]
**	stp	q\1, q\1, \[x0, #?64\]
**	stp	q\1, q\1, \[x0, #?96\]
**	ret
*/
DUP_FN (16, int64_t);

/*
** cons2_1_int64_t:
**	stp	x1, x2, \[x0\]
**	ret
*/
CONS2_FN (1, int64_t);

/*
** cons2_2_int64_t:
**	stp	x1, x2, \[x0\]
**	stp	x1, x2, \[x0, #?16\]
**	ret
*/
CONS2_FN (2, int64_t);

/*
** cons2_4_int64_t:
**	stp	x1, x2, \[x0\]
**	stp	x1, x2, \[x0, #?16\]
**	stp	x1, x2, \[x0, #?32\]
**	stp	x1, x2, \[x0, #?48\]
**	ret
*/
CONS2_FN (4, int64_t);

/* No preference between vectorizing or not vectorizing here.  */
CONS2_FN (8, int64_t);

/*
** cons2_16_int64_t:
**	...
**	stp	q[0-9]+, .*
**	ret
*/
CONS2_FN (16, int64_t);

/*
** cons4_1_int64_t:
**	stp	x1, x2, \[x0\]
**	stp	x3, x4, \[x0, #?16\]
**	ret
*/
CONS4_FN (1, int64_t);

/*
** cons4_2_int64_t:
**	stp	x1, x2, \[x0\]
**	stp	x3, x4, \[x0, #?16\]
**	stp	x1, x2, \[x0, #?32\]
**	stp	x3, x4, \[x0, #?48\]
**	ret
*/
CONS4_FN (2, int64_t);

/* No preference between vectorizing or not vectorizing here.  */
CONS4_FN (4, int64_t);

/* We should probably vectorize this, but currently don't.  */
CONS4_FN (8, int64_t);

/*
** cons4_16_int64_t:
**	...
**	stp	q[0-9]+, .*
**	ret
*/
CONS4_FN (16, int64_t);
