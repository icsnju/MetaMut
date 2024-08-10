/* { dg-do compile } */
/* { dg-options "-pedantic-errors" } */
int sc = (&sc >= 0);

void foo(void)
{
  int s = sc;
}
/* { dg-message "file included" "In file included" { target *-*-* } 0 } */
/* { dg-error "ordered comparison of pointer with integer zero" "pedantic error" { target *-*-* } 0 } */
