/* { dg-do compile } */
/* { dg-require-effective-target arm_arch_v8m_base_ok } */
/* { dg-options "-O2 -fno-ipa-icf" } */
/* { dg-add-options arm_arch_v8m_base } */


#define STRONG 0
#define WEAK 1
int v = 0;

int
atomic_compare_exchange_STRONG_RELEASE_ACQUIRE (int a, int b)
{
  return __atomic_compare_exchange (&v, &a, &b,
				    STRONG, __ATOMIC_RELEASE,
				    __ATOMIC_ACQUIRE);
}

int
atomic_compare_exchange_WEAK_RELEASE_ACQUIRE (int a, int b)
{
  return __atomic_compare_exchange (&v, &a, &b,
				    WEAK, __ATOMIC_RELEASE,
				    __ATOMIC_ACQUIRE);
}

int
atomic_compare_exchange_n_STRONG_RELEASE_ACQUIRE (int a, int b)
{
  return __atomic_compare_exchange_n (&v, &a, b,
				      STRONG, __ATOMIC_RELEASE,
				      __ATOMIC_ACQUIRE);
}

int
atomic_compare_exchange_n_WEAK_RELEASE_ACQUIRE (int a, int b)
{
  return __atomic_compare_exchange_n (&v, &a, b,
				      WEAK, __ATOMIC_RELEASE,
				      __ATOMIC_ACQUIRE);
}

/* { dg-final { scan-assembler-times "ldaex" 4 } } */
/* { dg-final { scan-assembler-times "stlex" 4 } } */
/* { dg-final { scan-assembler-not "dmb" } } */
