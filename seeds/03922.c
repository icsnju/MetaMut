/* { dg-do compile } */
/* { dg-options "-pedantic-errors" } */
#pragma GCC system_header
int sc = (&sc >= 0);


void foo(void)
{
  int s = sc;
}
