/* PR target/91931 */
/* { dg-do compile } */
/* { dg-options "-O1 -ftree-loop-vectorize -mavx -mno-avx2" } */

/* { dg-do compile } */
/* { dg-require-effective-target vect_floatint_cvt } */

unsigned int wlookup2[203];

void
SetSoundVariables (int x)
{
  for (x = 1; x < 32; x++)
  {
    wlookup2[x] = (double) 16 / x;
  }
}

