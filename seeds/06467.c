/* PR tree-optimization/81588 */
/* { dg-do run } */
/* { dg-options "-O2" } */

long long int a = 5011877430933453486LL, c = 1;
unsigned short b = 24847;

/* PR tree-optimization/81588 */
/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-reassoc1-details" } */
 	
extern long long int a, c;
extern unsigned short b;

/* { dg-final { scan-tree-dump-times "Optimizing range test \[^\n\r]* and comparison" 1 "reassoc1" } } */

__attribute__((noinline, noclone)) void
foo (void)
{
  if ((b > a) != (1 + (a < 0)))
    c = 0;
}

int
main ()
{
  foo ();
  if (c != 0)
    __builtin_abort ();
  a = 24846;
  c = 1;
  foo ();
  if (c != 1)
    __builtin_abort ();
  a = -5;
  foo ();
  if (c != 0)
    __builtin_abort ();
  return 0;
}
