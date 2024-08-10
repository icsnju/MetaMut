/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -fno-tree-loop-distribute-patterns" } */

#include <stdint.h>

#define NUM_ELEMS(TYPE) (128 / sizeof (TYPE))

#define DUP_FN(TYPE)				\
void __attribute__ ((noinline, noclone))	\
dup_##TYPE (TYPE *r, TYPE v)			\
{						\
  for (int i = 0; i < NUM_ELEMS (TYPE); i++)	\
    r[i] = v;					\
}

DUP_FN (int8_t)
DUP_FN (int16_t)
DUP_FN (int32_t)
DUP_FN (int64_t)
DUP_FN (_Float16)
DUP_FN (float)
DUP_FN (double)

/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.b, w[0-9]+\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.h, w[0-9]+\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.s, w[0-9]+\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.d, x[0-9]+\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.h, h[0-9]+\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.s, s[0-9]+\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.d, d[0-9]+\n} 1 } } */

#define TEST_INIT_VECTOR(TYPE, VAL)		\
  {						\
  TYPE r[NUM_ELEMS (TYPE)];			\
  dup_##TYPE (r, VAL);				\
  for (int i = 0; i < NUM_ELEMS (TYPE); i++)	\
    if (r[i] != VAL)				\
      __builtin_abort ();			\
  }

int __attribute__ ((optimize (1)))
main (void)
{
  TEST_INIT_VECTOR (int8_t, 0x2a);
  TEST_INIT_VECTOR (int16_t, 0x3976);
  TEST_INIT_VECTOR (int32_t, 0x31232976);
  TEST_INIT_VECTOR (int64_t, 0x9489363731232976LL);

  TEST_INIT_VECTOR (_Float16, -0x1.fp10);
  TEST_INIT_VECTOR (float, -0x1.fe02p10);
  TEST_INIT_VECTOR (double, 0x1.fe02eeeee1p10);

  return 0;
}
