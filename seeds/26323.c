/* { dg-do run { target { riscv_zvfh_hw } } } */
/* { dg-additional-options "-Wno-pedantic" } */

#include <assert.h>

/* { dg-do compile } */
/* { dg-additional-options "-march=rv64gcv_zvfh -mabi=lp64d -Wno-pedantic -Wno-psabi" } */

#include <stdint-gcc.h>

typedef int64_t vnx2di __attribute__((vector_size (16)));
typedef int32_t vnx4si __attribute__((vector_size (16)));
typedef int16_t vnx8hi __attribute__((vector_size (16)));
typedef int8_t vnx16qi __attribute__((vector_size (16)));
typedef _Float16 vnx8hf __attribute__((vector_size (16)));
typedef float vnx4sf __attribute__((vector_size (16)));
typedef double vnx2df __attribute__((vector_size (16)));

#define VEC_SET(S,V,IDX)			\
  V						\
  __attribute__((noipa))			\
  vec_set_##V##_##IDX (V v, S s)		\
  {						\
    v[IDX] = s;					\
    return v;					\
  }

#define VEC_SET_VAR1(S,V)			\
  V						\
  __attribute__((noipa))			\
  vec_set_var_##V (V v, int8_t idx, S s)	\
  {						\
    v[idx] = s;					\
    return v;					\
  }						\

#define TEST_ALL1(T)				\
  T (_Float16, vnx8hf, 0)			\
  T (_Float16, vnx8hf, 3)			\
  T (_Float16, vnx8hf, 7)			\
  T (float, vnx4sf, 0)				\
  T (float, vnx4sf, 1)				\
  T (float, vnx4sf, 3)				\
  T (double, vnx2df, 0)				\
  T (double, vnx2df, 1)				\
  T (int64_t, vnx2di, 0)			\
  T (int64_t, vnx2di, 1)			\
  T (int32_t, vnx4si, 0)			\
  T (int32_t, vnx4si, 1)			\
  T (int32_t, vnx4si, 3)			\
  T (int16_t, vnx8hi, 0)			\
  T (int16_t, vnx8hi, 2)			\
  T (int16_t, vnx8hi, 6)			\
  T (int8_t, vnx16qi, 0)			\
  T (int8_t, vnx16qi, 1)			\
  T (int8_t, vnx16qi, 7)			\
  T (int8_t, vnx16qi, 11)			\
  T (int8_t, vnx16qi, 15)			\

#define TEST_ALL_VAR1(T)			\
  T (_Float16, vnx8hf)				\
  T (float, vnx4sf)				\
  T (double, vnx2df)				\
  T (int64_t, vnx2di)				\
  T (int32_t, vnx4si)				\
  T (int16_t, vnx8hi)				\
  T (int8_t, vnx16qi)				\

TEST_ALL1 (VEC_SET)
TEST_ALL_VAR1 (VEC_SET_VAR1)

/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e8,\s*m1,\s*ta,\s*ma} 1 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e8,\s*m1,\s*tu,\s*ma} 5 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e16,\s*m1,\s*ta,\s*ma} 2 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e16,\s*m1,\s*tu,\s*ma} 6 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e32,\s*m1,\s*ta,\s*ma} 2 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e32,\s*m1,\s*tu,\s*ma} 6 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e64,\s*m1,\s*ta,\s*ma} 2 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e64,\s*m1,\s*tu,\s*ma} 4 } } */

/* { dg-final { scan-assembler-times {\tvmv.v.x} 13 } } */
/* { dg-final { scan-assembler-times {\tvfmv.v.f} 8 } } */
/* { dg-final { scan-assembler-times {\tvslideup.vi} 14 } } */
/* { dg-final { scan-assembler-times {\tvslideup.vx} 7 } } */

/* { dg-final { scan-assembler-times {\tvfmv.s.f} 3 } } */
/* { dg-final { scan-assembler-times {\tvmv.s.x} 4 } } */
/* { dg-do compile } */
/* { dg-additional-options "-march=rv64gcv_zvfh -mabi=lp64d -Wno-pedantic -Wno-psabi" } */

