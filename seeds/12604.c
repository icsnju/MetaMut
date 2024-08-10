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

#define VEC_PERM_CONST(TYPE, MASK)			\
TYPE __attribute__ ((noinline, noclone)) 		\
vec_perm_##TYPE (TYPE values1, TYPE values2)		\
{							\
  return __builtin_shuffle (values1, values2, MASK);	\
}

VEC_PERM_CONST (vnx2di,  ((vnx2di)  { 4, 3, 6, 1 }));
VEC_PERM_CONST (vnx4si,  ((vnx4si)  { 3, 9, 11, 12, 2, 4, 4, 2 }));
VEC_PERM_CONST (vnx8hi,  ((vnx8hi)  { 8, 27, 5, 4, 21, 12, 13, 0,
				      22, 1, 8, 9, 3, 24, 15, 1 }));
VEC_PERM_CONST (vnx16qi, ((vnx16qi) { 13, 31, 11, 2, 48, 28, 3, 4,
				      54, 11, 30, 1, 0, 61, 2, 3,
				      4, 5, 11, 63, 24, 11, 42, 39,
				      2, 57, 22, 11, 6, 16, 18, 21 }));
VEC_PERM_CONST (vnx2df,  ((vnx2di) { 7, 3, 2, 1 }));
VEC_PERM_CONST (vnx4sf,  ((vnx4si) { 1, 9, 13, 11, 2, 5, 4, 2 }));
VEC_PERM_CONST (vnx8hf,  ((vnx8hi) { 8, 27, 5, 4, 21, 12, 13, 0,
				     22, 1, 8, 9, 3, 24, 15, 1 }));

/* { dg-final { scan-assembler-times {\ttbl\tz[0-9]+\.d, z[0-9]+\.d, z[0-9]+\.d\n} 4 } } */
/* { dg-final { scan-assembler-times {\ttbl\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 4 } } */
/* { dg-final { scan-assembler-times {\ttbl\tz[0-9]+\.h, z[0-9]+\.h, z[0-9]+\.h\n} 4 } } */
/* { dg-final { scan-assembler-times {\ttbl\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b\n} 2 } } */
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

#define VEC_PERM_CONST_OVERRUN(TYPE, MASK)			\
TYPE vec_perm_overrun_##TYPE (TYPE values1, TYPE values2)	\
{								\
  return __builtin_shuffle (values1, values2, MASK);		\
}

VEC_PERM_CONST_OVERRUN (vnx2di,  ((vnx2di)  { 4 + (8 * 1), 3 + (8 * 1),
					      6 + (8 * 2), 1 + (8 * 3) }));
VEC_PERM_CONST_OVERRUN (vnx4si,  ((vnx4si)  { 3 + (16 * 3), 9 + (16 * 4),
					      11 + (16 * 5), 12 + (16 * 3),
					      2 + (16 * 2), 4 + (16 * 1),
					      4 + (16 * 2), 2 + (16 * 1) }));
VEC_PERM_CONST_OVERRUN (vnx8hi,  ((vnx8hi)  { 8 + (32 * 3), 27 + (32 * 1),
					      5 + (32 * 3), 4 + (32 * 3),
					      21 + (32 * 1), 12 + (32 * 3),
					      13 + (32 * 3), 0 + (32 * 1),
					      22 + (32 * 2), 1 + (32 * 2),
					      8 + (32 * 2), 9 + (32 * 1),
					      3 + (32 * 2), 24 + (32 * 2),
					      15 + (32 * 1), 1 + (32 * 1) }));
VEC_PERM_CONST_OVERRUN (vnx16qi, ((vnx16qi) { 13 + (64 * 2), 31 + (64 * 2),
					      11 + (64 * 2), 2 + (64 * 1),
					      48 + (64 * 1), 28 + (64 * 2),
					      3 + (64 * 2), 4 + (64 * 3),
					      54 + (64 * 1), 11 + (64 * 2),
					      30 + (64 * 2), 1 + (64 * 1),
					      0 + (64 * 1), 61 + (64 * 2),
					      2 + (64 * 3), 3 + (64 * 2),
					      4 + (64 * 3), 5 + (64 * 3),
					      11 + (64 * 3), 63 + (64 * 1),
					      24 + (64 * 1), 11 + (64 * 3),
					      42 + (64 * 3), 39 + (64 * 2),
					      2 + (64 * 2), 57 + (64 * 3),
					      22 + (64 * 3), 11 + (64 * 2),
					      6 + (64 * 2), 16 + (64 * 2),
					      18 + (64 * 2), 21 + (64 * 3) }));
VEC_PERM_CONST_OVERRUN (vnx2df,  ((vnx2di)  { 7 + (8 * 1), 3 + (8 * 3),
					      2 + (8 * 5), 1 + (8 * 3) }));
VEC_PERM_CONST_OVERRUN (vnx4sf,  ((vnx4si)  { 1 + (16 * 1), 9 + (16 * 2),
					      13 + (16 * 2), 11 + (16 * 3),
					      2 + (16 * 2), 5 + (16 * 2),
					      4 + (16 * 4), 2 + (16 * 3) }));
