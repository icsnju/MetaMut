/* { dg-do compile } */
/* { dg-require-effective-target arm_arch_v8m_base_ok } */
/* { dg-options "-O2" } */
/* { dg-add-options arm_arch_v8m_base } */

int v = 0;

int
atomic_fetch_add_CONSUME (int a)
{
  return __atomic_fetch_add (&v, a, __ATOMIC_CONSUME);
}

int
atomic_fetch_sub_CONSUME (int a)
{
  return __atomic_fetch_sub (&v, a, __ATOMIC_CONSUME);
}

int
atomic_fetch_and_CONSUME (int a)
{
  return __atomic_fetch_and (&v, a, __ATOMIC_CONSUME);
}

int
atomic_fetch_nand_CONSUME (int a)
{
  return __atomic_fetch_nand (&v, a, __ATOMIC_CONSUME);
}

int
atomic_fetch_xor_CONSUME (int a)
{
  return __atomic_fetch_xor (&v, a, __ATOMIC_CONSUME);
}

int
atomic_fetch_or_CONSUME (int a)
{
  return __atomic_fetch_or (&v, a, __ATOMIC_CONSUME);
}

/* Scan for ldaex is a PR59448 consume workaround.  */
/* { dg-final { scan-assembler-times "ldaex\tr\[0-9\]+, \\\[r\[0-9\]+\\\]" 6 } } */
/* { dg-final { scan-assembler-times "strex\t...?, r\[0-9\]+, \\\[r\[0-9\]+\\\]" 6 } } */
/* { dg-final { scan-assembler-not "dmb" } } */
