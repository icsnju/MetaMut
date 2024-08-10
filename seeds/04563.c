/* { dg-additional-options "-Wno-address-of-packed-member" } */
/* { dg-excess-errors "warnings about ignored 'packed' attribute" { target default_packed } } */

/* PR c/51628.  */
/* { dg-do compile } */
/* { dg-options "-O" } */

struct A { __complex int i; };
struct B { struct A a; };
struct C { struct B b __attribute__ ((packed)); };
/* { dg-warning "attribute ignored" "" { target default_packed } .-1 } */

extern struct C *p;

int*
foo1 (void)
{
  return &__real(p->b.a.i);
/* { dg-warning "may result in an unaligned pointer value" "" { target { ! default_packed } } .-1 } */
}

int*
foo2 (void)
{
  return &__imag(p->b.a.i);
/* { dg-warning "may result in an unaligned pointer value" "" { target { ! default_packed } } .-1 } */
}
