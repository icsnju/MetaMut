/* { dg-do assemble } */
/* { dg-options "-O2 -save-temps" } */

typedef int __v2si __attribute__((__vector_size__(8)));

#define TYPE __v2si
TYPE __attribute__((noinline, noclone))
vector_load (TYPE *p)
{
  return *p;
}

void __attribute__((noinline, noclone))
vector_store (TYPE *p, TYPE val)
{
  *p = val;
}

TYPE __attribute__((noinline, noclone))
vector_const ()
{
  TYPE res = {1, 2};
  return res;
}

/* { dg-final { scan-assembler ".reg\\.v2\\.u32" } } */
/* { dg-final { scan-assembler "ld\\.v2\\.u32" } } */
/* { dg-final { scan-assembler "st\\.v2\\.u32" } } */
/* { dg-final { scan-assembler "(?n)mov\\.v2\\.u32.*\\{ 1, 2 \\}" } } */
