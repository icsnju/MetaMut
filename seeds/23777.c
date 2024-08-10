/* { dg-do compile } */
/* { dg-additional-options "-march=rv64gcv -mabi=lp64d -Wno-psabi" } */

#include <stdint.h>

typedef int8_t vnx2qi __attribute__ ((vector_size (2)));
typedef int8_t vnx4qi __attribute__ ((vector_size (4)));
typedef int8_t vnx8qi __attribute__ ((vector_size (8)));
typedef int8_t vnx16qi __attribute__ ((vector_size (16)));
typedef int8_t vnx32qi __attribute__ ((vector_size (32)));
typedef int8_t vnx64qi __attribute__ ((vector_size (64)));
typedef int8_t vnx128qi __attribute__ ((vector_size (128)));

typedef int16_t vnx2hi __attribute__ ((vector_size (4)));
typedef int16_t vnx4hi __attribute__ ((vector_size (8)));
typedef int16_t vnx8hi __attribute__ ((vector_size (16)));
typedef int16_t vnx16hi __attribute__ ((vector_size (32)));
typedef int16_t vnx32hi __attribute__ ((vector_size (64)));
typedef int16_t vnx64hi __attribute__ ((vector_size (128)));

typedef int32_t vnx2si __attribute__ ((vector_size (8)));
typedef int32_t vnx4si __attribute__ ((vector_size (16)));
typedef int32_t vnx8si __attribute__ ((vector_size (32)));
typedef int32_t vnx16si __attribute__ ((vector_size (64)));
typedef int32_t vnx32si __attribute__ ((vector_size (128)));

typedef int64_t vnx2di __attribute__ ((vector_size (16)));
typedef int64_t vnx4di __attribute__ ((vector_size (32)));
typedef int64_t vnx8di __attribute__ ((vector_size (64)));
typedef int64_t vnx16di __attribute__ ((vector_size (128)));

typedef float vnx2sf __attribute__ ((vector_size (8)));
typedef float vnx4sf __attribute__ ((vector_size (16)));
typedef float vnx8sf __attribute__ ((vector_size (32)));
typedef float vnx16sf __attribute__ ((vector_size (64)));
typedef float vnx32sf __attribute__ ((vector_size (128)));

typedef double vnx2df __attribute__ ((vector_size (16)));
typedef double vnx4df __attribute__ ((vector_size (32)));
typedef double vnx8df __attribute__ ((vector_size (64)));
typedef double vnx16df __attribute__ ((vector_size (128)));

#define INIT_PERMUTE(NUNITS, NUM1, NUM2, TYPE)                                 \
  TYPE v_##TYPE##_in1;                                                         \
  TYPE v_##TYPE##_in2;                                                         \
  TYPE v_##TYPE##_out = {0};                                                   \
  for (int i = 0; i < NUNITS; i++)                                             \
    {                                                                          \
      v_##TYPE##_in1[i] = i * NUM1 + NUM2;                                     \
      v_##TYPE##_in2[i] = i * NUM1 - NUM2;                                     \
    }

#define CHECK_PERMUTE_SINGLE(NUNITS, VALUE, TYPE)                              \
  for (int i = 0; i < NUNITS; i++)                                             \
    if (v_##TYPE##_out[i] != VALUE)                                            \
      __builtin_abort ();

#define CHECK_PERMUTE_REVERSE(NUNITS, TYPE)                                    \
  for (int i = 0; i < NUNITS; i++)                                             \
    if (v_##TYPE##_out[i] != v_##TYPE##_in1[NUNITS - 1 - i])                   \
      __builtin_abort ();

#define CHECK_PERMUTE_DOUBLE(NUNITS, TYPE)                                     \
  for (int i = 0; i < NUNITS; i++)                                             \
    {                                                                          \
      int new_index = i * 2;                                                   \
      if (new_index < NUNITS                                                   \
	  && v_##TYPE##_out[i] != v_##TYPE##_in1[new_index])                   \
	__builtin_abort ();                                                    \
      if (new_index >= NUNITS                                                  \
	  && v_##TYPE##_out[i] != v_##TYPE##_in2[new_index % NUNITS])          \
	__builtin_abort ();                                                    \
    }

#define PERMUTE(TYPE, TYPE2, NUNITS)                                           \
  __attribute__ ((noipa)) void permute_##TYPE (TYPE values1, TYPE values2,     \
					       TYPE2 mask, TYPE *out)          \
  {                                                                            \
    TYPE v = __builtin_shuffle (values1, values2, mask);                       \
    *(TYPE *) out = v;                                                         \
  }

#define TEST_ALL(T)                                                            \
  T (vnx2qi, vnx2qi, 2)                                                        \
  T (vnx4qi, vnx4qi, 4)                                                        \
  T (vnx8qi, vnx8qi, 8)                                                        \
  T (vnx16qi, vnx16qi, 16)                                                     \
  T (vnx32qi, vnx32qi, 32)                                                     \
  T (vnx64qi, vnx64qi, 64)                                                     \
  T (vnx128qi, vnx128qi, 128)                                                  \
  T (vnx2hi, vnx2hi, 2)                                                        \
  T (vnx4hi, vnx4hi, 4)                                                        \
  T (vnx8hi, vnx8hi, 8)                                                        \
  T (vnx16hi, vnx16hi, 16)                                                     \
  T (vnx32hi, vnx32hi, 32)                                                     \
  T (vnx64hi, vnx64hi, 64)                                                     \
  T (vnx2si, vnx2si, 2)                                                        \
  T (vnx4si, vnx4si, 4)                                                        \
  T (vnx8si, vnx8si, 8)                                                        \
  T (vnx16si, vnx16si, 16)                                                     \
  T (vnx32si, vnx32si, 32)                                                     \
  T (vnx2di, vnx2di, 2)                                                        \
  T (vnx4di, vnx4di, 4)                                                        \
  T (vnx8di, vnx8di, 8)                                                        \
  T (vnx16di, vnx16di, 16)                                                     \
  T (vnx2sf, vnx2si, 2)                                                        \
  T (vnx4sf, vnx4si, 4)                                                        \
  T (vnx8sf, vnx8si, 8)                                                        \
  T (vnx16sf, vnx16si, 16)                                                     \
  T (vnx32sf, vnx32si, 32)                                                     \
  T (vnx2df, vnx2di, 2)                                                        \
  T (vnx4df, vnx4di, 4)                                                        \
  T (vnx8df, vnx8di, 8)                                                        \
  T (vnx16df, vnx16di, 16)

TEST_ALL (PERMUTE)

/* { dg-final { scan-assembler-times {vrgather\.vv\tv[0-9]+,\s*v[0-9]+,\s*v[0-9]+,\s*v0.t} 31 } } */
