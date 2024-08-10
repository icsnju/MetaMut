/* { dg-do run { target { riscv_vector } } } */
/* { dg-options "-O3 --param riscv-autovec-preference=fixed-vlmax -Wno-psabi" } */

#include <assert.h>
/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvfh -mabi=lp64d -O3 --param riscv-autovec-preference=fixed-vlmax -Wno-psabi" } */

#include <stdint-gcc.h>

typedef int8_t vnx8i __attribute__ ((vector_size (8)));
typedef int16_t vnx4i __attribute__ ((vector_size (8)));
typedef uint8_t vnx8ui __attribute__ ((vector_size (8)));
typedef uint16_t vnx4ui __attribute__ ((vector_size (8)));
typedef _Float16 vnx4f __attribute__ ((vector_size (8)));

#define MASK_4 1, 3, 6, 7
#define MASK_8 2, 3, 5, 6, 11, 12, 13, 14

vnx8i __attribute__ ((noinline, noclone))
test_1 (vnx8i x, vnx8i y)
{
  return __builtin_shufflevector (x, y, MASK_8);
}

vnx4i __attribute__ ((noinline, noclone))
test_2 (vnx4i x, vnx4i y)
{
  return __builtin_shufflevector (x, y, MASK_4);
}

vnx8ui __attribute__ ((noinline, noclone))
test_3 (vnx8ui x, vnx8ui y)
{
  return __builtin_shufflevector (x, y, MASK_8);
}

vnx4ui __attribute__ ((noinline, noclone))
test_4 (vnx4ui x, vnx4ui y)
{
  return __builtin_shufflevector (x, y, MASK_4);
}

vnx4f __attribute__ ((noinline, noclone))
test_5 (vnx4f x, vnx4f y)
{
  return __builtin_shufflevector (x, y, MASK_4);
}

/* { dg-final { scan-assembler-times {\tvcompress\.vm} 5 } } */
/* { dg-final { scan-assembler-times {\tvslideup} 2 } } */

int
main (void)
{
  vnx8i test_1_x = {0, 1, 2, 3, 5, 6, 7, 8};
  vnx8i test_1_y = {8, 9, 10, 11, 13, 14, 15, 16};
  vnx8i test_1_except = {2, 3, 6, 7, 11, 13, 14, 15};
  vnx8i test_1_real;
  test_1_real = test_1 (test_1_x, test_1_y);
  for (int i = 0; i < 8; i++)
    assert (test_1_real[i] == test_1_except[i]);

  vnx4i test_2_x = {1, 2, 3, 4};
  vnx4i test_2_y = {5, 6, 7, 8};
  vnx4i test_2_except = {2, 4, 7, 8};
  vnx4i test_2_real;
  test_2_real = test_2 (test_2_x, test_2_y);
  for (int i = 0; i < 4; i++)
    assert (test_2_real[i] == test_2_except[i]);

  vnx8ui test_3_x = {0, 1, 2, 3, 4, 5, 6, 8};
  vnx8ui test_3_y = {8, 9, 10, 11, 12, 13, 15, 16};
  vnx8ui test_3_except = {2, 3, 5, 6, 11, 12, 13, 15};
  vnx8ui test_3_real;
  test_3_real = test_3 (test_3_x, test_3_y);
  for (int i = 0; i < 8; i++)
    assert (test_3_real[i] == test_3_except[i]);

  vnx4ui test_4_x = {1, 2, 3, 4};
  vnx4ui test_4_y = {4, 5, 6, 8};
  vnx4ui test_4_except = {2, 4, 6, 8};
  vnx4ui test_4_real;
  test_4_real = test_4 (test_4_x, test_4_y);
  for (int i = 0; i < 4; i++)
    assert (test_4_real[i] == test_4_except[i]);

  vnx4f test_5_x = {0, 1, 3, 4};
  vnx4f test_5_y = {4, 5, 6, 7};
  vnx4f test_5_except = {1, 4, 6, 7};
  vnx4f test_5_real;
  test_5_real = test_5 (test_5_x, test_5_y);
  for (int i = 0; i < 4; i++)
    assert (test_5_real[i] == test_5_except[i]);

  return 0;
}
