/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -fplt -fno-pic -mno-direct-extern-access" } */

extern void foo (void);

void
bar (void)
{
  foo ();
}

/* { dg-final { scan-assembler "jmp\[ \t\]*foo" } } */
/* { dg-final { scan-assembler-not "foo@GOTPCREL" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-not "foo@GOT" { target ia32 } } } */
/* { dg-final { scan-assembler "\.section\[ \t]+.note.gnu.property," } } */
/* { dg-final { scan-assembler "\.long\[ \t]+0xb0008000" } } */

