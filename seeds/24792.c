/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O3 -fno-tree-loop-distribute-patterns" } */

/* { dg-do compile } */
/* { dg-options "-O3 -fno-tree-loop-distribute-patterns" } */

#include <stdint.h>

#define NUM_ELEMS(TYPE) (1024 / sizeof (TYPE))

#define DEF_LOAD_BROADCAST(TYPE)			\
  void __attribute__ ((noinline, noclone))		\
  set_##TYPE (TYPE *restrict a, TYPE *restrict b)	\
  {							\
    for (int i = 0; i < NUM_ELEMS (TYPE); i++)		\
      a[i] = *b;					\
  }

#define DEF_LOAD_BROADCAST_IMM(TYPE, IMM, SUFFIX)	\
  void __attribute__ ((noinline, noclone))		\
  set_##TYPE##_##SUFFIX (TYPE *a)			\
  {							\
    for (int i = 0; i < NUM_ELEMS (TYPE); i++)		\
      a[i] = IMM;					\
  }

#define FOR_EACH_LOAD_BROADCAST(T)		\
  T (int8_t)					\
  T (int16_t)					\
  T (int32_t)					\
  T (int64_t)

#define FOR_EACH_LOAD_BROADCAST_IMM(T)					\
  T (int32_t, 0x12345678, imm_0x12345678)				\
  T (int64_t, 0x12345678, imm_0x12345678)				\
									\
  T (int32_t, 0xF2345678, imm_0xF2345678)				\
  T (int64_t, 0xF2345678, imm_0xF2345678)				\
									\
  T (int64_t, (int64_t) 0xFEBA716B12371765, imm_FEBA716B12371765)

FOR_EACH_LOAD_BROADCAST (DEF_LOAD_BROADCAST)
FOR_EACH_LOAD_BROADCAST_IMM (DEF_LOAD_BROADCAST_IMM)

/* { dg-final { scan-assembler-times {\tld1rb\tz[0-9]+\.b, p[0-7]/z, } 1 } } */
/* { dg-final { scan-assembler-times {\tld1rh\tz[0-9]+\.h, p[0-7]/z, } 1 } } */
/* { dg-final { scan-assembler-times {\tld1rw\tz[0-9]+\.s, p[0-7]/z, } 3 } } */
/* { dg-final { scan-assembler-times {\tld1rd\tz[0-9]+\.d, p[0-7]/z, } 4 } } */

#define TEST_LOAD_BROADCAST(TYPE)		\
  {						\
    TYPE v[NUM_ELEMS (TYPE)];			\
    TYPE val = 99;				\
    set_##TYPE (v, &val);			\
    for (int i = 0; i < NUM_ELEMS (TYPE); i++)	\
      {						\
	if (v[i] != (TYPE) 99)			\
	  __builtin_abort ();			\
	asm volatile ("" ::: "memory");		\
      }						\
  }

#define TEST_LOAD_BROADCAST_IMM(TYPE, IMM, SUFFIX)	\
  {							\
    TYPE v[NUM_ELEMS (TYPE)];				\
    set_##TYPE##_##SUFFIX (v);				\
    for (int i = 0; i < NUM_ELEMS (TYPE); i++ )		\
      {							\
	if (v[i] != (TYPE) IMM)				\
	  __builtin_abort ();				\
	asm volatile ("" ::: "memory");			\
      }							\
  }

int __attribute__ ((optimize (1)))
main (int argc, char **argv)
{
  FOR_EACH_LOAD_BROADCAST (TEST_LOAD_BROADCAST)
  FOR_EACH_LOAD_BROADCAST_IMM (TEST_LOAD_BROADCAST_IMM)

  return 0;
}
