/* { dg-do compile } */
/* Verify that compare exchange mappings match Table A.6's recommended mapping.  */
/* { dg-final { scan-assembler-times "lr.w.aq\t" 1 } } */
/* { dg-final { scan-assembler-times "sc.w\t" 1 } } */

void foo (int bar, int baz, int qux)
{
  __atomic_compare_exchange_n(&bar, &baz, qux, 1, __ATOMIC_CONSUME, __ATOMIC_CONSUME);
}
