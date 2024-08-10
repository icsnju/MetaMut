/* PR debug/56154 */
/* { dg-do run } */
/* { dg-options "-g" } */
/* { dg-additional-sources "pr56154-aux.c" } */

#if defined (__ia64__) || defined (__s390__) || defined (__s390x__)
#define NOP "nop 0"
#elif defined (__MMIX__)
#define NOP "swym 0"
#elif defined (__or1k__)
#define NOP "l.nop"
#else
#define NOP "nop"
#endif

extern void abort (void);

__attribute__((noinline, noclone)) int
foo (int x)
{
  asm ("");
  x++;
  asm ("");
  x++;
  asm ("");
  x++;
  asm ("");
  x++;
  asm ("");
  x++;
  asm ("");
  x++;
  asm ("");
  x++;
  asm ("");
  x++;
  asm (NOP : : : "memory");
  asm (NOP : : : "memory");	/* { dg-final { gdb-test pr56154-2.c:30 "x" "28" } } */
  return x;
}

void
test_main (void)
{
  if (foo (20) != 28)
    abort ();
}
