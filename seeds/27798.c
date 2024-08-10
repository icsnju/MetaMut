/* { dg-do compile } */
/* { dg-options "-mcmse -mfloat-abi=soft" }  */
/* { dg-skip-if "Incompatible float ABI" { *-*-* } { "-mfloat-abi=*" } { "-mfloat-abi=soft" } } */

extern float bar (void);

float __attribute__ ((cmse_nonsecure_entry))
foo (void)
{
  return bar ();
}

/* { dg-final { scan-assembler "mov\tr1, lr" } } */
/* { dg-final { scan-assembler "mov\tr2, lr" } } */
/* { dg-final { scan-assembler "mov\tr3, lr" } } */
/* { dg-final { scan-assembler "mov\tip, lr" } } */
/* { dg-final { scan-assembler-not "vmov" } } */
/* { dg-final { scan-assembler-not "vmsr" } } */
/* { dg-final { scan-assembler "msr\tAPSR_nzcvq, lr" { target { ! arm_dsp } } } } */
/* { dg-final { scan-assembler "msr\tAPSR_nzcvqg, lr" { target arm_dsp } } } */
/* { dg-final { scan-assembler "bxns" } } */
