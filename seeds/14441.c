/* { dg-do compile } */
/* { dg-options "-mcmse -mfloat-abi=softfp -mfpu=fpv5-sp-d16" }  */
/* { dg-skip-if "Incompatible float ABI" { *-*-* } { "-mfloat-abi=*" } { "-mfloat-abi=softfp" } } */
/* { dg-skip-if "Skip these if testing double precision" {*-*-*} {"-mfpu=fpv[4-5]-d16"} {""} } */

int __attribute__ ((cmse_nonsecure_call)) (*bar) (double);

int
foo (int a)
{
  return bar (2.0) + a + 1;
}

/* Checks for saving and clearing prior to function call.  */
/* { dg-final { scan-assembler "lsrs\tr4, r4, #1" } } */
/* { dg-final { scan-assembler "lsls\tr4, r4, #1" } } */
/* { dg-final { scan-assembler-not "mov\tr0, r4" } } */
/* { dg-final { scan-assembler-not "mov\tr1, r4" } } */
/* { dg-final { scan-assembler "mov\tr2, r4" } } */
/* { dg-final { scan-assembler "mov\tr3, r4" } } */

/* Now we check that we use the correct intrinsic to call.  */
/* { dg-final { scan-assembler "bl\t__gnu_cmse_nonsecure_call" } } */
