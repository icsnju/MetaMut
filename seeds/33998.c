/* { dg-lto-do run } */

#if defined (__ia64__) || defined (__s390__) || defined (__s390x__)
#define NOP "nop 0"
#elif defined (__MMIX__)
#define NOP "swym 0"
#elif defined (__or1k__)
#define NOP "l.nop"
#else
#define NOP "nop"
#endif

asm (".globl start_\nstart_: " NOP);

int
main ()
{
  return 0;
}
