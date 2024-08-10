/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O" } */
/* { dg-options "-O -msve-vector-bits=256" { target aarch64_sve256_hw } } */

/* { dg-do compile } */
/* { dg-options "-O -msve-vector-bits=256" } */

#include <stdint.h>

typedef int64_t vnx2di __attribute__((vector_size (32)));
typedef int32_t vnx4si __attribute__((vector_size (32)));
typedef int16_t vnx8hi __attribute__((vector_size (32)));
typedef int8_t vnx16qi __attribute__((vector_size (32)));
typedef double vnx2df __attribute__((vector_size (32)));
typedef float vnx4sf __attribute__((vector_size (32)));
typedef _Float16 vnx8hf __attribute__((vector_size (32)));

#define VEC_PERM(TYPE, MASKTYPE)					\
TYPE __attribute__ ((noinline, noclone)) 				\
vec_perm_##TYPE (TYPE values1, TYPE values2, MASKTYPE mask)		\
{									\
  return __builtin_shuffle (values1, values2, mask);			\
}

VEC_PERM (vnx2di, vnx2di);
VEC_PERM (vnx4si, vnx4si);
VEC_PERM (vnx8hi, vnx8hi);
VEC_PERM (vnx16qi, vnx16qi);
VEC_PERM (vnx2df, vnx2di);
VEC_PERM (vnx4sf, vnx4si);
VEC_PERM (vnx8hf, vnx8hi);

/* { dg-final { scan-assembler-times {\ttbl\tz[0-9]+\.d, z[0-9]+\.d, z[0-9]+\.d\n} 4 } } */
/* { dg-final { scan-assembler-times {\ttbl\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 4 } } */
/* { dg-final { scan-assembler-times {\ttbl\tz[0-9]+\.h, z[0-9]+\.h, z[0-9]+\.h\n} 4 } } */
/* { dg-final { scan-assembler-times {\ttbl\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b\n} 2 } } */

#define TEST_VEC_PERM(TYPE, MASK_TYPE, EXPECTED_RESULT,			\
		      VALUES1, VALUES2, MASK)				\
{									\
  TYPE expected_result = EXPECTED_RESULT;				\
  TYPE values1 = VALUES1;						\
  TYPE values2 = VALUES2;						\
  MASK_TYPE mask = MASK;						\
  TYPE dest;								\
  dest = vec_perm_##TYPE (values1, values2, mask);			\
  if (__builtin_memcmp (&dest, &expected_result, sizeof (TYPE)) != 0)	\
    __builtin_abort ();							\
}

int main (void)
{
  TEST_VEC_PERM (vnx2di, vnx2di,
		 ((vnx2di) { 5, 36, 7, 48 }),
		 ((vnx2di) { 4, 5, 6, 7 }),
		 ((vnx2di) { 12, 24, 36, 48 }),
		 ((vnx2di) { 1, 6, 3, 7 }));
  TEST_VEC_PERM (vnx4si, vnx4si,
		 ((vnx4si) { 34, 38, 40, 10, 9, 8, 7, 35 }),
		 ((vnx4si) { 3, 4, 5, 6, 7, 8, 9, 10 }),
		 ((vnx4si) { 33, 34, 35, 36, 37, 38, 39, 40 }),
		 ((vnx4si) { 9, 13, 15, 7, 6, 5, 4, 10 }));
  TEST_VEC_PERM (vnx8hi, vnx8hi,
		 ((vnx8hi) { 12, 16, 18, 10, 42, 43, 44, 34,
			     7, 48, 3, 35, 9, 8, 7, 13 }),
		 ((vnx8hi) { 3, 4, 5, 6, 7, 8, 9, 10,
			     11, 12, 13, 14, 15, 16, 17, 18 }),
		 ((vnx8hi) { 33, 34, 35, 36, 37, 38, 39, 40,
			     41, 42, 43, 44, 45, 46, 47, 48 }),
		 ((vnx8hi) { 9, 13, 15, 7, 25, 26, 27, 17,
			     4, 31, 0, 18, 6, 5, 4, 10 }));
  TEST_VEC_PERM (vnx16qi, vnx16qi,
		 ((vnx16qi) { 5, 6, 7, 4, 5, 6, 4, 5,
			      6, 7, 12, 24, 36, 48, 12, 24,
			      5, 6, 7, 4, 5, 6, 4, 5,
			      6, 7, 12, 24, 36, 48, 12, 24 }),
		 ((vnx16qi) { 4, 5, 6, 7, 4, 5, 6, 7,
			      4, 5, 6, 7, 4, 5, 6, 7,
			      4, 5, 6, 7, 4, 5, 6, 7,
			      4, 5, 6, 7, 4, 5, 6, 7 }),
		 ((vnx16qi) { 12, 24, 36, 48, 12, 24, 36, 48,
			      12, 24, 36, 48, 12, 24, 36, 48,
			      12, 24, 36, 48, 12, 24, 36, 48,
			      12, 24, 36, 48, 12, 24, 36, 48 }),
		 ((vnx16qi) { 5, 6, 7, 8, 9, 10, 28, 29,
			      30, 31, 32, 33, 54, 55, 56, 61,
			      5, 6, 7, 8, 9, 10, 28, 29,
			      30, 31, 32, 33, 54, 55, 56, 61 }));
  TEST_VEC_PERM (vnx2df, vnx2di,
		 ((vnx2df) { 5.1, 36.1, 7.1, 48.1 }),
		 ((vnx2df) { 4.1, 5.1, 6.1, 7.1 }),
		 ((vnx2df) { 12.1, 24.1, 36.1, 48.1 }),
		 ((vnx2di) { 1, 6, 3, 7 }));
  TEST_VEC_PERM (vnx4sf, vnx4si,
		 ((vnx4sf) { 34.2, 38.2, 40.2, 10.2, 9.2, 8.2, 7.2, 35.2 }),
		 ((vnx4sf) { 3.2, 4.2, 5.2, 6.2, 7.2, 8.2, 9.2, 10.2 }),
		 ((vnx4sf) { 33.2, 34.2, 35.2, 36.2,
			     37.2, 38.2, 39.2, 40.2 }),
		 ((vnx4si) { 9, 13, 15, 7, 6, 5, 4, 10 }));
  TEST_VEC_PERM (vnx8hf, vnx8hi,
		 ((vnx8hf) { 12.0, 16.0, 18.0, 10.0, 42.0, 43.0, 44.0, 34.0,
			     7.0, 48.0, 3.0, 35.0, 9.0, 8.0, 7.0, 13.0 }),
		 ((vnx8hf) { 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0,
			     11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0 }),
		 ((vnx8hf) { 33.0, 34.0, 35.0, 36.0, 37.0, 38.0, 39.0, 40.0,
			     41.0, 42.0, 43.0, 44.0, 45.0, 46.0, 47.0, 48.0 }),
		 ((vnx8hi) { 9, 13, 15, 7, 25, 26, 27, 17,
			     4, 31, 0, 18, 6, 5, 4, 10 }));
  return 0;
}
