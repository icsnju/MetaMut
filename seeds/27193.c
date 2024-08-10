/* { dg-do compile } */
/* { dg-options "-O2 -Wno-attributes -mfpmath=sse -mfma" } */

/* Test that the compiler properly optimizes floating point multiply
   and add instructions into FMA3 instructions.  */

#define TYPE double


#ifndef fma_3
#define fma_3

TYPE __attribute__((sseregparm))
test_noneg_add_noneg_add (TYPE a, TYPE b, TYPE c)
{
  return ((a * b) + c) * b + a;
}

TYPE __attribute__((sseregparm))
test_noneg_add_noneg_sub (TYPE a, TYPE b, TYPE c)
{
  return ((a * b) + c) * b - a;
}

TYPE __attribute__((sseregparm))
test_noneg_add_neg_add (TYPE a, TYPE b, TYPE c)
{
  return -((a * b) + c) * b + a;
}

TYPE __attribute__((sseregparm))
test_noneg_add_neg_sub (TYPE a, TYPE b, TYPE c)
{
  return -((a * b) + c) * b - a;
}

TYPE __attribute__((sseregparm))
test_noneg_sub_noneg_add (TYPE a, TYPE b, TYPE c)
{
  return ((a * b) - c) * b + a;
}

TYPE __attribute__((sseregparm))
test_noneg_sub_noneg_sub (TYPE a, TYPE b, TYPE c)
{
  return ((a * b) - c) * b - a;
}

TYPE __attribute__((sseregparm))
test_noneg_sub_neg_add (TYPE a, TYPE b, TYPE c)
{
  return -((a * b) - c) * b + a;
}

TYPE __attribute__((sseregparm))
test_noneg_sub_neg_sub (TYPE a, TYPE b, TYPE c)
{
  return -((a * b) - c) * b - a;
}

TYPE __attribute__((sseregparm))
test_neg_add_noneg_add (TYPE a, TYPE b, TYPE c)
{
  return (-(a * b) + c) * b + a;
}

TYPE __attribute__((sseregparm))
test_neg_add_noneg_sub (TYPE a, TYPE b, TYPE c)
{
  return (-(a * b) + c) * b - a;
}

TYPE __attribute__((sseregparm))
test_neg_add_neg_add (TYPE a, TYPE b, TYPE c)
{
  return -(-(a * b) + c) * b + a;
}

TYPE __attribute__((sseregparm))
test_neg_add_neg_sub (TYPE a, TYPE b, TYPE c)
{
  return -(-(a * b) + c) * b - a;
}

TYPE __attribute__((sseregparm))
test_neg_sub_noneg_add (TYPE a, TYPE b, TYPE c)
{
  return (-(a * b) - c) * b + a;
}

TYPE __attribute__((sseregparm))
test_neg_sub_noneg_sub (TYPE a, TYPE b, TYPE c)
{
  return (-(a * b) - c) * b - a;
}

TYPE __attribute__((sseregparm))
test_neg_sub_neg_add (TYPE a, TYPE b, TYPE c)
{
  return -(-(a * b) - c) * b + a;
}

TYPE __attribute__((sseregparm))
test_neg_sub_neg_sub (TYPE a, TYPE b, TYPE c)
{
  return -(-(a * b) - c) * b - a;
}

#endif

/* { dg-final { scan-assembler-times "vfmadd\[123\]+sd" 12  } } */
/* { dg-final { scan-assembler-times "vfmsub\[132\]+sd" 12  } } */
/* { dg-final { scan-assembler-times "vfnmadd\[132\]+sd" 4  } } */
/* { dg-final { scan-assembler-times "vfnmsub\[132\]+sd" 4  } } */
