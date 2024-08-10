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
** const_2_float_0:
**	str	xzr, \[x0\]
**	ret
*/
CONST_FN (2, float, 0);

/*
** const_4_float_0:
**	stp	xzr, xzr, \[x0\]
**	ret
*/
CONST_FN (4, float, 0);

/* No preference between vectorizing or not vectorizing here.  */
CONST_FN (8, float, 0);

/*
** const_16_float_0:
**	movi	v([0-9]+)\.4s, .*
**	stp	q\1, q\1, \[x0\]
**	stp	q\1, q\1, \[x0, #?32\]
**	ret
*/
CONST_FN (16, float, 0);

/*
** const_2_float_1:
**	fmov	v([0-9]+)\.2s, .*
**	str	d\1, \[x0\]
**	ret
*/
CONST_FN (2, float, 1);

/*
** const_4_float_1:
**	fmov	v([0-9]+)\.4s, .*
**	str	q\1, \[x0\]
**	ret
*/
CONST_FN (4, float, 1);

/*
** dup_2_float:
**	stp	s0, s0, \[x0\]
**	ret
*/
DUP_FN (2, float);

/* No preference between vectorizing or not vectorizing here.  */
DUP_FN (4, float);

/*
** dup_8_float:
**	dup	v([0-9]+)\.4s, v0.s\[0\]
**	stp	q\1, q\1, \[x0\]
**	ret
*/
DUP_FN (8, float);

/*
** cons2_1_float:
**	stp	s0, s1, \[x0\]
**	ret
*/
CONS2_FN (1, float);

/*
** cons2_2_float:
**	stp	s0, s1, \[x0\]
**	stp	s0, s1, \[x0, #?8\]
**	ret
*/
CONS2_FN (2, float);

/*
** cons2_4_float:	{ target aarch64_little_endian }
**	ins	v0.s\[1\], v1.s\[0\]
**	stp	d0, d0, \[x0\]
**	stp	d0, d0, \[x0, #?16\]
**	ret
*/
/*
** cons2_4_float:	{ target aarch64_big_endian }
**	ins	v1.s\[1\], v0.s\[0\]
**	stp	d1, d1, \[x0\]
**	stp	d1, d1, \[x0, #?16\]
**	ret
*/
CONS2_FN (4, float);

/*
** cons2_8_float:
**	dup	v[0-9]+\.2s, v[0-9]+\.s\[0\]
**	dup	v[0-9]+\.2s, v[0-9]+\.s\[0\]
**	zip1	v([0-9]+)\.4s, v[0-9]+\.4s, v[0-9]+\.4s
**	stp	q\1, q\1, \[x0\]
**	stp	q\1, q\1, \[x0, #?32\]
**	ret
*/
CONS2_FN (8, float);

/*
** cons4_1_float:
**	stp	s0, s1, \[x0\]
**	stp	s2, s3, \[x0, #?8\]
**	ret
*/
CONS4_FN (1, float);

/*
** cons4_2_float:
**	stp	s0, s1, \[x0\]
**	stp	s2, s3, \[x0, #?8\]
**	stp	s0, s1, \[x0, #?16\]
**	stp	s2, s3, \[x0, #?24\]
**	ret
*/
CONS4_FN (2, float);

/*
** cons4_4_float:
**	ins	v([0-9]+)\.s.*
**	...
**	stp	q\1, q\1, \[x0\]
**	stp	q\1, q\1, \[x0, #?32\]
**	ret
*/
CONS4_FN (4, float);
