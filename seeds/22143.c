/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param=riscv-autovec-preference=scalable" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=scalable -ffast-math -fno-vect-cost-model" } */

#include <stdint-gcc.h>

#define NUM_ELEMS(TYPE) (1024 / sizeof (TYPE))

#define DEF_REDUC_PLUS(TYPE)					\
void __attribute__ ((noinline, noclone))			\
reduc_plus_##TYPE (TYPE (*restrict a)[NUM_ELEMS (TYPE)],	\
		   TYPE *restrict r, int n)			\
{								\
  for (int i = 0; i < n; i++)					\
    {								\
      r[i] = 0;							\
      for (int j = 0; j < NUM_ELEMS (TYPE); j++)		\
        r[i] += a[i][j];					\
    }								\
}

#define TEST_PLUS(T)				\
  T (int8_t)					\
  T (int16_t)					\
  T (int32_t)					\
  T (int64_t)					\
  T (uint8_t)					\
  T (uint16_t)					\
  T (uint32_t)					\
  T (uint64_t)					\
  T (_Float16)					\
  T (float)					\
  T (double)

TEST_PLUS (DEF_REDUC_PLUS)

#define DEF_REDUC_MAXMIN(TYPE, NAME, CMP_OP)			\
void __attribute__ ((noinline, noclone))			\
reduc_##NAME##_##TYPE (TYPE (*restrict a)[NUM_ELEMS (TYPE)],	\
		       TYPE *restrict r, int n)			\
{								\
  for (int i = 0; i < n; i++)					\
    {								\
      r[i] = a[i][0];						\
      for (int j = 0; j < NUM_ELEMS (TYPE); j++)		\
        r[i] = a[i][j] CMP_OP r[i] ? a[i][j] : r[i];		\
    }								\
}

#define TEST_MAXMIN(T)				\
  T (int8_t, max, >)				\
  T (int16_t, max, >)				\
  T (int32_t, max, >)				\
  T (int64_t, max, >)				\
  T (uint8_t, max, >)				\
  T (uint16_t, max, >)				\
  T (uint32_t, max, >)				\
  T (uint64_t, max, >)				\
  T (_Float16, max, >)				\
  T (float, max, >)				\
  T (double, max, >)				\
						\
  T (int8_t, min, <)				\
  T (int16_t, min, <)				\
  T (int32_t, min, <)				\
  T (int64_t, min, <)				\
  T (uint8_t, min, <)				\
  T (uint16_t, min, <)				\
  T (uint32_t, min, <)				\
  T (uint64_t, min, <)				\
  T (_Float16, min, <)				\
  T (float, min, <)				\
  T (double, min, <)

TEST_MAXMIN (DEF_REDUC_MAXMIN)

#define DEF_REDUC_BITWISE(TYPE,NAME,BIT_OP)			\
void __attribute__ ((noinline, noclone))			\
reduc_##NAME##TYPE (TYPE (*restrict a)[NUM_ELEMS(TYPE)],	\
		    TYPE *restrict r, int n)			\
{								\
  for (int i = 0; i < n; i++)					\
    {								\
      r[i] = a[i][0];						\
      for (int j = 0; j < NUM_ELEMS(TYPE); j++)			\
        r[i] BIT_OP a[i][j];					\
    }								\
}

#define TEST_BITWISE(T)				\
  T (int8_t, and, &=)				\
  T (int16_t, and, &=)				\
  T (int32_t, and, &=)				\
  T (int64_t, and, &=)				\
  T (uint8_t, and, &=)				\
  T (uint16_t, and, &=)				\
  T (uint32_t, and, &=)				\
  T (uint64_t, and, &=)				\
						\
  T (int8_t, ior, |=)				\
  T (int16_t, ior, |=)				\
  T (int32_t, ior, |=)				\
  T (int64_t, ior, |=)				\
  T (uint8_t, ior, |=)				\
  T (uint16_t, ior, |=)				\
  T (uint32_t, ior, |=)				\
  T (uint64_t, ior, |=)				\
						\
  T (int8_t, xor, ^=)				\
  T (int16_t, xor, ^=)				\
  T (int32_t, xor, ^=)				\
  T (int64_t, xor, ^=)				\
  T (uint8_t, xor, ^=)				\
  T (uint16_t, xor, ^=)				\
  T (uint32_t, xor, ^=)				\
  T (uint64_t, xor, ^=)

TEST_BITWISE (DEF_REDUC_BITWISE)

