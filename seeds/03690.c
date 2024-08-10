/* Check that we get the expected alignment-checking code, op variant, short.  */
/* { dg-do compile } */
/* { dg-options "-O2 -Dop -Dtype=short" } */
/* { dg-additional-options "-mtrap-using-break8 -mtrap-unaligned-atomic" { target cris-*-elf } } */
/* { dg-final { scan-assembler "\tbreak 8" } } */
/* { dg-final { scan-assembler "\tbtstq \\(1-1\\)," } } */
/* { dg-final { scan-assembler-not "\tand" } } */
/* { dg-final { scan-assembler-not "\t\[jb\]sr" } } */
/* Check that we can assemble both base atomic variants, for v0.  */
/* { dg-do assemble } */
/* { dg-options "-O2 -march=v0" { target { ! march_option } } } */

#ifndef type
#define type char
#endif

#if !defined(op) && !defined(xchg)
#define op 1
#define xchg 1
#endif

#ifndef op
#define op 0
#endif

#ifndef xchg
#define xchg 0
#endif

#if op
int sfa (type *p, type *q, int a)
{
  return __atomic_fetch_nand (p, a, __ATOMIC_ACQ_REL)
    + __atomic_fetch_add (q, a, __ATOMIC_SEQ_CST);
}
#endif

#if xchg
void acen (type *ptr, type *val, type *ret)
{
  __atomic_exchange (ptr, val, ret, __ATOMIC_SEQ_CST);
}
#endif
