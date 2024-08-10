/* { dg-do compile } */
/* { dg-skip-if "avoid conflicts with multilib options" { ! arm_thumb2_ok } { "-march=*" } { "" } } */
/* { dg-skip-if "avoid conflicts with multilib options" { *-*-* } { "-mcpu=*" } { "-mcpu=cortex-m4" } } */
/* { dg-skip-if "avoid conflicts with multilib options" { *-*-* } { "-mfpu=*" } { "-mfpu=fpv4-sp-d16" } } */
/* { dg-skip-if "avoid conflicts with multilib options" { *-*-* } { "-mfloat-abi=*" } { "-mfloat-abi=hard" } } */
/* { dg-options "-O2 -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -mfloat-abi=hard" } */

extern double fma (double, double, double);
extern float fmaf (float, float, float);

float
vfma32 (float x, float y, float z)
{
  return fmaf (x, y, z);
}

float
vfms32 (float x, float y, float z)
{
  return fmaf (-x, y, z);
}

float
vfnms32 (float x, float y, float z)
{
  return fmaf (x, y, -z);
}

float
vfnma32 (float x, float y, float z)
{
  return fmaf (-x, y, -z);
}

double
vfma64 (double x, double y, double z)
{
  return fma (x, y, z);
}

double
vfms64 (double x, double y, double z)
{
  return fma (-x, y, z);
}

double
vfnms64 (double x, double y, double z)
{
  return fma (x, y, -z);
}

double
vfnma64 (double x, double y, double z)
{
  return fma (-x, y, -z);
}

/* { dg-final { scan-assembler-not "vfma\.f64\td\[0-9\]" } } */
/* { dg-final { scan-assembler-times "vfma\.f32\ts\[0-9\]" 1 } } */
/* { dg-final { scan-assembler-not "vfms\.f64\td\[0-9\]" } } */
/* { dg-final { scan-assembler-times "vfms\.f32\ts\[0-9\]" 1 } } */
/* { dg-final { scan-assembler-not "vfnma\.f64\td\[0-9\]" } } */
/* { dg-final { scan-assembler-times "vfnma\.f32\ts\[0-9\]" 1 } } */
/* { dg-final { scan-assembler-not "vfnms\.f64\td\[0-9\]" } } */
/* { dg-final { scan-assembler-times "vfnms\.f32\ts\[0-9\]" 1 } } */
