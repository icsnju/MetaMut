/* PR target/79570 */
/* { dg-do compile { target powerpc*-*-* ia64-*-* i?86-*-* x86_64-*-* } } */
/* { dg-options "-O2 -fselective-scheduling2 -fvar-tracking-assignments -g" } */
/* { dg-warning "changes selective scheduling" "" { target *-*-* } 0 } */

/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize" } */
/* { dg-additional-options "-march=skylake-avx512" { target { i?86-*-* x86_64-*-* } } } */

void
fn1 (char *b, char *d, int *c, int i)
{
  for (; i; i++, d++)
    if (b[i])
      *d = c[i];
}
