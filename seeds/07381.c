/* { dg-do run { target { riscv_vector } } } */
/* { dg-options "--param riscv-autovec-preference=fixed-vlmax -O3 --param riscv-autovec-lmul=m2" } */

#include <stdint-gcc.h>
#include <assert.h>
#define N 8

int
main ()
{
  int mask[N] = {0, 1, 0, 1, 0, 1, 0, 1};
  int out[N] = {0};
  for (int i = 0; i < N; ++i)
    if (mask[i])
      out[i] = i;
  for (int i = 0; i < N; ++i)
    {
      if (mask[i])
	assert (out[i] == i);
      else
	assert (out[i] == 0);
    }
}
