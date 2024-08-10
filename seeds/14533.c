/* { dg-do run } */
/* { dg-options "-O2 -g" } */

/* PR tree-optimization/94589 */
/* { dg-do compile } */
/* { dg-options "-O2 -g0 -fdump-tree-optimized" } */
/* { dg-final { scan-tree-dump-times "\[ij]_\[0-9]+\\(D\\) (?:<|<=|==|!=|>|>=) \[ij]_\[0-9]+\\(D\\)" 14 "optimized" } } */
/* { dg-final { scan-tree-dump-times "i_\[0-9]+\\(D\\) (?:<|<=|==|!=|>|>=) \[45]" 14 "optimized" } } */

#define A __attribute__((noipa))
A int f1 (int i, int j) { int c = i == j ? 0 : i < j ? -1 : 1; return c == 0; }
A int f2 (int i, int j) { int c = i == j ? 0 : i < j ? -1 : 1; return c != 0; }
A int f3 (int i, int j) { int c = i == j ? 0 : i < j ? -1 : 1; return c > 0; }
A int f4 (int i, int j) { int c = i == j ? 0 : i < j ? -1 : 1; return c < 0; }
A int f5 (int i, int j) { int c = i == j ? 0 : i < j ? -1 : 1; return c >= 0; }
A int f6 (int i, int j) { int c = i == j ? 0 : i < j ? -1 : 1; return c <= 0; }
A int f7 (int i, int j) { int c = i == j ? 0 : i < j ? -1 : 1; return c == -1; }
A int f8 (int i, int j) { int c = i == j ? 0 : i < j ? -1 : 1; return c != -1; }
A int f9 (int i, int j) { int c = i == j ? 0 : i < j ? -1 : 1; return c > -1; }
A int f10 (int i, int j) { int c = i == j ? 0 : i < j ? -1 : 1; return c <= -1; }
A int f11 (int i, int j) { int c = i == j ? 0 : i < j ? -1 : 1; return c == 1; }
A int f12 (int i, int j) { int c = i == j ? 0 : i < j ? -1 : 1; return c != 1; }
A int f13 (int i, int j) { int c = i == j ? 0 : i < j ? -1 : 1; return c < 1; }
A int f14 (int i, int j) { int c = i == j ? 0 : i < j ? -1 : 1; return c >= 1; }
A int f15 (int i) { int c = i == 5 ? 0 : i < 5 ? -1 : 1; return c == 0; }
A int f16 (int i) { int c = i == 5 ? 0 : i < 5 ? -1 : 1; return c != 0; }
A int f17 (int i) { int c = i == 5 ? 0 : i < 5 ? -1 : 1; return c > 0; }
A int f18 (int i) { int c = i == 5 ? 0 : i < 5 ? -1 : 1; return c < 0; }
A int f19 (int i) { int c = i == 5 ? 0 : i < 5 ? -1 : 1; return c >= 0; }
A int f20 (int i) { int c = i == 5 ? 0 : i < 5 ? -1 : 1; return c <= 0; }
A int f21 (int i) { int c = i == 5 ? 0 : i < 5 ? -1 : 1; return c == -1; }
A int f22 (int i) { int c = i == 5 ? 0 : i < 5 ? -1 : 1; return c != -1; }
A int f23 (int i) { int c = i == 5 ? 0 : i < 5 ? -1 : 1; return c > -1; }
A int f24 (int i) { int c = i == 5 ? 0 : i < 5 ? -1 : 1; return c <= -1; }
A int f25 (int i) { int c = i == 5 ? 0 : i < 5 ? -1 : 1; return c == 1; }
A int f26 (int i) { int c = i == 5 ? 0 : i < 5 ? -1 : 1; return c != 1; }
A int f27 (int i) { int c = i == 5 ? 0 : i < 5 ? -1 : 1; return c < 1; }
A int f28 (int i) { int c = i == 5 ? 0 : i < 5 ? -1 : 1; return c >= 1; }

#define C(fn, i, j, r) if (fn (i, j) != r) __builtin_abort ()
#define D(fn, i, r) if (fn (i) != r) __builtin_abort ()

int
main ()
{
  C (f1, 7, 8, 0);
  C (f1, 8, 8, 1);
  C (f1, 9, 8, 0);
  C (f2, 7, 8, 1);
  C (f2, 8, 8, 0);
  C (f2, 9, 8, 1);
  C (f3, 7, 8, 0);
  C (f3, 8, 8, 0);
  C (f3, 9, 8, 1);
  C (f4, 7, 8, 1);
  C (f4, 8, 8, 0);
  C (f4, 9, 8, 0);
  C (f5, 7, 8, 0);
  C (f5, 8, 8, 1);
  C (f5, 9, 8, 1);
  C (f6, 7, 8, 1);
  C (f6, 8, 8, 1);
  C (f6, 9, 8, 0);
  C (f7, 7, 8, 1);
  C (f7, 8, 8, 0);
  C (f7, 9, 8, 0);
  C (f8, 7, 8, 0);
  C (f8, 8, 8, 1);
  C (f8, 9, 8, 1);
  C (f9, 7, 8, 0);
  C (f9, 8, 8, 1);
  C (f9, 9, 8, 1);
  C (f10, 7, 8, 1);
  C (f10, 8, 8, 0);
  C (f10, 9, 8, 0);
  C (f11, 7, 8, 0);
  C (f11, 8, 8, 0);
  C (f11, 9, 8, 1);
  C (f12, 7, 8, 1);
  C (f12, 8, 8, 1);
  C (f12, 9, 8, 0);
  C (f13, 7, 8, 1);
  C (f13, 8, 8, 1);
  C (f13, 9, 8, 0);
  C (f14, 7, 8, 0);
  C (f14, 8, 8, 0);
  C (f14, 9, 8, 1);
  D (f15, 4, 0);
  D (f15, 5, 1);
  D (f15, 6, 0);
  D (f16, 4, 1);
  D (f16, 5, 0);
  D (f16, 6, 1);
  D (f17, 4, 0);
  D (f17, 5, 0);
  D (f17, 6, 1);
  D (f18, 4, 1);
  D (f18, 5, 0);
  D (f18, 6, 0);
  D (f19, 4, 0);
  D (f19, 5, 1);
  D (f19, 6, 1);
  D (f20, 4, 1);
  D (f20, 5, 1);
  D (f20, 6, 0);
  D (f21, 4, 1);
  D (f21, 5, 0);
  D (f21, 6, 0);
  D (f22, 4, 0);
  D (f22, 5, 1);
  D (f22, 6, 1);
  D (f23, 4, 0);
  D (f23, 5, 1);
  D (f23, 6, 1);
  D (f24, 4, 1);
  D (f24, 5, 0);
  D (f24, 6, 0);
  D (f25, 4, 0);
  D (f25, 5, 0);
  D (f25, 6, 1);
  D (f26, 4, 1);
  D (f26, 5, 1);
  D (f26, 6, 0);
  D (f27, 4, 1);
  D (f27, 5, 1);
  D (f27, 6, 0);
  D (f28, 4, 0);
  D (f28, 5, 0);
  D (f28, 6, 1);
  return 0;
}
