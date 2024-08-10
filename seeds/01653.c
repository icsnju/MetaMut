/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "-std=c99 -Wno-pedantic -Wno-psabi" } */

#include <assert.h>
#include <limits.h>

/* { dg-do compile } */
/* { dg-additional-options "-march=rv64gcv -mabi=lp64d -Wno-pedantic -Wno-psabi" } */

#include <stdint-gcc.h>

typedef uint64_t vnx2di __attribute__((vector_size (16)));
typedef uint32_t vnx4si __attribute__((vector_size (16)));
typedef uint16_t vnx8hi __attribute__((vector_size (16)));
typedef uint8_t vnx16qi __attribute__((vector_size (16)));

#define VEC_EXTRACT(S,V,IDX)			\
  S						\
  __attribute__((noipa))			\
  vec_extract_##V##_##IDX (V v)			\
  {						\
    return v[IDX];				\
  }

#define VEC_EXTRACT_VAR1(S,V)			\
  S						\
  __attribute__((noipa))			\
  vec_extract_var_##V (V v, int8_t idx)		\
  {						\
    return v[idx];				\
  }

#define TEST_ALL1(T)				\
  T (uint64_t, vnx2di, 0)			\
  T (uint64_t, vnx2di, 1)			\
  T (uint32_t, vnx4si, 0)			\
  T (uint32_t, vnx4si, 1)			\
  T (uint32_t, vnx4si, 3)			\
  T (uint16_t, vnx8hi, 0)			\
  T (uint16_t, vnx8hi, 2)			\
  T (uint16_t, vnx8hi, 6)			\
  T (uint8_t, vnx16qi, 0)			\
  T (uint8_t, vnx16qi, 1)			\
  T (uint8_t, vnx16qi, 7)			\
  T (uint8_t, vnx16qi, 11)			\
  T (uint8_t, vnx16qi, 15)			\

#define TEST_ALL_VAR1(T)			\
  T (uint64_t, vnx2di)				\
  T (uint32_t, vnx4si)				\
  T (uint16_t, vnx8hi)				\
  T (uint8_t, vnx16qi)				\

TEST_ALL1 (VEC_EXTRACT)
TEST_ALL_VAR1 (VEC_EXTRACT_VAR1)

/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e8,\s*m1,\s*ta,\s*ma} 6 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e16,\s*m1,\s*ta,\s*ma} 4 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e32,\s*m1,\s*ta,\s*ma} 4 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e64,\s*m1,\s*ta,\s*ma} 3 } } */

/* { dg-final { scan-assembler-times {\tvslidedown.vi} 9 } } */
/* { dg-final { scan-assembler-times {\tvslidedown.vx} 4 } } */

/* { dg-final { scan-assembler-times {\tvmv.x.s} 17 } } */

/* { dg-final { scan-assembler-times {\tandi\ta0,a0,0xff} 6 } } */
/* { dg-final { scan-assembler-times {\tslli\ta0,a0,48} 4 } } */
/* { dg-final { scan-assembler-times {\tsrli\ta0,a0,48} 4 } } */
/* { dg-do compile } */
/* { dg-additional-options "-march=rv64gcv -mabi=lp64d -Wno-pedantic -Wno-psabi" } */

#include <stdint-gcc.h>

typedef uint64_t vnx4di __attribute__((vector_size (32)));
typedef uint32_t vnx8si __attribute__((vector_size (32)));
typedef uint16_t vnx16hi __attribute__((vector_size (32)));
typedef uint8_t vnx32qi __attribute__((vector_size (32)));

#define VEC_EXTRACT(S,V,IDX)			\
  S						\
  __attribute__((noipa))			\
  vec_extract_##V##_##IDX (V v)			\
  {						\
    return v[IDX];				\
  }

#define VEC_EXTRACT_VAR2(S,V)			\
  S						\
  __attribute__((noipa))			\
  vec_extract_var_##V (V v, int16_t idx)	\
  {						\
    return v[idx];				\
  }

