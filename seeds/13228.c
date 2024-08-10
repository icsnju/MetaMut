/* PR debug/69947 */
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

static const char *c = "foobar";

__attribute__((noinline, noclone)) void
foo (void)
{
  static const char a[] = "abcdefg";
  const char *b = a;		/* { dg-final { gdb-test .+1 "c\[2\]" "'o'" } } */
  asm (NOP : : : "memory");	/* { dg-final { gdb-test . "b\[4\]" "'e'" } } */
}

int
main ()
{
  foo ();
  return 0;
}
