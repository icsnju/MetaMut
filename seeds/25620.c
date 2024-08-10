/* PR rtl-optimization/92591 */
/* { dg-do compile } */
/* { dg-options "-Os -fmodulo-sched -fmodulo-sched-allow-regmoves --param sms-dfa-history=8" } */

/* PR tree-optimization/61682 */

int a, b;
static int *c = &b;

int
main ()
{
  int *d = &a;
  for (a = 0; a < 12; a++)
    *c |= *d / 9;

  if (b != 1)
    __builtin_abort ();

  return 0;
}
