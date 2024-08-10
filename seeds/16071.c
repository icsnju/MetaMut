/* { dg-do compile { target { lp64 && has_arch_pwr9 } } } */
/* { dg-require-effective-target powerpc_vsx_ok } */
/* { dg-options "-O2 -mvsx" } */

/* { dg-final { scan-assembler-times {\mmtvsrdd\M} 1 } } */
/* { dg-final { scan-assembler-times {\mxxlnor\M} 1 } } */
/* { dg-final { scan-assembler-times {\mstxv\M} 1 } } */
/* { dg-final { scan-assembler-not {\mld\M} } } */
/* { dg-final { scan-assembler-not {\mnot\M} } } */

/* Source code for the test in pr92398.h */
/* This test code is included into pr92398.p9-.c and pr92398.p9+.c.
   The two files have the tests for the number of instructions generated for
   P9- versus P9+.

   store generates difference instructions as below:
   P9+: mtvsrdd;xxlnot;stxv.
   P8/P7/P6 LE: not;not;std;std.
   P8 BE: mtvsrd;mtvsrd;xxpermdi;xxlnor;stxvd2x.
   P7/P6 BE: std;std;addi;lxvd2x;xxlnor;stxvd2x.
   P9+ and P9- LE are expected, P6/P7/P8 BE are unexpected.  */

void
bar (__int128_t *dst, __int128_t src)
{
  *dst =  ~src;
}