#define TEST_ALL2(T)				\
  T (uint64_t, vnx4di, 0)			\
  T (uint64_t, vnx4di, 1)			\
  T (uint64_t, vnx4di, 2)			\
  T (uint64_t, vnx4di, 3)			\
  T (uint32_t, vnx8si, 0)			\
  T (uint32_t, vnx8si, 1)			\
  T (uint32_t, vnx8si, 3)			\
  T (uint32_t, vnx8si, 4)			\
  T (uint32_t, vnx8si, 7)			\
  T (uint16_t, vnx16hi, 0)			\
  T (uint16_t, vnx16hi, 1)			\
  T (uint16_t, vnx16hi, 7)			\
  T (uint16_t, vnx16hi, 8)			\
  T (uint16_t, vnx16hi, 15)			\
  T (uint8_t, vnx32qi, 0)			\
  T (uint8_t, vnx32qi, 1)			\
  T (uint8_t, vnx32qi, 15)			\
  T (uint8_t, vnx32qi, 16)			\
  T (uint8_t, vnx32qi, 31)			\

#define TEST_ALL_VAR2(T)			\
  T (uint64_t, vnx4di)				\
  T (uint32_t, vnx8si)				\
  T (uint16_t, vnx16hi)				\
  T (uint8_t, vnx32qi)				\

TEST_ALL2 (VEC_EXTRACT)
TEST_ALL_VAR2 (VEC_EXTRACT_VAR2)

/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e8,\s*m2,\s*ta,\s*ma} 6 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e16,\s*m2,\s*ta,\s*ma} 6 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e32,\s*m2,\s*ta,\s*ma} 6 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e64,\s*m2,\s*ta,\s*ma} 5 } } */

/* { dg-final { scan-assembler-times {\tvslidedown.vi} 15 } } */
/* { dg-final { scan-assembler-times {\tvslidedown.vx} 4 } } */

/* { dg-final { scan-assembler-times {\tvmv.x.s} 23 } } */

/* { dg-final { scan-assembler-times {\tandi\ta0,a0,0xff} 6 } } */
/* { dg-final { scan-assembler-times {\tslli\ta0,a0,48} 6 } } */
/* { dg-final { scan-assembler-times {\tsrli\ta0,a0,48} 6 } } */
/* { dg-do compile } */
/* { dg-additional-options "-march=rv64gcv -mabi=lp64d -Wno-pedantic -Wno-psabi" } */

#include <stdint-gcc.h>

typedef uint64_t vnx8di __attribute__((vector_size (64)));
typedef uint32_t vnx16si __attribute__((vector_size (64)));
typedef uint16_t vnx32hi __attribute__((vector_size (64)));
typedef uint8_t vnx64qi __attribute__((vector_size (64)));

#define VEC_EXTRACT(S,V,IDX)			\
  S						\
  __attribute__((noipa))			\
  vec_extract_##V##_##IDX (V v)			\
  {						\
    return v[IDX];				\
  }

#define VEC_EXTRACT_VAR3(S,V)			\
  S						\
  __attribute__((noipa))			\
  vec_extract_var_##V (V v, int32_t idx)	\
  {						\
    return v[idx];				\
  }

#define TEST_ALL3(T)				\
  T (uint64_t, vnx8di, 0)			\
  T (uint64_t, vnx8di, 2)			\
  T (uint64_t, vnx8di, 4)			\
  T (uint64_t, vnx8di, 6)			\
  T (uint32_t, vnx16si, 0)			\
  T (uint32_t, vnx16si, 2)			\
  T (uint32_t, vnx16si, 6)			\
  T (uint32_t, vnx16si, 8)			\
  T (uint32_t, vnx16si, 14)			\
  T (uint16_t, vnx32hi, 0)			\
  T (uint16_t, vnx32hi, 2)			\
  T (uint16_t, vnx32hi, 14)			\
  T (uint16_t, vnx32hi, 16)			\
  T (uint16_t, vnx32hi, 30)			\
  T (uint8_t, vnx64qi, 0)			\
  T (uint8_t, vnx64qi, 2)			\
  T (uint8_t, vnx64qi, 30)			\
  T (uint8_t, vnx64qi, 32)			\
  T (uint8_t, vnx64qi, 63)			\