VEC_PERM_CONST_OVERRUN (vnx8hf,  ((vnx8hi)  { 8 + (32 * 3), 27 + (32 * 1),
					      5 + (32 * 3), 4 + (32 * 3),
					      21 + (32 * 1), 12 + (32 * 3),
					      13 + (32 * 3), 0 + (32 * 1),
					      22 + (32 * 2), 1 + (32 * 2),
					      8 + (32 * 2), 9 + (32 * 1),
					      3 + (32 * 2), 24 + (32 * 2),
					      15 + (32 * 1), 1 + (32 * 1) }));

/* { dg-final { scan-assembler-times {\ttbl\tz[0-9]+\.d, z[0-9]+\.d, z[0-9]+\.d\n} 4 } } */
/* { dg-final { scan-assembler-times {\ttbl\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 4 } } */
/* { dg-final { scan-assembler-times {\ttbl\tz[0-9]+\.h, z[0-9]+\.h, z[0-9]+\.h\n} 4 } } */
/* { dg-final { scan-assembler-times {\ttbl\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b\n} 2 } } */

#define TEST_VEC_PERM(TYPE, EXPECTED_RESULT, VALUES1, VALUES2)		\
{									\
  TYPE expected_result = EXPECTED_RESULT;				\
  TYPE values1 = VALUES1;						\
  TYPE values2 = VALUES2;						\
  TYPE dest;								\
  dest = vec_perm_##TYPE (values1, values2);				\
  if (__builtin_memcmp (&dest, &expected_result, sizeof (TYPE)) != 0)	\
    __builtin_abort ();							\
  TYPE dest2;								\
  dest2 = vec_perm_overrun_##TYPE (values1, values2);			\
  if (__builtin_memcmp (&dest, &expected_result, sizeof (TYPE)) != 0)	\
    __builtin_abort ();							\
}

int main (void)
{
  TEST_VEC_PERM (vnx2di,
		 ((vnx2di) { 12, 7, 36, 5 }),
		 ((vnx2di) { 4, 5, 6, 7 }),
		 ((vnx2di) { 12, 24, 36, 48 }));
  TEST_VEC_PERM (vnx4si,
		 ((vnx4si) { 6, 34, 36, 37, 5, 7, 7, 5 }),
		 ((vnx4si) { 3, 4, 5, 6, 7, 8, 9, 10 }),
		 ((vnx4si) { 33, 34, 35, 36, 37, 38, 39, 40 }));
  TEST_VEC_PERM (vnx8hi,
		 ((vnx8hi) { 11, 44, 8, 7, 38, 15, 16, 3,
			     39, 4, 11, 12, 6, 41, 18, 4 }),
		 ((vnx8hi) { 3, 4, 5, 6, 7, 8, 9, 10, 11,
			     12, 13, 14, 15, 16, 17, 18 }),
		 ((vnx8hi) { 33, 34, 35, 36, 37, 38, 39, 40,
			     41, 42, 43, 44, 45, 46, 47, 48 }));
  TEST_VEC_PERM (vnx16qi,
		 ((vnx16qi) { 5, 7, 7, 6, 12, 4, 7, 4,
			      36, 7, 6, 5, 4, 24, 6, 7,
			      4, 5, 7, 48, 4, 7, 36, 48,
			      6, 24, 6, 7, 6, 4, 6, 5 }),
		 ((vnx16qi) { 4, 5, 6, 7, 4, 5, 6, 7,
			      4, 5, 6, 7, 4, 5, 6, 7,
			      4, 5, 6, 7, 4, 5, 6, 7,
			      4, 5, 6, 7, 4, 5, 6, 7 }),
		 ((vnx16qi) { 12, 24, 36, 48, 12, 24, 36, 48,
			      12, 24, 36, 48, 12, 24, 36, 48,
			      12, 24, 36, 48, 12, 24, 36, 48,
			      12, 24, 36, 48, 12, 24, 36, 48 }));
  TEST_VEC_PERM (vnx2df,
		 ((vnx2df) { 48.5, 7.5, 6.5, 5.5 }),
		 ((vnx2df) { 4.5, 5.5, 6.5, 7.5 }),
		 ((vnx2df) { 12.5, 24.5, 36.5, 48.5 }));
  TEST_VEC_PERM (vnx4sf,
		 ((vnx4sf) { 4.5, 34.5, 38.5, 36.5, 5.5, 8.5, 7.5, 5.5 }),
		 ((vnx4sf) { 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5 }),
		 ((vnx4sf) { 33.5, 34.5, 35.5, 36.5,
			     37.5, 38.5, 39.5, 40.5 }));
  TEST_VEC_PERM (vnx8hf,
		 ((vnx8hf) { 11.0, 44.0, 8.0, 7.0, 38.0, 15.0, 16.0, 3.0,
			     39.0, 4.0, 11.0, 12.0, 6.0, 41.0, 18.0, 4.0 }),
		 ((vnx8hf) { 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0,
			     12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0 }),
		 ((vnx8hf) { 33.0, 34.0, 35.0, 36.0, 37.0, 38.0, 39.0, 40.0,
			     41.0, 42.0, 43.0, 44.0, 45.0, 46.0, 47.0, 48.0 }));
  return 0;
}
