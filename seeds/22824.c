/* { dg-do link } */
/* { dg-do run { target runtime_ptx_alias } } */
/* { dg-options "-save-temps -O2" } */
/* { dg-add-options ptx_alias } */

/* { dg-do link } */
/* { dg-do run { target runtime_ptx_alias } } */
/* { dg-options "-save-temps" } */
/* { dg-add-options ptx_alias } */

int v;

void __f ()
{
  v = 1;
}

void f () __attribute__ ((alias ("__f")));

int
main (void)
{
  if (v != 0)
    __builtin_abort ();
  f ();
  if (v != 1)
    __builtin_abort ();
  return 0;
}

/* { dg-final { scan-assembler-times "\\.alias f,__f;" 1 } } */
/* { dg-final { scan-assembler-times "\\.visible \\.func __f;" 1 } } */
/* { dg-final { scan-assembler-times "\\.visible \\.func f;" 1 } } */

/* Inlined, so no alias.  */
/* { dg-final { scan-assembler-not "\\.alias.*;" } } */
/* { dg-final { scan-assembler-not "\\.visible \\.func f;" } } */

/* Note static and inlined, so still there.  */
/* { dg-final { scan-assembler-times "\\.visible \\.func __f;" 1 } } */

