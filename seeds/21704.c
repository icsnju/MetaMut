/* PR lto/48437 */
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

int i __attribute__((used));
int main()
{
  volatile int i;
  for (i = 3; i < 7; ++i)
    {
      extern int i;
      asm volatile (NOP : : : "memory"); /* { dg-final { gdb-test . "i" "0" } } */
    }
  return 0;
}