#include <stdint-gcc.h>

typedef int64_t vnx4di __attribute__((vector_size (32)));
typedef int32_t vnx8si __attribute__((vector_size (32)));
typedef int16_t vnx16hi __attribute__((vector_size (32)));
typedef int8_t vnx32qi __attribute__((vector_size (32)));
typedef _Float16 vnx16hf __attribute__((vector_size (32)));
typedef float vnx8sf __attribute__((vector_size (32)));
typedef double vnx4df __attribute__((vector_size (32)));

#define VEC_SET(S,V,IDX)			\
  V						\
  __attribute__((noipa))			\
  vec_set_##V##_##IDX (V v, S s)		\
  {						\
    v[IDX] = s;					\
    return v;					\
  }

#define VEC_SET_VAR2(S,V)			\
  V						\
  __attribute__((noipa))			\
  vec_set_var_##V (V v, int16_t idx, S s)	\
  {						\
    v[idx] = s;					\
    return v;					\
  }						\

#define TEST_ALL2(T)				\
  T (_Float16, vnx16hf, 0)			\
  T (_Float16, vnx16hf, 3)			\
  T (_Float16, vnx16hf, 7)			\
  T (_Float16, vnx16hf, 8)			\
  T (_Float16, vnx16hf, 15)			\
  T (float, vnx8sf, 0)				\
  T (float, vnx8sf, 1)				\
  T (float, vnx8sf, 3)				\
  T (float, vnx8sf, 4)				\
  T (float, vnx8sf, 7)				\
  T (double, vnx4df, 0)				\
  T (double, vnx4df, 1)				\
  T (double, vnx4df, 2)				\
  T (double, vnx4df, 3)				\
  T (int64_t, vnx4di, 0)			\
  T (int64_t, vnx4di, 1)			\
  T (int64_t, vnx4di, 2)			\
  T (int64_t, vnx4di, 3)			\
  T (int32_t, vnx8si, 0)			\
  T (int32_t, vnx8si, 1)			\
  T (int32_t, vnx8si, 3)			\
  T (int32_t, vnx8si, 4)			\
  T (int32_t, vnx8si, 7)			\
  T (int16_t, vnx16hi, 0)			\
  T (int16_t, vnx16hi, 1)			\
  T (int16_t, vnx16hi, 7)			\
  T (int16_t, vnx16hi, 8)			\
  T (int16_t, vnx16hi, 15)			\
  T (int8_t, vnx32qi, 0)			\
  T (int8_t, vnx32qi, 1)			\
  T (int8_t, vnx32qi, 15)			\
  T (int8_t, vnx32qi, 16)			\
  T (int8_t, vnx32qi, 31)			\

#define TEST_ALL_VAR2(T)			\
  T (_Float16, vnx16hf)				\
  T (float, vnx8sf)				\
  T (double, vnx4df)				\
  T (int64_t, vnx4di)				\
  T (int32_t, vnx8si)				\
  T (int16_t, vnx16hi)				\
  T (int8_t, vnx32qi)				\

TEST_ALL2 (VEC_SET)
TEST_ALL_VAR2 (VEC_SET_VAR2)

/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e8,\s*m2,\s*ta,\s*ma} 1 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e8,\s*m2,\s*tu,\s*ma} 5 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e16,\s*m2,\s*ta,\s*ma} 2 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e16,\s*m2,\s*tu,\s*ma} 10 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e32,\s*m2,\s*ta,\s*ma} 2 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e32,\s*m2,\s*tu,\s*ma} 10 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e64,\s*m2,\s*ta,\s*ma} 2 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e64,\s*m2,\s*tu,\s*ma} 8 } } */

/* { dg-final { scan-assembler-times {\tvmv.v.x} 19 } } */
/* { dg-final { scan-assembler-times {\tvfmv.v.f} 14 } } */
/* { dg-final { scan-assembler-times {\tvslideup.vi} 26 } } */
/* { dg-final { scan-assembler-times {\tvslideup.vx} 7 } } */

