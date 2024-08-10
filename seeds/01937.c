/* { dg-do compile } */
/* { dg-options "-O3 -Wno-attributes -mfpmath=sse -mfma -mtune=generic -mno-fma4 -mno-avx512f" } */
/* Disabling epilogues until we find a better way to deal with scans.  */
/* { dg-additional-options "--param vect-epilogues-nomask=0" } */

/* Test that the compiler properly optimizes floating point multiply
   and add instructions into FMA3 instructions.  */

#define TYPE float


#ifndef l_fma_6
#define l_fma_6

void __attribute__((sseregparm))
test_noneg_add_noneg_add (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = ((a[i] * b[i]) + c[i]) * c[i] + b[i];
}

void __attribute__((sseregparm))
test_noneg_add_noneg_sub (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = ((a[i] * b[i]) + c[i]) * c[i] - b[i];
}

void __attribute__((sseregparm))
test_noneg_add_neg_add (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = -((a[i] * b[i]) + c[i]) * c[i] + b[i];
}

void __attribute__((sseregparm))
test_noneg_add_neg_sub (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = -((a[i] * b[i]) + c[i]) * c[i] - b[i];
}

void __attribute__((sseregparm))
test_noneg_sub_noneg_add (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = ((a[i] * b[i]) - c[i]) * c[i] + b[i];
}

void __attribute__((sseregparm))
test_noneg_sub_noneg_sub (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = ((a[i] * b[i]) - c[i]) * c[i] - b[i];
}

void __attribute__((sseregparm))
test_noneg_sub_neg_add (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = -((a[i] * b[i]) - c[i]) * c[i] + b[i];
}

void __attribute__((sseregparm))
test_noneg_sub_neg_sub (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = -((a[i] * b[i]) - c[i]) * c[i] - b[i];
}

void __attribute__((sseregparm))
test_neg_add_noneg_add (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = (-(a[i] * b[i]) + c[i]) * c[i] + b[i];
}

void __attribute__((sseregparm))
test_neg_add_noneg_sub (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = (-(a[i] * b[i]) + c[i]) * c[i] - b[i];
}

void __attribute__((sseregparm))
test_neg_add_neg_add (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = -(-(a[i] * b[i]) + c[i]) * c[i] + b[i];
}

void __attribute__((sseregparm))
test_neg_add_neg_sub (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = -(-(a[i] * b[i]) + c[i]) * c[i] - b[i];
}

void __attribute__((sseregparm))
test_neg_sub_noneg_add (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = (-(a[i] * b[i]) - c[i]) * c[i] + b[i];
}

void __attribute__((sseregparm))
test_neg_sub_noneg_sub (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = (-(a[i] * b[i]) - c[i]) * c[i] - b[i];
}

void __attribute__((sseregparm))
test_neg_sub_neg_add (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = -(-(a[i] * b[i]) - c[i]) * c[i] + b[i];
}

void __attribute__((sseregparm))
test_neg_sub_neg_sub (TYPE *a, TYPE *b, TYPE *c, TYPE *d, int n)
{
  int i;
  for (i = 0; i < n; i++)
    d[i] = -(-(a[i] * b[i]) - c[i]) * c[i] - b[i];
}

#endif

/* { dg-final { scan-assembler-times "vfmadd\[123\]+ps" 12 } } */
/* { dg-final { scan-assembler-times "vfmsub\[123\]+ps" 12 } } */
/* { dg-final { scan-assembler-times "vfnmadd\[123\]+ps" 4 } } */
/* { dg-final { scan-assembler-times "vfnmsub\[123\]+ps" 4 } } */
/* { dg-final { scan-assembler-times "vfmadd\[123\]+ss" 96 } } */
/* { dg-final { scan-assembler-times "vfmsub\[123\]+ss" 96 } } */
/* { dg-final { scan-assembler-times "vfnmadd\[123\]+ss" 32 } } */
/* { dg-final { scan-assembler-times "vfnmsub\[123\]+ss" 32 } } */