#define TEST_ALL_VAR3(T)			\
  T (uint64_t, vnx8di)				\
  T (uint32_t, vnx16si)				\
  T (uint16_t, vnx32hi)				\
  T (uint8_t, vnx64qi)				\

TEST_ALL3 (VEC_EXTRACT)
TEST_ALL_VAR3 (VEC_EXTRACT_VAR3)

/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e8,\s*m4,\s*ta,\s*ma} 6 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e16,\s*m4,\s*ta,\s*ma} 6 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e32,\s*m4,\s*ta,\s*ma} 6 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e64,\s*m4,\s*ta,\s*ma} 5 } } */

/* { dg-final { scan-assembler-times {\tvslidedown.vi} 13 } } */
/* { dg-final { scan-assembler-times {\tvslidedown.vx} 6 } } */

/* { dg-final { scan-assembler-times {\tvmv.x.s} 23 } } */

/* { dg-final { scan-assembler-times {\tandi\ta0,a0,0xff} 6 } } */
/* { dg-final { scan-assembler-times {\tslli\ta0,a0,48} 6 } } */
/* { dg-final { scan-assembler-times {\tsrli\ta0,a0,48} 6 } } */
/* { dg-do compile } */
/* { dg-additional-options "-march=rv64gcv -mabi=lp64d -Wno-pedantic -Wno-psabi" } */

#include <stdint-gcc.h>

typedef uint64_t vnx16di __attribute__((vector_size (128)));
typedef uint32_t vnx32si __attribute__((vector_size (128)));
typedef uint16_t vnx64hi __attribute__((vector_size (128)));
typedef uint8_t vnx128qi __attribute__((vector_size (128)));

#define VEC_EXTRACT(S,V,IDX)			\
  S						\
  __attribute__((noipa))			\
  vec_extract_##V##_##IDX (V v)			\
  {						\
    return v[IDX];				\
  }

#define VEC_EXTRACT_VAR4(S,V)			\
  S						\
  __attribute__((noipa))			\
  vec_extract_var_##V (V v, int64_t idx)	\
  {						\
    return v[idx];				\
  }

#define TEST_ALL4(T)				\
  T (uint64_t, vnx16di, 0)			\
  T (uint64_t, vnx16di, 4)			\
  T (uint64_t, vnx16di, 8)			\
  T (uint64_t, vnx16di, 12)			\
  T (uint32_t, vnx32si, 0)			\
  T (uint32_t, vnx32si, 4)			\
  T (uint32_t, vnx32si, 12)			\
  T (uint32_t, vnx32si, 16)			\
  T (uint32_t, vnx32si, 28)			\
  T (uint16_t, vnx64hi, 0)			\
  T (uint16_t, vnx64hi, 4)			\
  T (uint16_t, vnx64hi, 28)			\
  T (uint16_t, vnx64hi, 32)			\
  T (uint16_t, vnx64hi, 60)			\
  T (uint8_t, vnx128qi, 0)			\
  T (uint8_t, vnx128qi, 4)			\
  T (uint8_t, vnx128qi, 30)			\
  T (uint8_t, vnx128qi, 60)			\
  T (uint8_t, vnx128qi, 64)			\
  T (uint8_t, vnx128qi, 127)			\

#define TEST_ALL_VAR4(T)			\
  T (uint64_t, vnx16di)				\
  T (uint32_t, vnx32si)				\
  T (uint16_t, vnx64hi)				\
  T (uint8_t, vnx128qi)				\

