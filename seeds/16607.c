/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model --param=riscv-autovec-preference=fixed-vlmax -ffast-math" } */

#include <stdint-gcc.h>

#define TEST_TYPE(TYPE, SUFFIX)                                                \
  __attribute__ ((noipa)) void copysign_##TYPE (TYPE *restrict dst,            \
						TYPE *restrict a,              \
						TYPE *restrict b,              \
						int *restrict pred, int n)     \
  {                                                                            \
    for (int i = 0; i < n; i++)                                                \
      dst[i] = pred[i] ? __builtin_copysign##SUFFIX (a[i], b[i]) : dst[i];     \
  }

#define TEST_TYPE2(TYPE, SUFFIX)                                               \
  __attribute__ ((noipa)) void copysigns_##TYPE (TYPE *restrict dst,           \
						 TYPE *restrict a, TYPE b,     \
						 int *restrict pred, int n)    \
  {                                                                            \
    for (int i = 0; i < n; i++)                                                \
      dst[i] = pred[i] ? __builtin_copysign##SUFFIX (a[i], b) : dst[i];        \
  }

#define TEST_TYPE3(TYPE, SUFFIX)                                               \
  __attribute__ ((noipa)) void xorsign_##TYPE (TYPE *restrict dst,             \
					       TYPE *restrict a,               \
					       TYPE *restrict b,               \
					       int *restrict pred, int n)      \
  {                                                                            \
    for (int i = 0; i < n; i++)                                                \
      dst[i]                                                                   \
	= pred[i] ? a[i] * __builtin_copysign##SUFFIX (1.0, b[i]) : dst[i];    \
  }

#define TEST_TYPE4(TYPE, SUFFIX)                                               \
  __attribute__ ((noipa)) void xorsigns_##TYPE (TYPE *restrict dst,            \
						TYPE *restrict a, TYPE b,      \
						int *restrict pred, int n)     \
  {                                                                            \
    for (int i = 0; i < n; i++)                                                \
      dst[i] = pred[i] ? a[i] * __builtin_copysign##SUFFIX (1.0, b) : dst[i];  \
  }

#define TEST_TYPE5(TYPE, SUFFIX)                                               \
  __attribute__ ((noipa)) void ncopysign_##TYPE (TYPE *restrict dst,           \
						 TYPE *restrict a,             \
						 TYPE *restrict b,             \
						 int *restrict pred, int n)    \
  {                                                                            \
    for (int i = 0; i < n; i++)                                                \
      dst[i] = pred[i] ? -__builtin_copysign##SUFFIX (a[i], b[i]) : dst[i];    \
  }

#define TEST_TYPE6(TYPE, SUFFIX)                                               \
  __attribute__ ((noipa)) void ncopysigns_##TYPE (TYPE *restrict dst,          \
						  TYPE *restrict a, TYPE b,    \
						  int *restrict pred, int n)   \
  {                                                                            \
    for (int i = 0; i < n; i++)                                                \
      dst[i] = pred[i] ? -__builtin_copysign##SUFFIX (a[i], b) : dst[i];       \
  }

#define TEST_ALL()                                                             \
  TEST_TYPE (_Float16, f16)                                                    \
  TEST_TYPE (float, f)                                                         \
  TEST_TYPE (double, )                                                         \
  TEST_TYPE2 (_Float16, f16)                                                   \
  TEST_TYPE2 (float, f)                                                        \
  TEST_TYPE2 (double, )                                                        \
  TEST_TYPE3 (_Float16, f16)                                                   \
  TEST_TYPE3 (float, f)                                                        \
  TEST_TYPE3 (double, )                                                        \
  TEST_TYPE4 (_Float16, f16)                                                   \
  TEST_TYPE4 (float, f)                                                        \
  TEST_TYPE4 (double, )                                                        \
  TEST_TYPE5 (_Float16, f16)                                                   \
  TEST_TYPE5 (float, f)                                                        \
  TEST_TYPE5 (double, )                                                        \
  TEST_TYPE6 (_Float16, f16)                                                   \
  TEST_TYPE6 (float, f)                                                        \
  TEST_TYPE6 (double, )

TEST_ALL ()

#include <assert.h>

#define SZ 512

#define EPS 1e-6

#define INIT_PRED()                                                            \
  int pred[SZ];                                                                \
  for (int i = 0; i < SZ; i++)                                                 \
    {                                                                          \
      pred[i] = i % 3;                                                         \
    }