/* { dg-final { scan-assembler-times {\tvfmv.s.f} 3 } } */
/* { dg-final { scan-assembler-times {\tvmv.s.x} 4 } } */
/* { dg-do compile } */
/* { dg-additional-options "-march=rv64gcv_zvfh -mabi=lp64d -Wno-pedantic -Wno-psabi" } */

#include <stdint-gcc.h>

typedef int64_t vnx8di __attribute__((vector_size (64)));
typedef int32_t vnx16si __attribute__((vector_size (64)));
typedef int16_t vnx32hi __attribute__((vector_size (64)));
typedef int8_t vnx64qi __attribute__((vector_size (64)));
typedef _Float16 vnx32hf __attribute__((vector_size (64)));
typedef float vnx16sf __attribute__((vector_size (64)));
typedef double vnx8df __attribute__((vector_size (64)));

#define VEC_SET(S,V,IDX)			\
  V						\
  __attribute__((noipa))			\
  vec_set_##V##_##IDX (V v, S s)		\
  {						\
    v[IDX] = s;					\
    return v;					\
  }

#define VEC_SET_VAR3(S,V)			\
  V						\
  __attribute__((noipa))			\
  vec_set_var_##V (V v, int32_t idx, S s)	\
  {						\
    v[idx] = s;					\
    return v;					\
  }						\

#define TEST_ALL3(T)				\
  T (_Float16, vnx32hf, 0)			\
  T (_Float16, vnx32hf, 3)			\
  T (_Float16, vnx32hf, 7)			\
  T (_Float16, vnx32hf, 8)			\
  T (_Float16, vnx32hf, 16)			\
  T (_Float16, vnx32hf, 31)			\
  T (float, vnx16sf, 0)				\
  T (float, vnx16sf, 2)				\
  T (float, vnx16sf, 6)				\
  T (float, vnx16sf, 8)				\
  T (float, vnx16sf, 14)			\
  T (double, vnx8df, 0)				\
  T (double, vnx8df, 2)				\
  T (double, vnx8df, 4)				\
  T (double, vnx8df, 6)				\
  T (int64_t, vnx8di, 0)			\
  T (int64_t, vnx8di, 2)			\
  T (int64_t, vnx8di, 4)			\
  T (int64_t, vnx8di, 6)			\
  T (int32_t, vnx16si, 0)			\
  T (int32_t, vnx16si, 2)			\
  T (int32_t, vnx16si, 6)			\
  T (int32_t, vnx16si, 8)			\
  T (int32_t, vnx16si, 14)			\
  T (int16_t, vnx32hi, 0)			\
  T (int16_t, vnx32hi, 2)			\
  T (int16_t, vnx32hi, 14)			\
  T (int16_t, vnx32hi, 16)			\
  T (int16_t, vnx32hi, 30)			\
  T (int8_t, vnx64qi, 0)			\
  T (int8_t, vnx64qi, 2)			\
  T (int8_t, vnx64qi, 30)			\
  T (int8_t, vnx64qi, 32)			\
  T (int8_t, vnx64qi, 63)			\

#define TEST_ALL_VAR3(T)			\
  T (_Float16, vnx32hf)				\
  T (float, vnx16sf)				\
  T (double, vnx8df)				\
  T (int64_t, vnx8di)				\
  T (int32_t, vnx16si)				\
  T (int16_t, vnx32hi)				\
  T (int8_t, vnx64qi)				\

TEST_ALL3 (VEC_SET)
TEST_ALL_VAR3 (VEC_SET_VAR3)

/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e8,\s*m4,\s*ta,\s*ma} 1 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e8,\s*m4,\s*tu,\s*ma} 5 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e16,\s*m4,\s*ta,\s*ma} 2 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e16,\s*m4,\s*tu,\s*ma} 11 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e32,\s*m4,\s*ta,\s*ma} 2 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e32,\s*m4,\s*tu,\s*ma} 10 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e64,\s*m4,\s*ta,\s*ma} 2 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e64,\s*m4,\s*tu,\s*ma} 8 } } */

