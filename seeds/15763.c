/* { dg-do run { target { riscv_vector } } } */
/* { dg-options "--param riscv-autovec-preference=fixed-vlmax -O3 -Wno-psabi" } */

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

#define MASK_2(X, Y) 31, 31
#define MASK_4(X, Y) MASK_2 (X, Y), MASK_2 (X + 2, Y)
#define MASK_8(X, Y) MASK_4 (X, Y), MASK_4 (X + 4, Y)
#define MASK_16(X, Y) MASK_8 (X, Y), MASK_8 (X + 8, Y)
#define MASK_32(X, Y) MASK_16 (X, Y), MASK_16 (X + 16, Y)
#define MASK_64(X, Y) MASK_32 (X, Y), MASK_32 (X + 32, Y)
#define MASK_128(X, Y) MASK_64 (X, Y), MASK_64 (X + 64, Y)

#define PERMUTE(TYPE, NUNITS)                                                  \
  void permute_##TYPE (TYPE values1, TYPE values2, TYPE *out)                  \
  {                                                                            \
    TYPE v                                                                     \
      = __builtin_shufflevector (values1, values2, MASK_##NUNITS (0, NUNITS)); \
    *(TYPE *) out = v;                                                         \
  }

#define TEST_ALL(T)                                                            \
  T (vnx32qi, 32)                                                              \
  T (vnx64qi, 64)                                                              \
  T (vnx128qi, 128)                                                            \
  T (vnx32hi, 32)                                                              \
  T (vnx64hi, 64)                                                              \
  T (vnx32si, 32)                                                              \
  T (vnx32sf, 32)

TEST_ALL (PERMUTE)

/* { dg-final { scan-assembler-times {vrgather\.vi\tv[0-9]+,\s*v[0-9]+,\s*31} 7 } } */

int __attribute__ ((optimize (0)))
main ()
{
  INIT_PERMUTE(32, -1, 30, vnx32qi)
  permute_vnx32qi (v_vnx32qi_in1, v_vnx32qi_in2, &v_vnx32qi_out);
  CHECK_PERMUTE_SINGLE(32, -1*31+30, vnx32qi)
  INIT_PERMUTE(64, -1, 66, vnx64qi)
  permute_vnx64qi (v_vnx64qi_in1, v_vnx64qi_in2, &v_vnx64qi_out);
  CHECK_PERMUTE_SINGLE(64, -1*31+66, vnx64qi)
  INIT_PERMUTE(128, -1, 38, vnx128qi)
  permute_vnx128qi (v_vnx128qi_in1, v_vnx128qi_in2, &v_vnx128qi_out);
  CHECK_PERMUTE_SINGLE(128, -1*31+38, vnx128qi)
  INIT_PERMUTE(32, 156, -9156, vnx32hi)
  permute_vnx32hi (v_vnx32hi_in1, v_vnx32hi_in2, &v_vnx32hi_out);
  CHECK_PERMUTE_SINGLE(32, 156*31+-9156, vnx32hi)
  INIT_PERMUTE(64, -251, 8093, vnx64hi)
  permute_vnx64hi (v_vnx64hi_in1, v_vnx64hi_in2, &v_vnx64hi_out);
  CHECK_PERMUTE_SINGLE(64, -251*31+8093, vnx64hi)
  INIT_PERMUTE(32, -532, 98416, vnx32si)
  permute_vnx32si (v_vnx32si_in1, v_vnx32si_in2, &v_vnx32si_out);
  CHECK_PERMUTE_SINGLE(32, -532*31+98416, vnx32si)
  INIT_PERMUTE(32, 995, 1561318, vnx32sf)
  permute_vnx32sf (v_vnx32sf_in1, v_vnx32sf_in2, &v_vnx32sf_out);
  CHECK_PERMUTE_SINGLE(32, 995*31+1561318, vnx32sf)

  return 0;
}
