// RUN: %clang_analyze_cc1 -analyzer-checker=core -analyzer-output=plist-multi-file -o %t.plist -verify %s
// RUN: tail -n +11 %t.plist | %normalize_plist | diff -ub %S/Inputs/expected-plists/plist-multi-file.c.plist -

void foo(int *ptr) {
  *ptr = 1; // expected-warning{{Dereference of null pointer (loaded from variable 'ptr')}}
}

void bar(void) {
  foo(0);
}
