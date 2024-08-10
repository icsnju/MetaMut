/* { dg-do run } */
/* { dg-options "-fsanitize=undefined -fno-sanitize-recover=signed-integer-overflow -w" } */
/* { dg-shouldfail "ubsan" } */

typedef const unsigned long long int CULLI;
typedef volatile int VI;
struct s { signed long int a; };

int
main (void)
{
  volatile int shiftcount = 153;
  volatile int a = __INT_MAX__;
  volatile int b = __INT_MAX__;

  a << 152;
  b << shiftcount;
  a += 1;
  b += 2;

  return 0;
}


/* { dg-output "shift exponent 152 is too large for \[^\n\r]*-bit type 'int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*shift exponent 153 is too large for \[^\n\r]*-bit type 'int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*signed integer overflow: 2147483647 \\+ 1 cannot be represented in type 'int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*(?!.*signed integer overflow)" } */
