/* { dg-do compile } */
/* { dg-options "-O3 -mzarch -march=z196" } */

/* Use load-and-test instructions if compared for (in)equality and if variable
   `a` is dead after the comparison.  For all other cases use
   compare-and-signal instructions.  */

double gl;

#define test(N, CMP) \
  void   N ## _dead(double a) { if (a CMP 0.0) gl = 1; } \
  double N ## _live(double a) { if (a CMP 0.0) gl = 1; return a; }

test(eq, ==)
test(ne, !=)
test(ge, >=)
test(gt, >)
test(le, <=)
test(lt, <)

/* { dg-final { scan-assembler-times "ltdbr\t" 2 } } */
/* { dg-final { scan-assembler-times "cdbr\t" 2 } } */
/* { dg-final { scan-assembler-times "kdbr\t" 8 } } */