/* { dg-final { scan-assembler-times {\tvmv.v.x} 19 } } */
/* { dg-final { scan-assembler-times {\tvfmv.v.f} 15 } } */
/* { dg-final { scan-assembler-times {\tvslideup.vi} 25 } } */
/* { dg-final { scan-assembler-times {\tvslideup.vx} 9 } } */

/* { dg-final { scan-assembler-times {\tvfmv.s.f} 3 } } */
/* { dg-final { scan-assembler-times {\tvmv.s.x} 4 } } */
/* { dg-do compile } */
/* { dg-additional-options "-march=rv64gcv_zvfh -mabi=lp64d -Wno-pedantic -Wno-psabi" } */

#include <stdint-gcc.h>

typedef int64_t vnx16di __attribute__((vector_size (128)));
typedef int32_t vnx32si __attribute__((vector_size (128)));
typedef int16_t vnx64hi __attribute__((vector_size (128)));
typedef int8_t vnx128qi __attribute__((vector_size (128)));
typedef _Float16 vnx64hf __attribute__((vector_size (128)));
typedef float vnx32sf __attribute__((vector_size (128)));
typedef double vnx16df __attribute__((vector_size (128)));

#define VEC_SET(S,V,IDX)			\
  V						\
  __attribute__((noipa))			\
  vec_set_##V##_##IDX (V v, S s)		\
  {						\
    v[IDX] = s;					\
    return v;					\
  }

#define VEC_SET_VAR4(S,V)			\
  V						\
  __attribute__((noipa))			\
  vec_set_var_##V (V v, int64_t idx, S s)	\
  {						\
    v[idx] = s;					\
    return v;					\
  }						\

#define TEST_ALL4(T)				\
  T (_Float16, vnx64hf, 0)			\
  T (_Float16, vnx64hf, 3)			\
  T (_Float16, vnx64hf, 7)			\
  T (_Float16, vnx64hf, 8)			\
  T (_Float16, vnx64hf, 16)			\
  T (_Float16, vnx64hf, 31)			\
  T (_Float16, vnx64hf, 42)			\
  T (_Float16, vnx64hf, 63)			\
  T (float, vnx32sf, 0)				\
  T (float, vnx32sf, 3)				\
  T (float, vnx32sf, 12)			\
  T (float, vnx32sf, 17)			\
  T (float, vnx32sf, 14)			\
  T (double, vnx16df, 0)			\
  T (double, vnx16df, 4)			\
  T (double, vnx16df, 8)			\
  T (double, vnx16df, 12)			\
  T (int64_t, vnx16di, 0)			\
  T (int64_t, vnx16di, 4)			\
  T (int64_t, vnx16di, 8)			\
  T (int64_t, vnx16di, 12)			\
  T (int32_t, vnx32si, 0)			\
  T (int32_t, vnx32si, 4)			\
  T (int32_t, vnx32si, 12)			\
  T (int32_t, vnx32si, 16)			\
  T (int32_t, vnx32si, 28)			\
  T (int16_t, vnx64hi, 0)			\
  T (int16_t, vnx64hi, 4)			\
  T (int16_t, vnx64hi, 28)			\
  T (int16_t, vnx64hi, 32)			\
  T (int16_t, vnx64hi, 60)			\
  T (int8_t, vnx128qi, 0)			\
  T (int8_t, vnx128qi, 4)			\
  T (int8_t, vnx128qi, 30)			\
  T (int8_t, vnx128qi, 60)			\
  T (int8_t, vnx128qi, 64)			\
  T (int8_t, vnx128qi, 127)			\

#define TEST_ALL_VAR4(T)			\
  T (_Float16, vnx64hf)				\
  T (float, vnx32sf)				\
  T (double, vnx16df)				\
  T (int64_t, vnx16di)				\
  T (int32_t, vnx32si)				\
  T (int16_t, vnx64hi)				\
  T (int8_t, vnx128qi)				\

