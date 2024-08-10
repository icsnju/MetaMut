/* { dg-do run { target { riscv_vector } } } */
/* { dg-options "-O3 --param riscv-autovec-preference=fixed-vlmax -Wno-psabi" } */

#include <assert.h>
/* { dg-do compile } */
/* { dg-options "-march=rv64gcv -mabi=lp64d -O3 --param riscv-autovec-preference=fixed-vlmax -Wno-psabi" } */

#include <stdint-gcc.h>

typedef int8_t vnx4i __attribute__ ((vector_size (4)));
typedef uint8_t vnx4ui __attribute__ ((vector_size (4)));

#define MASK_4  0, 2, 6, 7 

vnx4i __attribute__ ((noinline, noclone)) test_1 (vnx4i x, vnx4i y)
{
  return __builtin_shufflevector (x, y, MASK_4);
}

vnx4ui __attribute__ ((noinline, noclone)) test_2 (vnx4ui x, vnx4ui y)
{
  return __builtin_shufflevector (x, y, MASK_4);
}

/* { dg-final { scan-assembler-times {\tvcompress\.vm} 2 } } */

int
main (void)
{
  vnx4i test_1_x = {0, 1, 2, 4};
  vnx4i test_1_y = {4, 5, 7, 8};
  vnx4i test_1_except = {0, 2, 7, 8};
  vnx4i test_1_real;
  test_1_real = test_1 (test_1_x, test_1_y);
  for (int i = 0; i < 4; i++)
    assert (test_1_real[i] == test_1_except[i]);

  vnx4ui test_2_x = {0, 1, 2, 4};
  vnx4ui test_2_y = {4, 5, 6, 8};
  vnx4ui test_2_except = {0, 2, 6, 8};
  vnx4ui test_2_real;
  test_2_real = test_2 (test_2_x, test_2_y);
  for (int i = 0; i < 4; i++)
    assert (test_2_real[i] == test_2_except[i]);

  return 0;
}
