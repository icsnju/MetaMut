
/* { dg-do run } */
/* { dg-options "-O3 -ffast-math" } */

extern void abort (void);


typedef float *__restrict__ pRF32;
typedef double *__restrict__ pRF64;

float addv_f32 (pRF32 a)
{
  int i;
  float s = 0.0;
  for (i=0; i<16; i++)
    s += a[i];

  return s;
}

double addv_f64 (pRF64 a)
{
  int i;
  double  s = 0.0;
  for (i=0; i<16; i++)
    s += a[i];

  return s;
}

int main (void)
{
  float addv_f32_value = -120.0f;
  double addv_f64_value = 120.0;
  float af32[16];
  double af64[16];
  int i;

  /* Set up input vectors.  */
  for (i=0; i<16; i++)
    {
      af32[i] = (float)-i;
      af64[i] = (double)i;
    }

  if (addv_f32 (af32) != addv_f32_value)
    abort ();

  if (addv_f64 (af64) != addv_f64_value)
    abort ();

  return 0;
}
