/* { dg-do assemble } */
/* { dg-options "-O2 -save-temps" } */

typedef long long int __v2di __attribute__((__vector_size__(16)));

#define TYPE __v2di
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

/* { dg-final { scan-assembler ".reg\\.v2\\.u64" } } */
/* { dg-final { scan-assembler "ld\\.v2\\.u64" } } */
/* { dg-final { scan-assembler "st\\.v2\\.u64" } } */
/* { dg-final { scan-assembler "mov\\.v2\\.u64.*\\{ 1, 2 \\}" } } */
