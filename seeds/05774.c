/* PR tree-optimization/81346 */
/* { dg-do run { target int32plus } } */
/* { dg-options "-O2" } */

/* PR tree-optimization/81346 */
/* { dg-do compile { target int32 } } */
/* { dg-options "-O2 -fdump-tree-optimized" } */
/* { dg-final { scan-tree-dump-not " / " "optimized" } } */

__attribute__((noinline, noclone)) int f00 (int x) { return x / 46340 > -46341; }
__attribute__((noinline, noclone)) int f01 (int x) { int a = 46340, b = -46341; return x / a > b; }
__attribute__((noinline, noclone)) int f02 (int x) { return x / 46340 >= 46341; }
__attribute__((noinline, noclone)) int f03 (int x) { int a = 46340, b = 46341; return x / a >= b; }
__attribute__((noinline, noclone)) int f04 (int x) { return x / 46340 < 46341; }
__attribute__((noinline, noclone)) int f05 (int x) { int a = 46340, b = 46341; return x / a < b; }
__attribute__((noinline, noclone)) int f06 (int x) { return x / 46340 <= -46341; }
__attribute__((noinline, noclone)) int f07 (int x) { int a = 46340, b = -46341; return x / a <= b; }
__attribute__((noinline, noclone)) int f08 (int x) { return x / 46340 == -46341; }
__attribute__((noinline, noclone)) int f09 (int x) { int a = 46340, b = -46341; return x / a == b; }
__attribute__((noinline, noclone)) int f10 (int x) { return x / 46340 == 46341; }
__attribute__((noinline, noclone)) int f11 (int x) { int a = 46340, b = 46341; return x / a == b; }
__attribute__((noinline, noclone)) int f12 (int x) { return x / 46340 != -46341; }
__attribute__((noinline, noclone)) int f13 (int x) { int a = 46340, b = -46341; return x / a != b; }
__attribute__((noinline, noclone)) int f14 (int x) { return x / 46340 != 46341; }
__attribute__((noinline, noclone)) int f15 (int x) { int a = 46340, b = 46341; return x / a != b; }
__attribute__((noinline, noclone)) int f16 (int x) { return x / 15 > -15; }
__attribute__((noinline, noclone)) int f17 (int x) { int a = 15, b = -15; return x / a > b; }
__attribute__((noinline, noclone)) int f18 (int x) { return x / 15 > 15; }
__attribute__((noinline, noclone)) int f19 (int x) { int a = 15, b = 15; return x / a > b; }
__attribute__((noinline, noclone)) int f20 (int x) { return x / 15 >= -15; }
__attribute__((noinline, noclone)) int f21 (int x) { int a = 15, b = -15; return x / a >= b; }
__attribute__((noinline, noclone)) int f22 (int x) { return x / 15 >= 15; }
__attribute__((noinline, noclone)) int f23 (int x) { int a = 15, b = 15; return x / a >= b; }
__attribute__((noinline, noclone)) int f24 (int x) { return x / 15 < -15; }
__attribute__((noinline, noclone)) int f25 (int x) { int a = 15, b = -15; return x / a < b; }
__attribute__((noinline, noclone)) int f26 (int x) { return x / 15 < 15; }
__attribute__((noinline, noclone)) int f27 (int x) { int a = 15, b = 15; return x / a < b; }
__attribute__((noinline, noclone)) int f28 (int x) { return x / 15 <= -15; }
__attribute__((noinline, noclone)) int f29 (int x) { int a = 15, b = -15; return x / a <= b; }
__attribute__((noinline, noclone)) int f30 (int x) { return x / 15 <= 15; }
__attribute__((noinline, noclone)) int f31 (int x) { int a = 15, b = 15; return x / a <= b; }
__attribute__((noinline, noclone)) int f32 (int x) { return x / 15 == -15; }
__attribute__((noinline, noclone)) int f33 (int x) { int a = 15, b = -15; return x / a == b; }
__attribute__((noinline, noclone)) int f34 (int x) { return x / 15 == 15; }
__attribute__((noinline, noclone)) int f35 (int x) { int a = 15, b = 15; return x / a == b; }
__attribute__((noinline, noclone)) int f36 (int x) { return x / 15 != -15; }
__attribute__((noinline, noclone)) int f37 (int x) { int a = 15, b = -15; return x / a != b; }
__attribute__((noinline, noclone)) int f38 (int x) { return x / 15 != 15; }
__attribute__((noinline, noclone)) int f39 (int x) { int a = 15, b = 15; return x / a != b; }
__attribute__((noinline, noclone)) int f40 (int x) { return x / -46340 > -46341; }
__attribute__((noinline, noclone)) int f41 (int x) { int a = -46340, b = -46341; return x / a > b; }
__attribute__((noinline, noclone)) int f42 (int x) { return x / -46340 >= 46341; }
__attribute__((noinline, noclone)) int f43 (int x) { int a = -46340, b = 46341; return x / a >= b; }
__attribute__((noinline, noclone)) int f44 (int x) { return x / -46340 < 46341; }
__attribute__((noinline, noclone)) int f45 (int x) { int a = -46340, b = 46341; return x / a < b; }
__attribute__((noinline, noclone)) int f46 (int x) { return x / -46340 <= -46341; }
__attribute__((noinline, noclone)) int f47 (int x) { int a = -46340, b = -46341; return x / a <= b; }
__attribute__((noinline, noclone)) int f48 (int x) { return x / -46340 == 46341; }
__attribute__((noinline, noclone)) int f49 (int x) { int a = -46340, b = 46341; return x / a == b; }
__attribute__((noinline, noclone)) int f50 (int x) { return x / -46340 == -46341; }
__attribute__((noinline, noclone)) int f51 (int x) { int a = -46340, b = -46341; return x / a == b; }
__attribute__((noinline, noclone)) int f52 (int x) { return x / -46340 != 46341; }
__attribute__((noinline, noclone)) int f53 (int x) { int a = -46340, b = 46341; return x / a != b; }
__attribute__((noinline, noclone)) int f54 (int x) { return x / -46340 != -46341; }
__attribute__((noinline, noclone)) int f55 (int x) { int a = -46340, b = -46341; return x / a != b; }
__attribute__((noinline, noclone)) int f56 (int x) { return x / -15 > 15; }
__attribute__((noinline, noclone)) int f57 (int x) { int a = -15, b = 15; return x / a > b; }
__attribute__((noinline, noclone)) int f58 (int x) { return x / -15 > -15; }
__attribute__((noinline, noclone)) int f59 (int x) { int a = -15, b = -15; return x / a > b; }
__attribute__((noinline, noclone)) int f60 (int x) { return x / -15 >= 15; }
__attribute__((noinline, noclone)) int f61 (int x) { int a = -15, b = 15; return x / a >= b; }
__attribute__((noinline, noclone)) int f62 (int x) { return x / -15 >= -15; }
__attribute__((noinline, noclone)) int f63 (int x) { int a = -15, b = -15; return x / a >= b; }
__attribute__((noinline, noclone)) int f64 (int x) { return x / -15 < 15; }
__attribute__((noinline, noclone)) int f65 (int x) { int a = -15, b = 15; return x / a < b; }
__attribute__((noinline, noclone)) int f66 (int x) { return x / -15 < -15; }
__attribute__((noinline, noclone)) int f67 (int x) { int a = -15, b = -15; return x / a < b; }
__attribute__((noinline, noclone)) int f68 (int x) { return x / -15 <= 15; }
__attribute__((noinline, noclone)) int f69 (int x) { int a = -15, b = 15; return x / a <= b; }
__attribute__((noinline, noclone)) int f70 (int x) { return x / -15 <= -15; }
__attribute__((noinline, noclone)) int f71 (int x) { int a = -15, b = -15; return x / a <= b; }
__attribute__((noinline, noclone)) int f72 (int x) { return x / -15 == 15; }
__attribute__((noinline, noclone)) int f73 (int x) { int a = -15, b = 15; return x / a == b; }
__attribute__((noinline, noclone)) int f74 (int x) { return x / -15 == -15; }
__attribute__((noinline, noclone)) int f75 (int x) { int a = -15, b = -15; return x / a == b; }
__attribute__((noinline, noclone)) int f76 (int x) { return x / -15 != 15; }
__attribute__((noinline, noclone)) int f77 (int x) { int a = -15, b = 15; return x / a != b; }
__attribute__((noinline, noclone)) int f78 (int x) { return x / -15 != -15; }
__attribute__((noinline, noclone)) int f79 (int x) { int a = -15, b = -15; return x / a != b; }
__attribute__((noinline, noclone)) int f80 (int x) { return x / -15 > 0; }
__attribute__((noinline, noclone)) int f81 (int x) { int a = -15, b = 0; return x / a > b; }
__attribute__((noinline, noclone)) int f82 (int x) { return x / 15 > 0; }
__attribute__((noinline, noclone)) int f83 (int x) { int a = 15, b = 0; return x / a > b; }
__attribute__((noinline, noclone)) int f84 (int x) { return x / -15 >= 0; }
__attribute__((noinline, noclone)) int f85 (int x) { int a = -15, b = 0; return x / a >= b; }
__attribute__((noinline, noclone)) int f86 (int x) { return x / 15 >= 0; }
__attribute__((noinline, noclone)) int f87 (int x) { int a = 15, b = 0; return x / a >= b; }
__attribute__((noinline, noclone)) int f88 (int x) { return x / -15 < 0; }
__attribute__((noinline, noclone)) int f89 (int x) { int a = -15, b = 0; return x / a < b; }
__attribute__((noinline, noclone)) int f90 (int x) { return x / 15 < 0; }
__attribute__((noinline, noclone)) int f91 (int x) { int a = 15, b = 0; return x / a < b; }
__attribute__((noinline, noclone)) int f92 (int x) { return x / -15 <= 0; }
__attribute__((noinline, noclone)) int f93 (int x) { int a = -15, b = 0; return x / a <= b; }
__attribute__((noinline, noclone)) int f94 (int x) { return x / 15 <= 0; }
__attribute__((noinline, noclone)) int f95 (int x) { int a = 15, b = 0; return x / a <= b; }
__attribute__((noinline, noclone)) int f96 (int x) { return x / -15 == 0; }
__attribute__((noinline, noclone)) int f97 (int x) { int a = -15, b = 0; return x / a == b; }
__attribute__((noinline, noclone)) int f98 (int x) { return x / 15 == 0; }
__attribute__((noinline, noclone)) int f99 (int x) { int a = 15, b = 0; return x / a == b; }
__attribute__((noinline, noclone)) int f100 (int x) { return x / -15 != 0; }
__attribute__((noinline, noclone)) int f101 (int x) { int a = -15, b = 0; return x / a != b; }
__attribute__((noinline, noclone)) int f102 (int x) { return x / 15 != 0; }
__attribute__((noinline, noclone)) int f103 (int x) { int a = 15, b = 0; return x / a != b; }

