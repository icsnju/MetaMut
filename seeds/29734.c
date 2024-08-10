// RUN: %clang_analyze_cc1 -analyzer-checker=core -verify %s
// RUN: %clang_analyze_cc1 -analyzer-checker=core -analyzer-output=html-single-file -o D30406.html %s 2>&1 | FileCheck %s

// Check that single file HTML output does not process multi-file diagnostics.
// (This used to test for PR12421, before the introduction of the html-single-file format)

static void f(void) {
  int *p = 0;
  *p = 1;       // expected-warning{{Dereference of null pointer}}
}

int main(void){
  f();
  return 0;
}

// CHECK: warning: Path diagnostic report is not generated.
