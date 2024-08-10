/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O3 -fno-inline" } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize" } */

#include <stdint.h>
#include <stdbool.h>

#define VEC_BOOL(NAME, OP, VARTYPE, INDUCTYPE)				\
void __attribute__ ((noinline, noclone))				\
vec_bool_##NAME##_##VARTYPE##_##INDUCTYPE (VARTYPE *dst, VARTYPE *src,	\
					   INDUCTYPE start,		\
					   INDUCTYPE n,			\
					   INDUCTYPE mask)		\
{									\
  for (INDUCTYPE i = 0; i < n; i++)					\
    {									\
      bool lhs = i >= start;						\
      bool rhs = (i & mask) != 0x3D;					\
      if (lhs OP rhs)							\
        dst[i] = src[i];						\
    }									\
}

#define TEST_OP(T, NAME, OP)			\
  T (NAME, OP, uint8_t, uint8_t)		\
  T (NAME, OP, uint16_t, uint16_t)		\
  T (NAME, OP, uint32_t, uint32_t)		\
  T (NAME, OP, uint64_t, uint64_t)		\
  T (NAME, OP, float, uint32_t)			\
  T (NAME, OP, double, uint64_t)

#define TEST_ALL(T)				\
  TEST_OP (T, cmpeq, ==)			\
  TEST_OP (T, cmpne, !=)

TEST_ALL (VEC_BOOL)

/* Both cmpne and cmpeq loops will contain an exclusive predicate or.  */
/* { dg-final { scan-assembler-times {\teors?\tp[0-9]*\.b, p[0-7]/z, p[0-9]*\.b, p[0-9]*\.b\n} 12 } } */
/* cmpeq will also contain a masked predicate not operation, which gets
   folded to BIC.  */
/* { dg-final { scan-assembler-times {\tbic\tp[0-9]+\.b, p[0-7]/z, p[0-9]+\.b, p[0-9]+\.b\n} 6 } } */

#define N 103

#define TEST_VEC_BOOL(NAME, OP, VARTYPE, INDUCTYPE)		\
{								\
  INDUCTYPE i;							\
  VARTYPE src[N];						\
  VARTYPE dst[N];						\
  for (i = 0; i < N; i++)					\
    {								\
      src[i] = i;						\
      dst[i] = i * 2;						\
      asm volatile ("" ::: "memory");				\
    }								\
  vec_bool_##NAME##_##VARTYPE##_##INDUCTYPE (dst, src, 13,	\
					     97, 0xFF);		\
  for (i = 0; i < 13; i++)					\
    if (dst[i] != (VARTYPE) (0 OP 1 ? i : i * 2))		\
      __builtin_abort ();					\
  for (i = 13; i < 97; i++)					\
    if (dst[i] != (VARTYPE) (1 OP (i != 0x3D) ? i : i * 2))	\
      __builtin_abort ();					\
  for (i = 97; i < N; i++)					\
    if (dst[i] != (i * 2))					\
      __builtin_abort ();					\
}

int __attribute__ ((optimize (1)))
main ()
{
  TEST_ALL (TEST_VEC_BOOL)
  return 0;
}
