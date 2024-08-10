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

#define PERMUTE(TYPE, TYPE2, NUNITS)                                           \
  __attribute__ ((noipa)) void permute_##TYPE (TYPE values1, TYPE values2,     \
					       TYPE2 mask, TYPE *out)          \
  {                                                                            \
    TYPE v = __builtin_shuffle (values1, values1, mask);                       \
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

/* { dg-final { scan-assembler-times {vrgather\.vv\tv[0-9]+,\s*v[0-9]+,\s*v[0-9]+} 31 } } */

#define MASK_2(X, Y) (Y) - 1 - (X) + (Y), (Y) -2 - (X) + (Y)
#define MASK_4(X, Y) MASK_2 (X, Y), MASK_2 (X + 2, Y)
#define MASK_8(X, Y) MASK_4 (X, Y), MASK_4 (X + 4, Y)
#define MASK_16(X, Y) MASK_8 (X, Y), MASK_8 (X + 8, Y)
#define MASK_32(X, Y) MASK_16 (X, Y), MASK_16 (X + 16, Y)
#define MASK_64(X, Y) MASK_32 (X, Y), MASK_32 (X + 32, Y)
#define MASK_128(X, Y) MASK_64 (X, Y), MASK_64 (X + 64, Y)

#define INIT_MASK(TYPE, NUNTIS) \
  TYPE TYPE##_mask = {MASK_##NUNTIS (0, NUNTIS)};

int __attribute__ ((optimize (0)))
main ()
{
  INIT_PERMUTE(2, 3, 79, vnx2qi)
  INIT_MASK (vnx2qi, 2)
  permute_vnx2qi (v_vnx2qi_in1, v_vnx2qi_in2, vnx2qi_mask, &v_vnx2qi_out);
  CHECK_PERMUTE_REVERSE(2, vnx2qi)
  INIT_PERMUTE(4, 2, -69, vnx4qi)
  INIT_MASK (vnx4qi, 4)
  permute_vnx4qi (v_vnx4qi_in1, v_vnx4qi_in2, vnx4qi_mask, &v_vnx4qi_out);
  CHECK_PERMUTE_REVERSE(4, vnx4qi)
  INIT_PERMUTE(8, 4, -33, vnx8qi)
  INIT_MASK (vnx8qi, 8)
  permute_vnx8qi (v_vnx8qi_in1, v_vnx8qi_in2, vnx8qi_mask, &v_vnx8qi_out);
  CHECK_PERMUTE_REVERSE(8, vnx8qi)
  INIT_PERMUTE(16, -3, 15, vnx16qi)
  INIT_MASK (vnx16qi, 16)
  permute_vnx16qi (v_vnx16qi_in1, v_vnx16qi_in2, vnx16qi_mask, &v_vnx16qi_out);
  CHECK_PERMUTE_REVERSE(16, vnx16qi)
  INIT_PERMUTE(32, -1, 30, vnx32qi)
  INIT_MASK (vnx32qi, 32)
  permute_vnx32qi (v_vnx32qi_in1, v_vnx32qi_in2, vnx32qi_mask, &v_vnx32qi_out);
  CHECK_PERMUTE_REVERSE(32, vnx32qi)
  INIT_PERMUTE(64, -1, 66, vnx64qi)
  INIT_MASK (vnx64qi, 64)
  permute_vnx64qi (v_vnx64qi_in1, v_vnx64qi_in2, vnx64qi_mask, &v_vnx64qi_out);
  CHECK_PERMUTE_REVERSE(64, vnx64qi)
  INIT_PERMUTE(128, -1, 38, vnx128qi)
  INIT_MASK (vnx128qi, 128)
  permute_vnx128qi (v_vnx128qi_in1, v_vnx128qi_in2, vnx128qi_mask, &v_vnx128qi_out);
  CHECK_PERMUTE_REVERSE(128, vnx128qi)
  INIT_PERMUTE(2, 2, 30238, vnx2hi)
  INIT_MASK (vnx2hi, 2)
  permute_vnx2hi (v_vnx2hi_in1, v_vnx2hi_in2, vnx2hi_mask, &v_vnx2hi_out);
  CHECK_PERMUTE_REVERSE(2, vnx2hi)
  INIT_PERMUTE(4, -45, -2345, vnx4hi)
  INIT_MASK (vnx4hi, 4)
  permute_vnx4hi (v_vnx4hi_in1, v_vnx4hi_in2, vnx4hi_mask, &v_vnx4hi_out);
  CHECK_PERMUTE_REVERSE(4, vnx4hi)
  INIT_PERMUTE(8, 98, -18415, vnx8hi)
  INIT_MASK (vnx8hi, 8)
  permute_vnx8hi (v_vnx8hi_in1, v_vnx8hi_in2, vnx8hi_mask, &v_vnx8hi_out);
  CHECK_PERMUTE_REVERSE(8, vnx8hi)
  INIT_PERMUTE(16, 56, 3299, vnx16hi)
  INIT_MASK (vnx16hi, 16)
  permute_vnx16hi (v_vnx16hi_in1, v_vnx16hi_in2, vnx16hi_mask, &v_vnx16hi_out);
  CHECK_PERMUTE_REVERSE(16, vnx16hi)
  INIT_PERMUTE(32, 15641, -9156, vnx32hi)
  INIT_MASK (vnx32hi, 32)
  permute_vnx32hi (v_vnx32hi_in1, v_vnx32hi_in2, vnx32hi_mask, &v_vnx32hi_out);
  CHECK_PERMUTE_REVERSE(32, vnx32hi)
  INIT_PERMUTE(64, -25641, 8093, vnx64hi)
  INIT_MASK (vnx64hi, 64)
  permute_vnx64hi (v_vnx64hi_in1, v_vnx64hi_in2, vnx64hi_mask, &v_vnx64hi_out);
  CHECK_PERMUTE_REVERSE(64, vnx64hi)
  INIT_PERMUTE(2, -428, -15651, vnx2si)
  INIT_MASK (vnx2si, 2)
  permute_vnx2si (v_vnx2si_in1, v_vnx2si_in2, vnx2si_mask, &v_vnx2si_out);
  CHECK_PERMUTE_REVERSE(2, vnx2si)
  INIT_PERMUTE(4, 208, -55651, vnx4si)
  INIT_MASK (vnx4si, 4)
  permute_vnx4si (v_vnx4si_in1, v_vnx4si_in2, vnx4si_mask, &v_vnx4si_out);
  CHECK_PERMUTE_REVERSE(4, vnx4si)
  INIT_PERMUTE(8, 808, 75651, vnx8si)
  INIT_MASK (vnx8si, 8)
  permute_vnx8si (v_vnx8si_in1, v_vnx8si_in2, vnx8si_mask, &v_vnx8si_out);
  CHECK_PERMUTE_REVERSE(8, vnx8si)
  INIT_PERMUTE(16, 816, -8941561, vnx16si)
  INIT_MASK (vnx16si, 16)
  permute_vnx16si (v_vnx16si_in1, v_vnx16si_in2, vnx16si_mask, &v_vnx16si_out);
  CHECK_PERMUTE_REVERSE(16, vnx16si)
  INIT_PERMUTE(32, -532, 98416, vnx32si)
  INIT_MASK (vnx32si, 32)
  permute_vnx32si (v_vnx32si_in1, v_vnx32si_in2, vnx32si_mask, &v_vnx32si_out);
  CHECK_PERMUTE_REVERSE(32, vnx32si)
  INIT_PERMUTE(2, -4161, 9551616, vnx2di)
  INIT_MASK (vnx2di, 2)
  permute_vnx2di (v_vnx2di_in1, v_vnx2di_in2, vnx2di_mask, &v_vnx2di_out);
  CHECK_PERMUTE_REVERSE(2, vnx2di)
  INIT_PERMUTE(4, 7259, -15644961, vnx4di)
  INIT_MASK (vnx4di, 4)
  permute_vnx4di (v_vnx4di_in1, v_vnx4di_in2, vnx4di_mask, &v_vnx4di_out);
  CHECK_PERMUTE_REVERSE(4, vnx4di)
  INIT_PERMUTE(8, 351, 9156651, vnx8di)
  INIT_MASK (vnx8di, 8)
  permute_vnx8di (v_vnx8di_in1, v_vnx8di_in2, vnx8di_mask, &v_vnx8di_out);
  CHECK_PERMUTE_REVERSE(8, vnx8di)
  INIT_PERMUTE(16, 11, -816196231,vnx16di)
  INIT_MASK (vnx16di, 16)
  permute_vnx16di (v_vnx16di_in1, v_vnx16di_in2, vnx16di_mask, &v_vnx16di_out);
  CHECK_PERMUTE_REVERSE(16, vnx16di)
  INIT_PERMUTE(2, 4552, -89, vnx2sf)
  permute_vnx2sf (v_vnx2sf_in1, v_vnx2sf_in2, vnx2si_mask, &v_vnx2sf_out);
  CHECK_PERMUTE_REVERSE(2, vnx2sf)
  INIT_PERMUTE(4, 685, 7961, vnx4sf)
  permute_vnx4sf (v_vnx4sf_in1, v_vnx4sf_in2, vnx4si_mask, &v_vnx4sf_out);
  CHECK_PERMUTE_REVERSE(4, vnx4sf)
  INIT_PERMUTE(8, 3927, 16513, vnx8sf)
  permute_vnx8sf (v_vnx8sf_in1, v_vnx8sf_in2, vnx8si_mask, &v_vnx8sf_out);
  CHECK_PERMUTE_REVERSE(8, vnx8sf)
  INIT_PERMUTE(16, -68, 16156571, vnx16sf)
  permute_vnx16sf (v_vnx16sf_in1, v_vnx16sf_in2, vnx16si_mask, &v_vnx16sf_out);
  CHECK_PERMUTE_REVERSE(16, vnx16sf)
  INIT_PERMUTE(32, 9985, 1561318, vnx32sf)
  permute_vnx32sf (v_vnx32sf_in1, v_vnx32sf_in2, vnx32si_mask, &v_vnx32sf_out);
  CHECK_PERMUTE_REVERSE(32, vnx32sf)
  INIT_PERMUTE(2, -1565.1561, -5641565.515, vnx2df)
  permute_vnx2df (v_vnx2df_in1, v_vnx2df_in2, vnx2di_mask, &v_vnx2df_out);
  CHECK_PERMUTE_REVERSE(2, vnx2df)
  INIT_PERMUTE(4, -189.14897196, -15616547.5165574, vnx4df)
  permute_vnx4df (v_vnx4df_in1, v_vnx4df_in2, vnx4di_mask, &v_vnx4df_out);
  CHECK_PERMUTE_REVERSE(4, vnx4df)
  INIT_PERMUTE(8, 651.158691561, -56163.1655411, vnx8df)
  permute_vnx8df (v_vnx8df_in1, v_vnx8df_in2, vnx8di_mask, &v_vnx8df_out);
  CHECK_PERMUTE_REVERSE(8, vnx8df)
  INIT_PERMUTE(16, 58.91516377, 251465.81561, vnx16df)
  permute_vnx16df (v_vnx16df_in1, v_vnx16df_in2, vnx16di_mask, &v_vnx16df_out);
  CHECK_PERMUTE_REVERSE(16, vnx16df)

  return 0;
}
