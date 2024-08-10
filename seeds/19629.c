/* { dg-do run { target { riscv_vector } } } */
/* { dg-additional-options "--param=riscv-autovec-preference=scalable -ffast-math -fno-vect-cost-model" } */

/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv -mabi=ilp32d --param=riscv-autovec-preference=scalable -ffast-math -fno-vect-cost-model" } */

#include <stdint-gcc.h>

unsigned short __attribute__((noipa))
add_loop (unsigned short *x, int n)
{
  unsigned short res = 0;
  for (int i = 0; i < n; ++i)
    res += x[i];
  return res;
}

unsigned short __attribute__((noipa))
min_loop (unsigned short *x, int n)
{
  unsigned short res = ~0;
  for (int i = 0; i < n; ++i)
    res = res < x[i] ? res : x[i];
  return res;
}

unsigned short __attribute__((noipa))
max_loop (unsigned short *x, int n)
{
  unsigned short res = 0;
  for (int i = 0; i < n; ++i)
    res = res > x[i] ? res : x[i];
  return res;
}

unsigned short __attribute__((noipa))
and_loop (unsigned short *x, int n)
{
  unsigned short res = ~0;
  for (int i = 0; i < n; ++i)
    res &= x[i];
  return res;
}

unsigned short __attribute__((noipa))
or_loop (unsigned short *x, int n)
{
  unsigned short res = 0;
  for (int i = 0; i < n; ++i)
    res |= x[i];
  return res;
}

unsigned short __attribute__((noipa))
eor_loop (unsigned short *x, int n)
{
  unsigned short res = 0;
  for (int i = 0; i < n; ++i)
    res ^= x[i];
  return res;
}

/* { dg-final { scan-assembler-times {vredsum\.vs\s+v[0-9]+,\s*v[0-9]+,\s*v[0-9]+} 1 } } */
/* { dg-final { scan-assembler-times {vredmaxu\.vs\s+v[0-9]+,\s*v[0-9]+,\s*v[0-9]+} 1 } } */
/* { dg-final { scan-assembler-times {vredminu\.vs\s+v[0-9]+,\s*v[0-9]+,\s*v[0-9]+} 1 } } */
/* { dg-final { scan-assembler-times {vredand\.vs\s+v[0-9]+,\s*v[0-9]+,\s*v[0-9]+} 1 } } */
/* { dg-final { scan-assembler-times {vredor\.vs\s+v[0-9]+,\s*v[0-9]+,\s*v[0-9]+} 1 } } */
/* { dg-final { scan-assembler-times {vredxor\.vs\s+v[0-9]+,\s*v[0-9]+,\s*v[0-9]+} 1 } } */

#define N 0x1100

int
main (void)
{
  unsigned short x[N];
  for (int i = 0; i < N; ++i)
    x[i] = (i + 1) * (i + 2);

  if (add_loop (x, 0) != 0
      || add_loop (x, 11) != 572
      || add_loop (x, 0x100) != 22016
      || add_loop (x, 0xfff) != 20480
      || max_loop (x, 0) != 0
      || max_loop (x, 11) != 132
      || max_loop (x, 0x100) != 65280
      || max_loop (x, 0xfff) != 65504
      || or_loop (x, 0) != 0
      || or_loop (x, 11) != 0xfe
      || or_loop (x, 0x80) != 0x7ffe
      || or_loop (x, 0xb4) != 0x7ffe
      || or_loop (x, 0xb5) != 0xfffe
      || eor_loop (x, 0) != 0
      || eor_loop (x, 11) != 0xe8
      || eor_loop (x, 0x100) != 0xcf00
      || eor_loop (x, 0xfff) != 0xa000)
    __builtin_abort ();

  for (int i = 0; i < N; ++i)
    x[i] = ~x[i];

  if (min_loop (x, 0) != 65535
      || min_loop (x, 11) != 65403
      || min_loop (x, 0x100) != 255
      || min_loop (x, 0xfff) != 31
      || and_loop (x, 0) != 0xffff
      || and_loop (x, 11) != 0xff01
      || and_loop (x, 0x80) != 0x8001
      || and_loop (x, 0xb4) != 0x8001
      || and_loop (x, 0xb5) != 1)
    __builtin_abort ();

  return 0;
}