/* { dg-final { scan-assembler-times {vredsum\.vs\s+v[0-9]+,\s*v[0-9]+,\s*v[0-9]+} 8 } } */
/* { dg-final { scan-assembler-times {vredmax\.vs\s+v[0-9]+,\s*v[0-9]+,\s*v[0-9]+} 4 } } */
/* { dg-final { scan-assembler-times {vredmaxu\.vs\s+v[0-9]+,\s*v[0-9]+,\s*v[0-9]+} 4 } } */
/* { dg-final { scan-assembler-times {vredmin\.vs\s+v[0-9]+,\s*v[0-9]+,\s*v[0-9]+} 4 } } */
/* { dg-final { scan-assembler-times {vredminu\.vs\s+v[0-9]+,\s*v[0-9]+,\s*v[0-9]+} 4 } } */
/* { dg-final { scan-assembler-times {vredand\.vs\s+v[0-9]+,\s*v[0-9]+,\s*v[0-9]+} 8 } } */
/* { dg-final { scan-assembler-times {vredor\.vs\s+v[0-9]+,\s*v[0-9]+,\s*v[0-9]+} 8 } } */
/* { dg-final { scan-assembler-times {vredxor\.vs\s+v[0-9]+,\s*v[0-9]+,\s*v[0-9]+} 8 } } */
/* { dg-final { scan-assembler-times {vfredusum\.vs\s+v[0-9]+,\s*v[0-9]+,\s*v[0-9]+} 3 } } */
/* { dg-final { scan-assembler-times {vfredmax\.vs\s+v[0-9]+,\s*v[0-9]+,\s*v[0-9]+} 3 } } */
/* { dg-final { scan-assembler-times {vfredmin\.vs\s+v[0-9]+,\s*v[0-9]+,\s*v[0-9]+} 3 } } */

#define NROWS 53

/* -ffast-math fuzz for PLUS.  */
#define CMP__Float16(X, Y) ((X) >= (Y) * 0.875 && (X) <= (Y) * 1.125)
#define CMP_float(X, Y) ((X) == (Y))
#define CMP_double(X, Y) ((X) == (Y))
#define CMP_int8_t(X, Y) ((X) == (Y))
#define CMP_int16_t(X, Y) ((X) == (Y))
#define CMP_int32_t(X, Y) ((X) == (Y))
#define CMP_int64_t(X, Y) ((X) == (Y))
#define CMP_uint8_t(X, Y) ((X) == (Y))
#define CMP_uint16_t(X, Y) ((X) == (Y))
#define CMP_uint32_t(X, Y) ((X) == (Y))
#define CMP_uint64_t(X, Y) ((X) == (Y))

#define INIT_MATRIX(TYPE)				\
  TYPE mat[NROWS][NUM_ELEMS (TYPE)];			\
  TYPE r[NROWS];					\
  for (int i = 0; i < NROWS; i++)			\
    for (int j = 0; j < NUM_ELEMS (TYPE); j++)		\
      {							\
	mat[i][j] = i + (j * 2) * (j & 1 ? 1 : -1);	\
	asm volatile ("" ::: "memory");			\
      }

#define TEST_REDUC_PLUS(TYPE)				\
  {							\
    INIT_MATRIX (TYPE);					\
    reduc_plus_##TYPE (mat, r, NROWS);			\
    for (int i = 0; i < NROWS; i++)			\
      {							\
	volatile TYPE r2 = 0;				\
	for (int j = 0; j < NUM_ELEMS (TYPE); ++j)	\
	  r2 += mat[i][j];				\
	if (!CMP_##TYPE (r[i], r2))			\
	  __builtin_abort ();				\
      }							\
    }

#define TEST_REDUC_MAXMIN(TYPE, NAME, CMP_OP)		\
  {							\
    INIT_MATRIX (TYPE);					\
    reduc_##NAME##_##TYPE (mat, r, NROWS);		\
    for (int i = 0; i < NROWS; i++)			\
      {							\
	volatile TYPE r2 = mat[i][0];			\
	for (int j = 0; j < NUM_ELEMS (TYPE); ++j)	\
	  r2 = mat[i][j] CMP_OP r2 ? mat[i][j] : r2;	\
	if (r[i] != r2)					\
	  __builtin_abort ();				\
      }							\
    }

#define TEST_REDUC_BITWISE(TYPE, NAME, BIT_OP)		\
  {							\
    INIT_MATRIX (TYPE);					\
    reduc_##NAME##_##TYPE (mat, r, NROWS);		\
    for (int i = 0; i < NROWS; i++)			\
      {							\
	volatile TYPE r2 = mat[i][0];			\
	for (int j = 0; j < NUM_ELEMS (TYPE); ++j)	\
	  r2 BIT_OP mat[i][j];				\
	if (r[i] != r2)					\
	  __builtin_abort ();				\
      }							\
    }

int main ()
{
  TEST_PLUS (TEST_REDUC_PLUS)
  TEST_MAXMIN (TEST_REDUC_MAXMIN)

  return 0;
}
