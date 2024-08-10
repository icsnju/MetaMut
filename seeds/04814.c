/* { dg-do run } */
/* { dg-options "-O2 -g -ffast-math" } */

/* PR tree-optimization/94589 */
/* { dg-do compile } */
/* { dg-options "-O2 -g0 -ffast-math -fdump-tree-optimized" } */
/* { dg-final { scan-tree-dump-times "\[ij]_\[0-9]+\\(D\\) (?:<|<=|==|!=|>|>=) \[ij]_\[0-9]+\\(D\\)" 14 "optimized" } } */
/* { dg-final { scan-tree-dump-times "i_\[0-9]+\\(D\\) (?:<|<=|==|!=|>|>=) 5\\.0" 14 "optimized" } } */

#define A __attribute__((noipa))
A int f1 (double i, double j) { int c; if (i == j) c = 0; else if (i < j) c = -1; else if (i > j) c = 1; else c = 2; return c == 0; }
A int f2 (double i, double j) { int c; if (i == j) c = 0; else if (i < j) c = -1; else if (i > j) c = 1; else c = 2; return c != 0; }
A int f3 (double i, double j) { int c; if (i == j) c = 0; else if (i < j) c = -1; else if (i > j) c = 1; else c = 2; return c > 0; }
A int f4 (double i, double j) { int c; if (i == j) c = 0; else if (i < j) c = -1; else if (i > j) c = 1; else c = 2; return c < 0; }
A int f5 (double i, double j) { int c; if (i == j) c = 0; else if (i < j) c = -1; else if (i > j) c = 1; else c = 2; return c >= 0; }
A int f6 (double i, double j) { int c; if (i == j) c = 0; else if (i < j) c = -1; else if (i > j) c = 1; else c = 2; return c <= 0; }
A int f7 (double i, double j) { int c; if (i == j) c = 0; else if (i < j) c = -1; else if (i > j) c = 1; else c = 2; return c == -1; }
A int f8 (double i, double j) { int c; if (i == j) c = 0; else if (i < j) c = -1; else if (i > j) c = 1; else c = 2; return c != -1; }
A int f9 (double i, double j) { int c; if (i == j) c = 0; else if (i < j) c = -1; else if (i > j) c = 1; else c = 2; return c > -1; }
A int f10 (double i, double j) { int c; if (i == j) c = 0; else if (i < j) c = -1; else if (i > j) c = 1; else c = 2; return c <= -1; }
A int f11 (double i, double j) { int c; if (i == j) c = 0; else if (i < j) c = -1; else if (i > j) c = 1; else c = 2; return c == 1; }
A int f12 (double i, double j) { int c; if (i == j) c = 0; else if (i < j) c = -1; else if (i > j) c = 1; else c = 2; return c != 1; }
A int f13 (double i, double j) { int c; if (i == j) c = 0; else if (i < j) c = -1; else if (i > j) c = 1; else c = 2; return c < 1; }
A int f14 (double i, double j) { int c; if (i == j) c = 0; else if (i < j) c = -1; else if (i > j) c = 1; else c = 2; return c >= 1; }
A int f15 (double i) { int c; if (i == 5.0) c = 0; else if (i < 5.0) c = -1; else if (i > 5.0) c = 1; else c = 2; return c == 0; }
A int f16 (double i) { int c; if (i == 5.0) c = 0; else if (i < 5.0) c = -1; else if (i > 5.0) c = 1; else c = 2; return c != 0; }
A int f17 (double i) { int c; if (i == 5.0) c = 0; else if (i < 5.0) c = -1; else if (i > 5.0) c = 1; else c = 2; return c > 0; }
A int f18 (double i) { int c; if (i == 5.0) c = 0; else if (i < 5.0) c = -1; else if (i > 5.0) c = 1; else c = 2; return c < 0; }
A int f19 (double i) { int c; if (i == 5.0) c = 0; else if (i < 5.0) c = -1; else if (i > 5.0) c = 1; else c = 2; return c >= 0; }
A int f20 (double i) { int c; if (i == 5.0) c = 0; else if (i < 5.0) c = -1; else if (i > 5.0) c = 1; else c = 2; return c <= 0; }
A int f21 (double i) { int c; if (i == 5.0) c = 0; else if (i < 5.0) c = -1; else if (i > 5.0) c = 1; else c = 2; return c == -1; }
A int f22 (double i) { int c; if (i == 5.0) c = 0; else if (i < 5.0) c = -1; else if (i > 5.0) c = 1; else c = 2; return c != -1; }
A int f23 (double i) { int c; if (i == 5.0) c = 0; else if (i < 5.0) c = -1; else if (i > 5.0) c = 1; else c = 2; return c > -1; }
A int f24 (double i) { int c; if (i == 5.0) c = 0; else if (i < 5.0) c = -1; else if (i > 5.0) c = 1; else c = 2; return c <= -1; }
A int f25 (double i) { int c; if (i == 5.0) c = 0; else if (i < 5.0) c = -1; else if (i > 5.0) c = 1; else c = 2; return c == 1; }
A int f26 (double i) { int c; if (i == 5.0) c = 0; else if (i < 5.0) c = -1; else if (i > 5.0) c = 1; else c = 2; return c != 1; }
A int f27 (double i) { int c; if (i == 5.0) c = 0; else if (i < 5.0) c = -1; else if (i > 5.0) c = 1; else c = 2; return c < 1; }
A int f28 (double i) { int c; if (i == 5.0) c = 0; else if (i < 5.0) c = -1; else if (i > 5.0) c = 1; else c = 2; return c >= 1; }

