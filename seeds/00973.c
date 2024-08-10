/* { dg-do run { target { riscv_vector } } } */
/* { dg-options "-O3 --param riscv-autovec-preference=fixed-vlmax -Wno-psabi" } */

#include <assert.h>
/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvfh -mabi=lp64d -O3 --param riscv-autovec-preference=fixed-vlmax -Wno-psabi" } */

#include <stdint-gcc.h>

typedef int8_t vnx16i __attribute__ ((vector_size (16)));
typedef int16_t vnx8i __attribute__ ((vector_size (16)));
typedef int32_t vnx4i __attribute__ ((vector_size (16)));
typedef uint8_t vnx16ui __attribute__ ((vector_size (16)));
typedef uint16_t vnx8ui __attribute__ ((vector_size (16)));
typedef uint32_t vnx4ui __attribute__ ((vector_size (16)));
typedef _Float16 vnx8f __attribute__ ((vector_size (16)));
typedef float vnx4f __attribute__ ((vector_size (16)));

#define MASK_4 0, 2, 5, 6
#define MASK_8 0, 1, 2, 5, 10, 11, 12, 13
#define MASK_16 1, 3, 4, 6, 7, 8, 14, 15, 20, 21, 22, 23, 24, 25, 26, 27

vnx16i __attribute__ ((noinline, noclone)) test_1 (vnx16i x, vnx16i y)
{
  return __builtin_shufflevector (x, y, MASK_16);
}

vnx8i __attribute__ ((noinline, noclone)) test_2 (vnx8i x, vnx8i y)
{
  return __builtin_shufflevector (x, y, MASK_8);
}

vnx4i __attribute__ ((noinline, noclone)) test_3 (vnx4i x, vnx4i y)
{
  return __builtin_shufflevector (x, y, MASK_4);
}

vnx16ui __attribute__ ((noinline, noclone)) test_4 (vnx16ui x, vnx16ui y)
{
  return __builtin_shufflevector (x, y, MASK_16);
}

vnx8ui __attribute__ ((noinline, noclone)) test_5 (vnx8ui x, vnx8ui y)
{
  return __builtin_shufflevector (x, y, MASK_8);
}

vnx4ui __attribute__ ((noinline, noclone)) test_6 (vnx4ui x, vnx4ui y)
{
  return __builtin_shufflevector (x, y, MASK_4);
}

vnx8f __attribute__ ((noinline, noclone)) test_7 (vnx8f x, vnx8f y)
{
  return __builtin_shufflevector (x, y, MASK_8);
}

vnx4f __attribute__ ((noinline, noclone)) test_8 (vnx4f x, vnx4f y)
{
  return __builtin_shufflevector (x, y, MASK_4);
}

/* { dg-final { scan-assembler-times {\tvcompress.vm} 8 } } */
/* { dg-final { scan-assembler-times {\tvslideup} 8 } } */

int
main (void)
{
  vnx16i test_1_x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  vnx16i test_1_y
    = {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
  vnx16i test_1_except
    = {1, 3, 4, 6, 7, 8, 14, 15, 20, 21, 22, 23, 24, 25, 26, 27};
  vnx16i test_1_real;
  test_1_real = test_1 (test_1_x, test_1_y);
  for (int i = 0; i < 16; i++)
    assert (test_1_real[i] == test_1_except[i]);

  vnx8i test_2_x = {0, 1, 2, 3, 4, 5, 6, 7};
  vnx8i test_2_y = {8, 9, 10, 11, 12, 13, 14, 15};
  vnx8i test_2_except = {0, 1, 2, 5, 10, 11, 12, 13};
  vnx8i test_2_real;
  test_2_real = test_2 (test_2_x, test_2_y);
  for (int i = 0; i < 8; i++)
    assert (test_2_real[i] == test_2_except[i]);

  vnx4i test_3_x = {0, 1, 2, 3};
  vnx4i test_3_y = {4, 5, 6, 7};
  vnx4i test_3_except = {0, 2, 5, 6};
  vnx4i test_3_real;
  test_3_real = test_3 (test_3_x, test_3_y);
  for (int i = 0; i < 4; i++)
    assert (test_3_real[i] == test_3_except[i]);

  vnx16ui test_4_x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  vnx16ui test_4_y
    = {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
  vnx16ui test_4_except
    = {1, 3, 4, 6, 7, 8, 14, 15, 20, 21, 22, 23, 24, 25, 26, 27};
  vnx16ui test_4_real;
  test_4_real = test_4 (test_4_x, test_4_y);
  for (int i = 0; i < 16; i++)
    assert (test_4_real[i] == test_4_except[i]);

  vnx8ui test_5_x = {0, 1, 2, 3, 4, 5, 6, 7};
  vnx8ui test_5_y = {8, 9, 10, 11, 12, 13, 14, 15};
  vnx8ui test_5_except = {0, 1, 2, 5, 10, 11, 12, 13};
  vnx8ui test_5_real;
  test_5_real = test_5 (test_5_x, test_5_y);
  for (int i = 0; i < 8; i++)
    assert (test_5_real[i] == test_5_except[i]);

  vnx4ui test_6_x = {0, 1, 2, 3};
  vnx4ui test_6_y = {4, 5, 6, 7};
  vnx4ui test_6_except = {0, 2, 5, 6};
  vnx4ui test_6_real;
  test_6_real = test_6 (test_6_x, test_6_y);
  for (int i = 0; i < 4; i++)
    assert (test_6_real[i] == test_6_except[i]);

  vnx8f test_7_x = {0, 1, 2, 3, 4, 5, 6, 7};
  vnx8f test_7_y = {8, 9, 10, 11, 12, 13, 14, 15};
  vnx8f test_7_except = {0, 1, 2, 5, 10, 11, 12, 13};
  vnx8f test_7_real;
  test_7_real = test_7 (test_7_x, test_7_y);
  for (int i = 0; i < 8; i++)
    assert (test_7_real[i] == test_7_except[i]);

  vnx4f test_8_x = {0, 1, 2, 3};
  vnx4f test_8_y = {4, 5, 6, 7};
  vnx4f test_8_except = {0, 2, 5, 6};
  vnx4f test_8_real;
  test_8_real = test_8 (test_8_x, test_8_y);
  for (int i = 0; i < 4; i++)
    assert (test_8_real[i] == test_8_except[i]);

  return 0;
}
