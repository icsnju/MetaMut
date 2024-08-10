/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "-mcpu=msp430" } { "" } } */
/* { dg-options "-mlarge -mdata-region=either" } */

/* { dg-final { scan-assembler-not "\\.either\\.data" } } */
/* { dg-final { scan-assembler-not "\\.either\\.bss" } } */

__attribute__((persistent)) int persistent_1 = 1;
__attribute__((persistent)) int persistent_2 = 0;
static __attribute__((persistent)) int persistent_3 = 1;
static __attribute__((persistent)) int persistent_4 = 0;

int main (void)
{
  return 0;
}
