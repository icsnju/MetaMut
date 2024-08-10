/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do assemble { target aarch64_asm_sve_ok } } */
/* { dg-options "-O2 -ftree-vectorize --save-temps" } */

#include <stdint.h>

void __attribute__ ((noinline, noclone))
clz_32 (unsigned int *restrict dst, uint32_t *restrict src, int size)
{
  for (int i = 0; i < size; ++i)
    dst[i] = __builtin_clz (src[i]);
}

void __attribute__ ((noinline, noclone))
clz_64 (unsigned int *restrict dst, uint64_t *restrict src, int size)
{
  for (int i = 0; i < size; ++i)
    dst[i] = __builtin_clzll (src[i]);
}

/* { dg-final { scan-assembler-times {\tclz\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tclz\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d\n} 2 } } */
/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */

extern void abort (void) __attribute__ ((noreturn));

unsigned int data[] = {
  0xffffff80, 0,
  0xffffffff, 0,
  0x00000000, 32,
  0x80000000, 0,
  0x7fffffff, 1,
  0x000003ff, 22,
  0x1fffffff, 3,
  0x0000ffff, 16,
  0xffff0000, 0
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
  clz_32 (out32, in32, count);
  for (unsigned int i = 0; i < count; ++i)
    if (out32[i] != data[i * 2 + 1])
      abort ();

  uint64_t in64[count];
  unsigned int out64[count];
  for (unsigned int i = 0; i < count; ++i)
    {
      in64[i] = (uint64_t) data[i * 2] << 10;
      asm volatile ("" ::: "memory");
    }
  clz_64 (out64, in64, count);
  for (unsigned int i = 0; i < count; ++i)
    if (out64[i] != (data[i * 2] ? data[i * 2 + 1] + 22 : 64))
      abort ();

  return 0;
}