TEST_ALL4 (VEC_EXTRACT)
TEST_ALL_VAR4 (VEC_EXTRACT_VAR4)

/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e8,\s*m8,\s*ta,\s*ma} 7 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e16,\s*m8,\s*ta,\s*ma} 6 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e32,\s*m8,\s*ta,\s*ma} 6 } } */
/* { dg-final { scan-assembler-times {vset[i]*vli\s+[a-z0-9,]+,\s*e64,\s*m8,\s*ta,\s*ma} 5 } } */

/* { dg-final { scan-assembler-times {\tvslidedown.vi} 11 } } */
/* { dg-final { scan-assembler-times {\tvslidedown.vx} 9 } } */

/* { dg-final { scan-assembler-times {\tvmv.x.s} 24 } } */

/* { dg-final { scan-assembler-times {\tandi\ta0,a0,0xff} 7 } } */
/* { dg-final { scan-assembler-times {\tslli\ta0,a0,48} 6 } } */
/* { dg-final { scan-assembler-times {\tsrli\ta0,a0,48} 6 } } */

#define CHECK(S, V, IDX)                                                       \
  __attribute__ ((noipa, optimize ("0"))) void check_##V##_##IDX ()            \
  {                                                                            \
    V v;                                                                       \
    for (int i = 0; i < sizeof (V) / sizeof (S); i++)                          \
      v[i] = (S) (INT_MAX - i);                                                \
    S res = vec_extract_##V##_##IDX (v);                                       \
    assert (res == (S) (INT_MAX - IDX));                                       \
  }

#define CHECK_VAR(S, V)                                                        \
  __attribute__ ((noipa, optimize ("0"))) void check_var_##V (int32_t idx)     \
  {                                                                            \
    V v;                                                                       \
    for (int i = 0; i < sizeof (V) / sizeof (S); i++)                          \
      v[i] = (S) (INT_MAX - i);                                                \
    S res = vec_extract_var_##V (v, idx);                                      \
    assert (res == (S) (INT_MAX - idx));                                       \
  }

#define RUN(S, V, IDX) check_##V##_##IDX ();

#define RUN_VAR(S, V)                                                          \
  for (int i = 0; i < sizeof (V) / sizeof (S); i++)                            \
    check_var_##V (i);

