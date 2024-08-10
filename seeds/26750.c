/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize" } */

#include <stdint.h>

#define NUM_ELEMS(TYPE) (320 / sizeof (TYPE))

#define DEF_LOOP(TYPE, CONST)					\
  void __attribute__ ((noipa))					\
  test_##CONST##_##TYPE (TYPE *restrict r, TYPE *restrict a,	\
			 TYPE *restrict b)			\
  {								\
    for (int i = 0; i < NUM_ELEMS (TYPE); ++i)			\
      r[i] = a[i] > 20 ? b[i] & CONST : 127;			\
  }

#define TEST_ALL(T)			\
  T (uint16_t, 0xff)			\
					\
  T (uint32_t, 0xff)			\
  T (uint32_t, 0xffff)			\
					\
  T (uint64_t, 0xff)			\
  T (uint64_t, 0xffff)			\
  T (uint64_t, 0xffffffff)

TEST_ALL (DEF_LOOP)

/* { dg-final { scan-assembler {\tmovprfx\t(z[0-9]+), z[0-9]+\n\tuxtb\t\1\.h, p[0-7]/m, z[0-9]+\.h\n} } } */

/* { dg-final { scan-assembler {\tmovprfx\t(z[0-9]+), z[0-9]+\n\tuxtb\t\1\.s, p[0-7]/m, z[0-9]+\.s\n} } } */
/* { dg-final { scan-assembler {\tmovprfx\t(z[0-9]+), z[0-9]+\n\tuxth\t\1\.s, p[0-7]/m, z[0-9]+\.s\n} } } */

/* { dg-final { scan-assembler {\tmovprfx\t(z[0-9]+), z[0-9]+\n\tuxtb\t\1\.d, p[0-7]/m, z[0-9]+\.d\n} } } */
/* { dg-final { scan-assembler {\tmovprfx\t(z[0-9]+), z[0-9]+\n\tuxth\t\1\.d, p[0-7]/m, z[0-9]+\.d\n} } } */
/* { dg-final { scan-assembler {\tmovprfx\t(z[0-9]+), z[0-9]+\n\tuxtw\t\1\.d, p[0-7]/m, z[0-9]+\.d\n} } } */

/* { dg-final { scan-assembler-not {\tmov\tz[^\n]*z} } } */
/* { dg-final { scan-assembler-not {\tsel\t} } } */

#define TEST_LOOP(TYPE, CONST)				\
  {							\
    TYPE r[NUM_ELEMS (TYPE)];				\
    TYPE a[NUM_ELEMS (TYPE)];				\
    TYPE b[NUM_ELEMS (TYPE)];				\
    for (int i = 0; i < NUM_ELEMS (TYPE); ++i)		\
      {							\
	a[i] = (i & 1 ? i : 3 * i);			\
	b[i] = (i >> 4) << (i & 15);			\
	asm volatile ("" ::: "memory");			\
      }							\
    test_##CONST##_##TYPE (r, a, b);			\
    for (int i = 0; i < NUM_ELEMS (TYPE); ++i)		\
      if (r[i] != (a[i] > 20 ? b[i] & CONST : 127))	\
	__builtin_abort ();				\
  }

int main ()
{
  TEST_ALL (TEST_LOOP)
  return 0;
}
