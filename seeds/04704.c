/* { dg-do compile } */
/* { dg-options "-fdata-sections" } */

/* { dg-final { scan-assembler-not "\\.data" } } */
/* { dg-final { scan-assembler-not "\\.bss" } } */

__attribute__((persistent)) int persistent_1 = 1;
__attribute__((persistent)) int persistent_2 = 0;
static __attribute__((persistent)) int persistent_3 = 1;
static __attribute__((persistent)) int persistent_4 = 0;

int main (void)
{
  return 0;
}
