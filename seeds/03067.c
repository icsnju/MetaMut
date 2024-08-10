/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do assemble { target aarch64_asm_sve_ok } } */
/* { dg-options "-O2 -ftree-vectorize --save-temps" } */

#include <stdint.h>

void __attribute__ ((noinline, noclone))
popcount_32 (unsigned int *restrict dst, uint32_t *restrict src, int size)
{
  for (int i = 0; i < size; ++i)
    dst[i] = __builtin_popcount (src[i]);
}

void __attribute__ ((noinline, noclone))
popcount_64 (unsigned int *restrict dst, uint64_t *restrict src, int size)
{
  for (int i = 0; i < size; ++i)
    dst[i] = __builtin_popcountll (src[i]);
}

/* { dg-final { scan-assembler-times {\tcnt\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tcnt\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d\n} 2 } } */
/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */

extern void abort (void) __attribute__ ((noreturn));

unsigned int data[] = {
  0x11111100, 6,
  0xe0e0f0f0, 14,
  0x9900aab3, 13,
  0x00040003, 3,
  0x000e000c, 5,
  0x22227777, 16,
  0x12341234, 10,
  0x0, 0
};

int __attribute__ ((optimize (1)))
main (void)
{
  unsigned int count = sizeof (data) / sizeof (data[0]) / 2;

  uint32_t in32[count];
  unsigned int out32[count];
  for (unsigned int i = 0; i < count; ++i)
    {
      in32[i] = data[i * 2];
      asm volatile ("" ::: "memory");
    }
  popcount_32 (out32, in32, count);
  for (unsigned int i = 0; i < count; ++i)
    if (out32[i] != data[i * 2 + 1])
      abort ();

  count /= 2;
  uint64_t in64[count];
  unsigned int out64[count];
  for (unsigned int i = 0; i < count; ++i)
    {
      in64[i] = ((uint64_t) data[i * 4] << 32) | data[i * 4 + 2];
      asm volatile ("" ::: "memory");
    }
  popcount_64 (out64, in64, count);
  for (unsigned int i = 0; i < count; ++i)
    if (out64[i] != data[i * 4 + 1] + data[i * 4 + 3])
      abort ();

  return 0;
}
