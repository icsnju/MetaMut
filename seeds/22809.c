/* { dg-do compile } */
/* { dg-additional-options "-O3" } */
/* { dg-require-effective-target arm_v8_2a_dotprod_neon_ok } */
/* { dg-add-options arm_v8_2a_dotprod_neon }  */

#define N 64
#define TYPE unsigned

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


/* { dg-final { scan-assembler-times {vudot\.u8\tq[0-9]+, q[0-9]+, q[0-9]+} 4 } } */

