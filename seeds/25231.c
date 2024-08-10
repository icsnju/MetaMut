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
** const_2_int16_t_0:
**	str	wzr, \[x0\]
**	ret
*/
CONST_FN (2, int16_t, 0);

/*
** const_4_int16_t_0:
**	str	xzr, \[x0\]
**	ret
*/
CONST_FN (4, int16_t, 0);

/*
** const_8_int16_t_0:
**	stp	xzr, xzr, \[x0\]
**	ret
*/
CONST_FN (8, int16_t, 0);

/* No preference between vectorizing or not vectorizing here.  */
CONST_FN (16, int16_t, 0);

/*
** const_32_int16_t_0:
**	movi	v([0-9]+)\.4s, .*
**	stp	q\1, q\1, \[x0\]
**	stp	q\1, q\1, \[x0, #?32\]
**	ret
*/
CONST_FN (32, int16_t, 0);

/* No preference between vectorizing or not vectorizing here.  */
CONST_FN (2, int16_t, 1);

/*
** const_4_int16_t_1:
**	movi	v([0-9]+)\.4h, .*
**	str	d\1, \[x0\]
**	ret
*/
CONST_FN (4, int16_t, 1);

/*
** const_8_int16_t_1:
**	movi	v([0-9]+)\.8h, .*
**	str	q\1, \[x0\]
**	ret
*/
CONST_FN (8, int16_t, 1);

/* Fuzzy match due to PR104387.  */
/*
** dup_2_int16_t:
**	...
**	strh	w1, \[x0, #?2\]
**	ret
*/
DUP_FN (2, int16_t);

/*
** dup_4_int16_t:
**	dup	v([0-9]+)\.4h, w1
**	str	d\1, \[x0\]
**	ret
*/
DUP_FN (4, int16_t);

/*
** dup_8_int16_t:
**	dup	v([0-9]+)\.8h, w1
**	str	q\1, \[x0\]
**	ret
*/
DUP_FN (8, int16_t);

/*
** cons2_1_int16_t:
**	strh	w1, \[x0\]
**	strh	w2, \[x0, #?2\]
**	ret
*/
CONS2_FN (1, int16_t);
