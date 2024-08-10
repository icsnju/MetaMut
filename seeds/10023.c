/* { dg-do compile } */
/* { dg-require-effective-target arm_arch_v8a_ok } */
/* { dg-options "-O2" } */
/* { dg-add-options arm_arch_v8a } */

int v = 0;

int
atomic_fetch_add_SEQ_CST (int a)
{
  return __atomic_fetch_add (&v, a, __ATOMIC_SEQ_CST);
}

int
atomic_fetch_sub_SEQ_CST (int a)
{
  return __atomic_fetch_sub (&v, a, __ATOMIC_SEQ_CST);
}

int
atomic_fetch_and_SEQ_CST (int a)
{
  return __atomic_fetch_and (&v, a, __ATOMIC_SEQ_CST);
}

int
atomic_fetch_nand_SEQ_CST (int a)
{
  return __atomic_fetch_nand (&v, a, __ATOMIC_SEQ_CST);
}

int
atomic_fetch_xor_SEQ_CST (int a)
{
  return __atomic_fetch_xor (&v, a, __ATOMIC_SEQ_CST);
}

int
atomic_fetch_or_SEQ_CST (int a)
{
  return __atomic_fetch_or (&v, a, __ATOMIC_SEQ_CST);
}

/* { dg-final { scan-assembler-times "ldaex\tr\[0-9\]+, \\\[r\[0-9\]+\\\]" 6 } } */
/* { dg-final { scan-assembler-times "stlex\t...?, r\[0-9\]+, \\\[r\[0-9\]+\\\]" 6 } } */
/* { dg-final { scan-assembler-not "dmb" } } */
