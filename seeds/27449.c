/* { dg-do compile } */
/* { dg-options "-pedantic-errors -w" } */
int sc = (&sc >= 0);

void foo(void)
{
  int s = sc;
}
