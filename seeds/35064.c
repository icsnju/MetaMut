/* PR c/62294 */
/* { dg-do compile } */

#pragma GCC system_header
#define foo bar
extern void foo (float *);

void
fn (int *u)
{
  foo (u); /* { dg-error "passing argument 1 of .bar. from incompatible pointer type" } */
}