TEST_ALL4 (VEC_SET)
TEST_ALL_VAR4 (VEC_SET_VAR4)

/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e8,\s*m8,\s*ta,\s*ma} 1 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e8,\s*m8,\s*tu,\s*ma} 6 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e16,\s*m8,\s*ta,\s*ma} 2 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e16,\s*m8,\s*tu,\s*ma} 13 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e32,\s*m8,\s*ta,\s*ma} 2 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e32,\s*m8,\s*tu,\s*ma} 10 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e64,\s*m8,\s*ta,\s*ma} 2 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e64,\s*m8,\s*tu,\s*ma} 8 } } */

/* { dg-final { scan-assembler-times {\tvmv.v.x} 20 } } */
/* { dg-final { scan-assembler-times {\tvfmv.v.f} 17 } } */
/* { dg-final { scan-assembler-times {\tvslideup.vi} 23 } } */
/* { dg-final { scan-assembler-times {\tvslideup.vx} 14 } } */

/* { dg-final { scan-assembler-times {\tvfmv.s.f} 3 } } */
/* { dg-final { scan-assembler-times {\tvmv.s.x} 4 } } */

#define CHECK(S, V, IDX)				\
void check_##V##_##IDX ()				\
  {							\
    V v;						\
    for (int i = 0; i < sizeof (V) / sizeof (S); i++)	\
      v[i] = i;						\
    V res = vec_set_##V##_##IDX (v, 77);		\
    for (int i = 0; i < sizeof (V) / sizeof (S); i++)	\
      assert (res[i] == (i == IDX ? 77 : i));		\
  }

#define CHECK_VAR(S, V)					\
void check_var_##V (int32_t idx)			\
  {							\
    V v;						\
    for (int i = 0; i < sizeof (V) / sizeof (S); i++)	\
      v[i] = i;						\
    V res = vec_set_var_##V (v, idx, 77);		\
    for (int i = 0; i < sizeof (V) / sizeof (S); i++)	\
      assert (res[i] == (i == idx ? 77 : i));		\
  }

#define RUN(S, V, IDX)					\
  check_##V##_##IDX ();

#define RUN_VAR(S, V)					\
  for (int i = 0; i < sizeof (V) / sizeof (S); i++)	\
    check_var_##V (i);					\

#define RUN_ALL(T)					\
  T (_Float16, vnx8hf, 0)				\
  T (_Float16, vnx8hf, 3)				\
  T (_Float16, vnx8hf, 7)				\
  T (_Float16, vnx16hf, 0)				\
  T (_Float16, vnx16hf, 3)				\
  T (_Float16, vnx16hf, 7)				\
  T (_Float16, vnx16hf, 8)				\
  T (_Float16, vnx16hf, 15)				\
  T (_Float16, vnx32hf, 0)				\
  T (_Float16, vnx32hf, 3)				\
  T (_Float16, vnx32hf, 7)				\
  T (_Float16, vnx32hf, 8)				\
  T (_Float16, vnx32hf, 16)				\
  T (_Float16, vnx32hf, 31)				\
  T (_Float16, vnx64hf, 0)				\
  T (_Float16, vnx64hf, 3)				\
  T (_Float16, vnx64hf, 7)				\
  T (_Float16, vnx64hf, 8)				\
  T (_Float16, vnx64hf, 16)				\
  T (_Float16, vnx64hf, 31)				\
  T (_Float16, vnx64hf, 42)				\
  T (_Float16, vnx64hf, 63)				\

#define RUN_ALL_VAR(T)					\
  T (_Float16, vnx8hf)					\
  T (_Float16, vnx16hf)					\
  T (_Float16, vnx32hf)					\
  T (_Float16, vnx64hf)					\

RUN_ALL (CHECK)
RUN_ALL_VAR (CHECK_VAR)

int main ()
{
  RUN_ALL (RUN);
  RUN_ALL_VAR (RUN_VAR);
}
