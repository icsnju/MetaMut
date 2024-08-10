/* { dg-do run } */
/* { dg-options "-g" } */

#if defined (__ia64__) || defined (__s390__) || defined (__s390x__)
#define NOP "nop 0"
#elif defined (__MMIX__)
#define NOP "swym 0"
#elif defined (__or1k__)
#define NOP "l.nop"
#else
#define NOP "nop"
#endif

static int x = 0;

int
main (void)
{
  asm volatile (NOP);		/* { dg-final { gdb-test . "x" "0" } } */
  x = 1;
  asm volatile (NOP);		/* { dg-final { gdb-test . "x" "1" } } */
  return 0;
}
