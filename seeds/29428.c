/* PR middle-end/30314 */

/* PR middle-end/30314 */
/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */
/* { dg-final { scan-tree-dump-not "\.MUL_OVERFLOW " "optimized" } } */
/* { dg-final { scan-tree-dump " > 122713351" "optimized" { target int32 } } } */
/* { dg-final { scan-tree-dump " > 527049830677415760" "optimized" { target lp64 } } } */
/* { dg-final { scan-tree-dump " > 102261126" "optimized" { target int32 } } } */
/* { dg-final { scan-tree-dump " > 439208192231179800" "optimized" { target lp64 } } } */

__attribute__((noipa)) int
foo (unsigned int x)
{
  return __builtin_mul_overflow_p (x, 35U, 0U);
}

__attribute__((noipa)) int
bar (unsigned long int x)
{
  return __builtin_mul_overflow_p (x, 35UL, 0UL);
}

__attribute__((noipa)) int
baz (unsigned int x)
{
  return __builtin_mul_overflow_p (42, x, 0U);
}

__attribute__((noipa)) int
qux (unsigned long int x)
{
  return __builtin_mul_overflow_p (42, x, 0UL);
}

int
main ()
{
  if (foo (0) != 0
      || foo (~0U / 35) != 0
      || foo (~0U / 35 + 1) != 1
      || foo (~0U) != 1)
    __builtin_abort ();
  if (bar (0) != 0
      || bar (~0UL / 35) != 0
      || bar (~0UL / 35 + 1) != 1
      || bar (~0UL) != 1)
    __builtin_abort ();
  if (baz (0) != 0
      || baz (~0U / 42) != 0
      || baz (~0U / 42 + 1) != 1
      || baz (~0U) != 1)
    __builtin_abort ();
  if (qux (0) != 0
      || qux (~0UL / 42) != 0
      || qux (~0UL / 42 + 1) != 1
      || qux (~0UL) != 1)
    __builtin_abort ();
  return 0;
}