#define C(fn, i, j, r) if (fn (i, j) != r) __builtin_abort ()
#define D(fn, i, r) if (fn (i) != r) __builtin_abort ()

int
main ()
{
  C (f1, 7.0, 8.0, 0);
  C (f1, 8.0, 8.0, 1);
  C (f1, 9.0, 8.0, 0);
  C (f2, 7.0, 8.0, 1);
  C (f2, 8.0, 8.0, 0);
  C (f2, 9.0, 8.0, 1);
  C (f3, 7.0, 8.0, 0);
  C (f3, 8.0, 8.0, 0);
  C (f3, 9.0, 8.0, 1);
  C (f4, 7.0, 8.0, 1);
  C (f4, 8.0, 8.0, 0);
  C (f4, 9.0, 8.0, 0);
  C (f5, 7.0, 8.0, 0);
  C (f5, 8.0, 8.0, 1);
  C (f5, 9.0, 8.0, 1);
  C (f6, 7.0, 8.0, 1);
  C (f6, 8.0, 8.0, 1);
  C (f6, 9.0, 8.0, 0);
  C (f7, 7.0, 8.0, 1);
  C (f7, 8.0, 8.0, 0);
  C (f7, 9.0, 8.0, 0);
  C (f8, 7.0, 8.0, 0);
  C (f8, 8.0, 8.0, 1);
  C (f8, 9.0, 8.0, 1);
  C (f9, 7.0, 8.0, 0);
  C (f9, 8.0, 8.0, 1);
  C (f9, 9.0, 8.0, 1);
  C (f10, 7.0, 8.0, 1);
  C (f10, 8.0, 8.0, 0);
  C (f10, 9.0, 8.0, 0);
  C (f11, 7.0, 8.0, 0);
  C (f11, 8.0, 8.0, 0);
  C (f11, 9.0, 8.0, 1);
  C (f12, 7.0, 8.0, 1);
  C (f12, 8.0, 8.0, 1);
  C (f12, 9.0, 8.0, 0);
  C (f13, 7.0, 8.0, 1);
  C (f13, 8.0, 8.0, 1);
  C (f13, 9.0, 8.0, 0);
  C (f14, 7.0, 8.0, 0);
  C (f14, 8.0, 8.0, 0);
  C (f14, 9.0, 8.0, 1);
  D (f15, 4.0, 0);
  D (f15, 5.0, 1);
  D (f15, 6.0, 0);
  D (f16, 4.0, 1);
  D (f16, 5.0, 0);
  D (f16, 6.0, 1);
  D (f17, 4.0, 0);
  D (f17, 5.0, 0);
  D (f17, 6.0, 1);
  D (f18, 4.0, 1);
  D (f18, 5.0, 0);
  D (f18, 6.0, 0);
  D (f19, 4.0, 0);
  D (f19, 5.0, 1);
  D (f19, 6.0, 1);
  D (f20, 4.0, 1);
  D (f20, 5.0, 1);
  D (f20, 6.0, 0);
  D (f21, 4.0, 1);
  D (f21, 5.0, 0);
  D (f21, 6.0, 0);
  D (f22, 4.0, 0);
  D (f22, 5.0, 1);
  D (f22, 6.0, 1);
  D (f23, 4.0, 0);
  D (f23, 5.0, 1);
  D (f23, 6.0, 1);
  D (f24, 4.0, 1);
  D (f24, 5.0, 0);
  D (f24, 6.0, 0);
  D (f25, 4.0, 0);
  D (f25, 5.0, 0);
  D (f25, 6.0, 1);
  D (f26, 4.0, 1);
  D (f26, 5.0, 1);
  D (f26, 6.0, 0);
  D (f27, 4.0, 1);
  D (f27, 5.0, 1);
  D (f27, 6.0, 0);
  D (f28, 4.0, 0);
  D (f28, 5.0, 0);
  D (f28, 6.0, 1);
  return 0;
}
