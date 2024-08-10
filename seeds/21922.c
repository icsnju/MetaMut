/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize --param aarch64-sve-compare-costs=0" } */

#include <stdint.h>

#define PACK(TYPED, TYPES)				\
void __attribute__ ((noinline, noclone))		\
pack_##TYPED##_##TYPES (TYPED *d, TYPES *s, int size)	\
{							\
  for (int i = 0; i < size; i++)			\
    d[i] = s[i] + 1;					\
}

#define TEST_ALL(T)				\
  T (int32_t, int64_t)				\
  T (int16_t, int32_t)				\
  T (int8_t, int16_t)				\
  T (uint32_t, uint64_t)			\
  T (uint16_t, uint32_t)			\
  T (uint8_t, uint16_t)

TEST_ALL (PACK)

/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 2 } } */
/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.h, z[0-9]+\.h, z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b\n} 2 } } */

#define ARRAY_SIZE 57

#define TEST_LOOP(TYPED, TYPES)					\
  {								\
    TYPED arrayd[ARRAY_SIZE];					\
    TYPES arrays[ARRAY_SIZE];					\
    for (int i = 0; i < ARRAY_SIZE; i++)			\
      {								\
	arrays[i] = (i - 10) * 3;				\
	asm volatile ("" ::: "memory");				\
      }								\
    pack_##TYPED##_##TYPES (arrayd, arrays, ARRAY_SIZE);	\
    for (int i = 0; i < ARRAY_SIZE; i++)			\
      if (arrayd[i] != (TYPED) ((TYPES) ((i - 10) * 3) + 1))	\
	__builtin_abort ();					\
  }

int __attribute__ ((optimize (1)))
main (void)
{
  TEST_ALL (TEST_LOOP)
  return 0;
}
