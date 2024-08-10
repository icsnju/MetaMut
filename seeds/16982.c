// RUN: %clang_cc1 -mllvm -emptyline-comment-coverage=false -fprofile-instrument=clang -fcoverage-mapping -dump-coverage-mapping -emit-llvm-only -main-file-name include-macros.c %s | FileCheck %s

// Assorted macros to help test #include behavior across file boundaries.

#define helper1 0

void helper2(const char *, ...);

#define M1(a, ...) helper2(a, ##__VA_ARGS__);

// Note: M2 stresses vararg macro functions with macro arguments. The spelling
// locations of the args used to be set to the expansion site, leading to
// crashes (region LineEnd < LineStart). The regression test requires M2's line
// number to be greater than the line number containing the expansion.
#define M2(a, ...) M1(a, helper1, ##__VA_ARGS__);

void f1(void) {
  M2("a", "b");
}

// CHECK-LABEL: f1:
// CHECK-NEXT:   File 0, 5:15 -> 7:2 = #0
// CHECK-NEXT:   Expansion,File 0, 6:3 -> 6:5 = #0 (Expanded file = 1)
// CHECK-NEXT:   File 1, 13:20 -> 13:50 = #0
// CHECK-NEXT:   Expansion,File 1, 13:20 -> 13:22 = #0 (Expanded file = 2)
// CHECK-NEXT:   File 2, 7:20 -> 7:46 = #0
// CHECK-NEXT:   Expansion,File 2, 7:33 -> 7:44 = #0 (Expanded file = 3)
// CHECK-NEXT:   File 3, 13:26 -> 13:34 = #0
// CHECK-NEXT:   Expansion,File 3, 13:26 -> 13:33 = #0 (Expanded file = 4)
// CHECK-NEXT:   File 4, 3:17 -> 3:18 = #0
