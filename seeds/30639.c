/* { dg-do compile { target { s390-*-* } } } */
/* { dg-options "-foptimize-sibling-calls -mesa" } */
/* { dg-final { scan-assembler {brasl\t%r\d+,bar} } } */

/* This tests function s390_call_saved_register_used where
   REG_P (parm_rtx) and nregs == 2 holds.  */

/* For the S/390 ABI parameter D is passed in GPR 5 and 6 and the latter is
   call-saved which prohibits sibling call optimization.  This must hold true
   also if the mode of the parameter is BLKmode.  */

typedef struct
{
  short x;
  char y[6];
} t;

extern t d;

extern void bar (int a, int b, int c, t d);

void foo (int a, int b, int c)
{
  bar (a, b, c, d);
}
