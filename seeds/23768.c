/* { dg-do compile } */
/* { dg-options "-O3 -mzarch -ffast-math" } */

/* Fast-math implies -fno-trapping-math -fno-signaling-nans which imply
   that no user visible trap will happen.  */

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

/* { dg-final { scan-assembler-times "ltdbr\t" 12 } } */