#define RUN(TYPE, VAL)                                                         \
  TYPE a##TYPE[SZ];                                                            \
  TYPE b##TYPE[SZ];                                                            \
  for (int i = 0; i < SZ; i++)                                                 \
    {                                                                          \
      a##TYPE[i] = i;                                                          \
      b##TYPE[i] = (i & 1) ? VAL : -VAL;                                       \
    }                                                                          \
  copysign_##TYPE (a##TYPE, a##TYPE, b##TYPE, pred, SZ);                       \
  for (int i = 0; i < SZ; i++)                                                 \
    assert (!pred[i] || __builtin_fabs (a##TYPE[i] - ((i & 1) ? i : -i)) < EPS);

#define RUN2(TYPE, VAL)                                                        \
  TYPE a2##TYPE[SZ];                                                           \
  for (int i = 0; i < SZ; i++)                                                 \
    a2##TYPE[i] = i;                                                           \
  copysigns_##TYPE (a2##TYPE, a2##TYPE, -VAL, pred, SZ);                       \
  for (int i = 0; i < SZ; i++)                                                 \
    assert (!pred[i] || __builtin_fabs (a2##TYPE[i] + i) < EPS);

#define RUN3(TYPE, VAL)                                                        \
  TYPE a3##TYPE[SZ];                                                           \
  TYPE b3##TYPE[SZ];                                                           \
  for (int i = 0; i < SZ; i++)                                                 \
    {                                                                          \
      a3##TYPE[i] = (i & 1) ? -i : i;                                          \
      b3##TYPE[i] = (i & 1) ? VAL : -VAL;                                      \
    }                                                                          \
  xorsign_##TYPE (a3##TYPE, a3##TYPE, b3##TYPE, pred, SZ);                     \
  for (int i = 0; i < SZ; i++)                                                 \
    assert (!pred[i] || __builtin_fabs (a3##TYPE[i] + i) < EPS);

#define RUN4(TYPE, VAL)                                                        \
  TYPE a4##TYPE[SZ];                                                           \
  for (int i = 0; i < SZ; i++)                                                 \
    a4##TYPE[i] = -i;                                                          \
  xorsigns_##TYPE (a4##TYPE, a4##TYPE, -VAL, pred, SZ);                        \
  for (int i = 0; i < SZ; i++)                                                 \
    assert (!pred[i] || __builtin_fabs (a4##TYPE[i] - i) < EPS);

#define RUN5(TYPE, VAL)                                                        \
  TYPE a5##TYPE[SZ];                                                           \
  TYPE b5##TYPE[SZ];                                                           \
  for (int i = 0; i < SZ; i++)                                                 \
    {                                                                          \
      a5##TYPE[i] = i;                                                         \
      b5##TYPE[i] = (i & 1) ? VAL : -VAL;                                      \
    }                                                                          \
  ncopysign_##TYPE (a5##TYPE, a5##TYPE, b##TYPE, pred, SZ);                    \
  for (int i = 0; i < SZ; i++)                                                 \
    assert (!pred[i]                                                           \
	    || __builtin_fabs (-a5##TYPE[i] - ((i & 1) ? i : -i)) < EPS);

#define RUN6(TYPE, VAL)                                                        \
  TYPE a6##TYPE[SZ];                                                           \
  for (int i = 0; i < SZ; i++)                                                 \
    a6##TYPE[i] = i;                                                           \
  ncopysigns_##TYPE (a6##TYPE, a6##TYPE, -VAL, pred, SZ);                      \
  for (int i = 0; i < SZ; i++)                                                 \
    assert (!pred[i] || __builtin_fabs (-a6##TYPE[i] + i) < EPS);

#define RUN_ALL()                                                              \
  RUN (float, 5)                                                               \
  RUN (double, 6)                                                              \
  RUN2 (float, 11)                                                             \
  RUN2 (double, 12)                                                            \
  RUN3 (float, 16)                                                             \
  RUN3 (double, 18)                                                            \
  RUN4 (float, 17)                                                             \
  RUN4 (double, 19)                                                            \
  RUN5 (float, 123)                                                            \
  RUN5 (double, 523)                                                           \
  RUN6 (float, 777)                                                            \
  RUN6 (double, 877)

int
main ()
{
  INIT_PRED ()
  RUN_ALL ()
}
