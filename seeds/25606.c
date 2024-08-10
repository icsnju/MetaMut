/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfhmin -mabi=ilp32d --param riscv-autovec-preference=scalable -fdump-tree-vect-details" } */

#include <assert.h>
#include <stdint-gcc.h>

#define N 777

int cond[N] = {0};
#define test_1(TYPE)                                                           \
  TYPE a_##TYPE[N];                                                            \
  TYPE b_##TYPE[N];                                                            \
  void __attribute__ ((noinline, noclone)) test_1_##TYPE (unsigned int n)      \
  {                                                                            \
    unsigned int i = 0;                                                        \
    for (i = 0; i < n; i++)                                                    \
      if (cond[i])                                                             \
	b_##TYPE[i] = a_##TYPE[i];                                             \
  }

#define run_1(TYPE)                                                            \
  for (unsigned int i = 0; i < N; i++)                                         \
    a_##TYPE[i] = i * 2 * 33 + 1 + 109;                                        \
  test_1_##TYPE (5);                                                           \
  for (unsigned int i = 0; i < N; i++)                                         \
    {                                                                          \
      if (cond[i] && i < 5)                                                    \
	assert (b_##TYPE[i] == a_##TYPE[i]);                                   \
      else                                                                     \
	assert (b_##TYPE[i] == 0);                                             \
    }

#define run_2(TYPE)                                                            \
  for (unsigned int i = 0; i < N; i++)                                         \
    a_##TYPE[i] = i * 2 * 57 + 1 + 999;                                        \
  test_1_##TYPE (17);                                                          \
  for (unsigned int i = 0; i < N; i++)                                         \
    {                                                                          \
      if (cond[i] && i < 17)                                                   \
	assert (b_##TYPE[i] == a_##TYPE[i]);                                   \
      else                                                                     \
	assert (b_##TYPE[i] == 0);                                             \
    }

#define run_3(TYPE)                                                            \
  for (unsigned int i = 0; i < N; i++)                                         \
    a_##TYPE[i] = i * 2 * 77 + 1 + 3;                                          \
  test_1_##TYPE (32);                                                          \
  for (unsigned int i = 0; i < N; i++)                                         \
    {                                                                          \
      if (cond[i] && i < 32)                                                   \
	assert (b_##TYPE[i] == a_##TYPE[i]);                                   \
      else                                                                     \
	assert (b_##TYPE[i] == 0);                                             \
    }

#define run_4(TYPE)                                                            \
  for (unsigned int i = 0; i < N; i++)                                         \
    a_##TYPE[i] = i * 2 * 45 + 1 + 11;                                         \
  test_1_##TYPE (128);                                                         \
  for (unsigned int i = 0; i < N; i++)                                         \
    {                                                                          \
      if (cond[i] && i < 128)                                                  \
	assert (b_##TYPE[i] == a_##TYPE[i]);                                   \
      else                                                                     \
	assert (b_##TYPE[i] == 0);                                             \
    }

#define run_5(TYPE)                                                            \
  for (unsigned int i = 0; i < N; i++)                                         \
    a_##TYPE[i] = i * 2 * 199 + 1 + 79;                                        \
  test_1_##TYPE (177);                                                         \
  for (unsigned int i = 0; i < N; i++)                                         \
    {                                                                          \
      if (cond[i] && i < 177)                                                  \
	assert (b_##TYPE[i] == a_##TYPE[i]);                                   \
      else                                                                     \
	assert (b_##TYPE[i] == 0);                                             \
    }

#define run_6(TYPE)                                                            \
  for (unsigned int i = 0; i < N; i++)                                         \
    a_##TYPE[i] = i * 2 * 377 + 1 + 73;                                        \
  test_1_##TYPE (255);                                                         \
  for (unsigned int i = 0; i < N; i++)                                         \
    {                                                                          \
      if (cond[i] && i < 255)                                                  \
	assert (b_##TYPE[i] == a_##TYPE[i]);                                   \
      else                                                                     \
	assert (b_##TYPE[i] == 0);                                             \
    }

#define run_7(TYPE)                                                            \
  for (unsigned int i = 0; i < N; i++)                                         \
    a_##TYPE[i] = i * 2 * 98 + 1 + 66;                                         \
  test_1_##TYPE (333);                                                         \
  for (unsigned int i = 0; i < N; i++)                                         \
    {                                                                          \
      if (cond[i] && i < 333)                                                  \
	assert (b_##TYPE[i] == a_##TYPE[i]);                                   \
      else                                                                     \
	assert (b_##TYPE[i] == 0);                                             \
    }

#define run_8(TYPE)                                                            \
  for (unsigned int i = 0; i < N; i++)                                         \
    a_##TYPE[i] = i * 2 * 7 + 1 * 7;                                           \
  test_1_##TYPE (512);                                                         \
  for (unsigned int i = 0; i < N; i++)                                         \
    {                                                                          \
      if (cond[i] && i < 512)                                                  \
	assert (b_##TYPE[i] == a_##TYPE[i]);                                   \
      else                                                                     \
	assert (b_##TYPE[i] == 0);                                             \
    }

#define run_9(TYPE)                                                            \
  for (unsigned int i = 0; i < N; i++)                                         \
    a_##TYPE[i] = i * 2 + 1 + 88;                                              \
  test_1_##TYPE (637);                                                         \
  for (unsigned int i = 0; i < N; i++)                                         \
    {                                                                          \
      if (cond[i] && i < 637)                                                  \
	assert (b_##TYPE[i] == a_##TYPE[i]);                                   \
      else                                                                     \
	assert (b_##TYPE[i] == 0);                                             \
    }

#define run_10(TYPE)                                                           \
  for (unsigned int i = 0; i < N; i++)                                         \
    a_##TYPE[i] = i * 2 * 331 + 1 + 547;                                       \
  test_1_##TYPE (777);                                                         \
  for (unsigned int i = 0; i < N; i++)                                         \
    {                                                                          \
      if (cond[i] && i < 777)                                                  \
	assert (b_##TYPE[i] == a_##TYPE[i]);                                   \
      else                                                                     \
	assert (b_##TYPE[i] == 0);                                             \
    }

#define TEST_ALL(T)                                                            \
  T (int8_t)                                                                   \
  T (uint8_t)                                                                  \
  T (int16_t)                                                                  \
  T (uint16_t)                                                                 \
  T (int32_t)                                                                  \
  T (uint32_t)                                                                 \
  T (int64_t)                                                                  \
  T (uint64_t)                                                                 \
  T (_Float16)                                                                 \
  T (float)                                                                    \
  T (double)

TEST_ALL (test_1)

/* { dg-final { scan-tree-dump-times "vectorized 1 loops in function" 11 "vect" } } */