#define INT_MAX __INT_MAX__
#define INT_MIN (-__INT_MAX__ - 1)

extern void abort (void);

int
main ()
{
  if (__CHAR_BIT__ != 8 || __SIZEOF_INT__ != 4)
    return 0;
#define TEST(fn1, fn2, v1, v2) \
  do { \
    int w1 = v1; int w2 = v2; \
    int in = 1; if (w1 > w2) { in = w1; w1 = w2; w2 = in; in = 0; } \
    if (w1 != INT_MIN) { if (fn1 (w1 - 1) != !in || fn2 (w1 - 1) != !in) abort (); } \
    if (fn1 (w1) != in || fn2 (w1) != in) abort (); \
    if (fn1 (w2) != in || fn2 (w2) != in) abort (); \
    if (w2 != INT_MAX) { if (fn1 (w2 + 1) != !in || fn2 (w2 + 1) != !in) abort (); } \
  } while (0)
TEST (f00, f01, -2147441939, INT_MAX);
TEST (f02, f03, 2147441940, INT_MAX);
TEST (f04, f05, INT_MIN, 2147441939);
TEST (f06, f07, INT_MIN, -2147441940);
TEST (f08, f09, INT_MIN, -2147441940);
TEST (f10, f11, 2147441940, INT_MAX);
TEST (f12, f13, -2147441939, INT_MAX);
TEST (f14, f15, INT_MIN, 2147441939);
TEST (f16, f17, -224, INT_MAX);
TEST (f18, f19, 240, INT_MAX);
TEST (f20, f21, -239, INT_MAX);
TEST (f22, f23, 225, INT_MAX);
TEST (f24, f25, INT_MIN, -240);
TEST (f26, f27, INT_MIN, 224);
TEST (f28, f29, INT_MIN, -225);
TEST (f30, f31, INT_MIN, 239);
TEST (f32, f33, -239, -225);
TEST (f34, f35, 225, 239);
TEST (f36, f37, -225, -239);
TEST (f38, f39, 239, 225);
TEST (f40, f41, INT_MIN, 2147441939);
TEST (f42, f43, INT_MIN, -2147441940);
TEST (f44, f45, -2147441939, INT_MAX);
TEST (f46, f47, 2147441940, INT_MAX);
TEST (f48, f49, INT_MIN, -2147441940);
TEST (f50, f51, 2147441940, INT_MAX);
TEST (f52, f53, -2147441939, INT_MAX);
TEST (f54, f55, INT_MIN, 2147441939);
TEST (f56, f57, INT_MIN, -240);
TEST (f58, f59, INT_MIN, 224);
TEST (f60, f61, INT_MIN, -225);
TEST (f62, f63, INT_MIN, 239);
TEST (f64, f65, -224, INT_MAX);
TEST (f66, f67, 240, INT_MAX);
TEST (f68, f69, -239, INT_MAX);
TEST (f70, f71, 225, INT_MAX);
TEST (f72, f73, -239, -225);
TEST (f74, f75, 225, 239);
TEST (f76, f77, -225, -239);
TEST (f78, f79, 239, 225);
TEST (f80, f81, INT_MIN, -15);
TEST (f82, f83, 15, INT_MAX);
TEST (f84, f85, INT_MIN, 14);
TEST (f86, f87, -14, INT_MAX);
TEST (f88, f89, 15, INT_MAX);
TEST (f90, f91, INT_MIN, -15);
TEST (f92, f93, -14, INT_MAX);
TEST (f94, f95, INT_MIN, 14);
TEST (f96, f97, -14, 14);
TEST (f98, f99, -14, 14);
TEST (f100, f101, 14, -14);
TEST (f102, f103, 14, -14);
  return 0;
}
