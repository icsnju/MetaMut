/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O" } */

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

#define UZP1(TYPE, MASK)				\
TYPE uzp1_##TYPE (TYPE values1, TYPE values2)		\
{							\
  return __builtin_shuffle (values1, values2, MASK);	\
}


UZP1 (vnx2di,  ((vnx2di) { 0, 2, 4, 6 }));
UZP1 (vnx4si,  ((vnx4si) { 0, 2, 4, 6, 8, 10, 12, 14 }));
UZP1 (vnx8hi,  ((vnx8hi) { 0, 2, 4, 6, 8, 10, 12, 14,
			   16, 18, 20, 22, 24, 26, 28, 30 }));
UZP1 (vnx16qi, ((vnx16qi) { 0, 2, 4, 6, 8, 10, 12, 14,
			    16, 18, 20, 22, 24, 26, 28, 30,
			    32, 34, 36, 38, 40, 42, 44, 46,
			    48, 50, 52, 54, 56, 58, 60, 62 }));
UZP1 (vnx2df,  ((vnx2di) { 0, 2, 4, 6 }));
UZP1 (vnx4sf,  ((vnx4si) { 0, 2, 4, 6, 8, 10, 12, 14 }));
UZP1 (vnx8hf,  ((vnx8hi) { 0, 2, 4, 6, 8, 10, 12, 14,
			   16, 18, 20, 22, 24, 26, 28, 30 }));

/* { dg-final { scan-assembler-not {\ttbl\tz[0-9]+\.d, z[0-9]+\.d, z[0-9]+\.d\n} } } */
/* { dg-final { scan-assembler-not {\ttbl\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} } } */
/* { dg-final { scan-assembler-not {\ttbl\tz[0-9]+\.h, z[0-9]+\.h, z[0-9]+\.h\n} } } */
/* { dg-final { scan-assembler-not {\ttbl\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b\n} } } */

/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.d, z[0-9]+\.d, z[0-9]+\.d\n} 2 } } */
/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 2 } } */
/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.h, z[0-9]+\.h, z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b\n} 1 } } */

#define TEST_UZP1(TYPE, EXPECTED_RESULT, VALUES1, VALUES2)		\
{									\
  TYPE expected_result = EXPECTED_RESULT;				\
  TYPE values1 = VALUES1;						\
  TYPE values2 = VALUES2;						\
  TYPE dest;								\
  dest = uzp1_##TYPE (values1, values2);				\
  if (__builtin_memcmp (&dest, &expected_result, sizeof (TYPE)) != 0)	\
    __builtin_abort ();							\
}

int main (void)
{
  TEST_UZP1 (vnx2di,
	     ((vnx2di) { 4, 6, 12, 36 }),
	     ((vnx2di) { 4, 5, 6, 7 }),
	     ((vnx2di) { 12, 24, 36, 48 }));
  TEST_UZP1 (vnx4si,
	     ((vnx4si) { 3, 5, 7, 9, 33, 35, 37, 39 }),
	     ((vnx4si) { 3, 4, 5, 6, 7, 8, 9, 10 }),
	     ((vnx4si) { 33, 34, 35, 36, 37, 38, 39, 40 }));
  TEST_UZP1 (vnx8hi,
	     ((vnx8hi) { 3, 5, 7, 9, 11, 13, 15, 17,
			 33, 35, 37, 39, 41, 43, 45, 47 }),
	     ((vnx8hi) { 3, 4, 5, 6, 7, 8, 9, 10,
			 11, 12, 13, 14, 15, 16, 17, 18 }),
	     ((vnx8hi) { 33, 34, 35, 36, 37, 38, 39, 40,
			 41, 42, 43, 44, 45, 46, 47, 48 }));
  TEST_UZP1 (vnx16qi,
	     ((vnx16qi) { 4, 6, 4, 6, 4, 6, 4, 6,
			  4, 6, 4, 6, 4, 6, 4, 6,
			  12, 36, 12, 36, 12, 36, 12, 36,
			  12, 36, 12, 36, 12, 36, 12, 36 }),
	     ((vnx16qi) { 4, 5, 6, 7, 4, 5, 6, 7,
			  4, 5, 6, 7, 4, 5, 6, 7,
			  4, 5, 6, 7, 4, 5, 6, 7,
			  4, 5, 6, 7, 4, 5, 6, 7 }),
	     ((vnx16qi) { 12, 24, 36, 48, 12, 24, 36, 48,
			  12, 24, 36, 48, 12, 24, 36, 48,
			  12, 24, 36, 48, 12, 24, 36, 48,
			  12, 24, 36, 48, 12, 24, 36, 48 }));
  TEST_UZP1 (vnx2df,
	     ((vnx2df) { 4.0, 6.0, 12.0, 36.0 }),
	     ((vnx2df) { 4.0, 5.0, 6.0, 7.0 }),
	     ((vnx2df) { 12.0, 24.0, 36.0, 48.0 }));
  TEST_UZP1 (vnx4sf,
	     ((vnx4sf) { 3.0, 5.0, 7.0, 9.0, 33.0, 35.0, 37.0, 39.0 }),
	     ((vnx4sf) { 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0 }),
	     ((vnx4sf) { 33.0, 34.0, 35.0, 36.0, 37.0, 38.0, 39.0, 40.0 }));
  TEST_UZP1 (vnx8hf,
	     ((vnx8hf) { 3.0, 5.0, 7.0, 9.0, 11.0, 13.0, 15.0, 17.0,
			 33.0, 35.0, 37.0, 39.0, 41.0, 43.0, 45.0, 47.0 }),
	     ((vnx8hf) { 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0,
			 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0 }),
	     ((vnx8hf) { 33.0, 34.0, 35.0, 36.0, 37.0, 38.0, 39.0, 40.0,
			 41.0, 42.0, 43.0, 44.0, 45.0, 46.0, 47.0, 48.0 }));
  return 0;
}
