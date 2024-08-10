/* { dg-do compile { target { aarch64*-*-* } } } */
/* { dg-additional-options "-O3 -march=armv8.2-a+dotprod" } */

#define N 64
#define TYPE signed

TYPE char X[N] __attribute__ ((__aligned__(__BIGGEST_ALIGNMENT__)));
TYPE char Y[N] __attribute__ ((__aligned__(__BIGGEST_ALIGNMENT__)));

__attribute__ ((noinline)) int
foo1(int len) {
  int i;
  TYPE int result = 0;
  TYPE short prod;

  for (i=0; i<len; i++) {
    prod = X[i] * Y[i];
    result += prod;
  }
  return result;
}
/* { dg-final { scan-assembler-times {sdot\tv[0-9]+\.4s, v[0-9]+\.16b, v[0-9]+\.16b} 4 } } */
