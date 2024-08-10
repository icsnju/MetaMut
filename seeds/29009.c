/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do assemble { target aarch64_asm_sve_ok } } */
/* { dg-options "-O3 --save-temps" } */

#include <stdint.h>

void __attribute__ ((noinline, noclone))
vcvtf_16 (_Float16 *dst, uint16_t *src1, int size)
{
  for (int i = 0; i < size; i++)
    dst[i] = (_Float16) src1[i];
}

void __attribute__ ((noinline, noclone))
vcvtf_32 (float *dst, uint32_t *src1, int size)
{
  for (int i = 0; i < size; i++)
    dst[i] = (float) src1[i];
}

void __attribute__ ((noinline, noclone))
vcvtf_64 (double *dst, uint64_t *src1, int size)
{
  for (int i = 0; i < size; i++)
    dst[i] = (double) src1[i];
}

/* { dg-final { scan-assembler-times {\tucvtf\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\tucvtf\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tucvtf\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d\n} 1 } } */

#define ARRAY_SIZE 65

#define VAL1 (i * 109)
#define VAL2 (i * 9456)
#define VAL3 (i * 0xfddff13f)

int __attribute__ ((optimize (1)))
main (void)
{
  static _Float16 array_dest16[ARRAY_SIZE];
  static float array_dest32[ARRAY_SIZE];
  static double array_dest64[ARRAY_SIZE];

  uint16_t array_source16[ARRAY_SIZE];
  uint32_t array_source32[ARRAY_SIZE];
  uint64_t array_source64[ARRAY_SIZE];

  for (int i = 0; i < ARRAY_SIZE; i++)
    {
      array_source16[i] = VAL1;
      array_source32[i] = VAL2;
      array_source64[i] = VAL3;
      asm volatile ("" ::: "memory");
    }

  vcvtf_16 (array_dest16, array_source16, ARRAY_SIZE);
  for (int i = 0; i < ARRAY_SIZE; i++)
    if (array_dest16[i] != (_Float16) VAL1)
      __builtin_abort ();

  vcvtf_32 (array_dest32, array_source32, ARRAY_SIZE);
  for (int i = 0; i < ARRAY_SIZE; i++)
    if (array_dest32[i] != (float) VAL2)
      __builtin_abort ();

  vcvtf_64 (array_dest64, array_source64, ARRAY_SIZE);
  for (int i = 0; i < ARRAY_SIZE; i++)
    if (array_dest64[i] != (double) VAL3)
      __builtin_abort ();

  return 0;
}
