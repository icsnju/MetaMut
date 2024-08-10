/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize" } */

/* { dg-do assemble { target aarch64_asm_sve_ok } } */
/* { dg-options "-O2 -ftree-vectorize -fdump-tree-vect-details --save-temps" } */

#include <stdint.h>

#if !defined(TYPE)
#define TYPE uint32_t
#endif

#define N 254

/* Non-simple condition reduction.  */

TYPE __attribute__ ((noinline, noclone))
condition_reduction (TYPE *a, TYPE min_v)
{
  TYPE last = 65;

  for (TYPE i = 0; i < N; i++)
    if (a[i] < min_v)
      last = a[i];

  return last;
}

/* { dg-final { scan-tree-dump "operating on partial vectors." "vect" } } */
/* { dg-final { scan-assembler {\tclastb\ts[0-9]+, p[0-7], s[0-9]+, z[0-9]+\.s} } } */

int __attribute__ ((optimize (1)))
main (void)
{
  TYPE a[N] = {
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
    31, 32
  };
  __builtin_memset (a + 32, 43, (N - 32) * sizeof (TYPE));

  TYPE ret = condition_reduction (a, 16);

  if (ret != 10)
    __builtin_abort ();

  return 0;
}