#define RUN_ALL(T)                                                             \
  T (uint64_t, vnx2di, 0)                                                      \
  T (uint64_t, vnx2di, 1)                                                      \
  T (uint32_t, vnx4si, 0)                                                      \
  T (uint32_t, vnx4si, 1)                                                      \
  T (uint32_t, vnx4si, 3)                                                      \
  T (uint16_t, vnx8hi, 0)                                                      \
  T (uint16_t, vnx8hi, 2)                                                      \
  T (uint16_t, vnx8hi, 6)                                                      \
  T (uint8_t, vnx16qi, 0)                                                      \
  T (uint8_t, vnx16qi, 1)                                                      \
  T (uint8_t, vnx16qi, 7)                                                      \
  T (uint8_t, vnx16qi, 11)                                                     \
  T (uint8_t, vnx16qi, 15)                                                     \
  T (uint64_t, vnx4di, 0)                                                      \
  T (uint64_t, vnx4di, 1)                                                      \
  T (uint64_t, vnx4di, 2)                                                      \
  T (uint64_t, vnx4di, 3)                                                      \
  T (uint32_t, vnx8si, 0)                                                      \
  T (uint32_t, vnx8si, 1)                                                      \
  T (uint32_t, vnx8si, 3)                                                      \
  T (uint32_t, vnx8si, 4)                                                      \
  T (uint32_t, vnx8si, 7)                                                      \
  T (uint16_t, vnx16hi, 0)                                                     \
  T (uint16_t, vnx16hi, 1)                                                     \
  T (uint16_t, vnx16hi, 7)                                                     \
  T (uint16_t, vnx16hi, 8)                                                     \
  T (uint16_t, vnx16hi, 15)                                                    \
  T (uint8_t, vnx32qi, 0)                                                      \
  T (uint8_t, vnx32qi, 1)                                                      \
  T (uint8_t, vnx32qi, 15)                                                     \
  T (uint8_t, vnx32qi, 16)                                                     \
  T (uint8_t, vnx32qi, 31)                                                     \
  T (uint64_t, vnx8di, 0)                                                      \
  T (uint64_t, vnx8di, 2)                                                      \
  T (uint64_t, vnx8di, 4)                                                      \
  T (uint64_t, vnx8di, 6)                                                      \
  T (uint32_t, vnx16si, 0)                                                     \
  T (uint32_t, vnx16si, 2)                                                     \
  T (uint32_t, vnx16si, 6)                                                     \
  T (uint32_t, vnx16si, 8)                                                     \
  T (uint32_t, vnx16si, 14)                                                    \
  T (uint16_t, vnx32hi, 0)                                                     \
  T (uint16_t, vnx32hi, 2)                                                     \
  T (uint16_t, vnx32hi, 14)                                                    \
  T (uint16_t, vnx32hi, 16)                                                    \
  T (uint16_t, vnx32hi, 30)                                                    \
  T (uint8_t, vnx64qi, 0)                                                      \
  T (uint8_t, vnx64qi, 2)                                                      \
  T (uint8_t, vnx64qi, 30)                                                     \
  T (uint8_t, vnx64qi, 32)                                                     \
  T (uint8_t, vnx64qi, 63)                                                     \
  T (uint64_t, vnx16di, 0)                                                     \
  T (uint64_t, vnx16di, 4)                                                     \
  T (uint64_t, vnx16di, 8)                                                     \
  T (uint64_t, vnx16di, 12)                                                    \
  T (uint32_t, vnx32si, 0)                                                     \
  T (uint32_t, vnx32si, 4)                                                     \
  T (uint32_t, vnx32si, 12)                                                    \
  T (uint32_t, vnx32si, 16)                                                    \
  T (uint32_t, vnx32si, 28)                                                    \
  T (uint16_t, vnx64hi, 0)                                                     \
  T (uint16_t, vnx64hi, 4)                                                     \
  T (uint16_t, vnx64hi, 28)                                                    \
  T (uint16_t, vnx64hi, 32)                                                    \
  T (uint16_t, vnx64hi, 60)                                                    \
  T (uint8_t, vnx128qi, 0)                                                     \
  T (uint8_t, vnx128qi, 4)                                                     \
  T (uint8_t, vnx128qi, 30)                                                    \
  T (uint8_t, vnx128qi, 60)                                                    \
  T (uint8_t, vnx128qi, 64)                                                    \
  T (uint8_t, vnx128qi, 127)

#define RUN_ALL_VAR(T)                                                         \
  T (uint64_t, vnx2di)                                                         \
  T (uint32_t, vnx4si)                                                         \
  T (uint16_t, vnx8hi)                                                         \
  T (uint8_t, vnx16qi)                                                         \
  T (uint64_t, vnx4di)                                                         \
  T (uint32_t, vnx8si)                                                         \
  T (uint16_t, vnx16hi)                                                        \
  T (uint8_t, vnx32qi)                                                         \
  T (uint64_t, vnx8di)                                                         \
  T (uint32_t, vnx16si)                                                        \
  T (uint16_t, vnx32hi)                                                        \
  T (uint8_t, vnx64qi)                                                         \
  T (uint64_t, vnx16di)                                                        \
  T (uint32_t, vnx32si)                                                        \
  T (uint16_t, vnx64hi)                                                        \
  T (uint8_t, vnx128qi)

RUN_ALL (CHECK)
RUN_ALL_VAR (CHECK_VAR)

int
main ()
{
  RUN_ALL (RUN);
  RUN_ALL_VAR (RUN_VAR);
}
