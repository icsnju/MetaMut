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

union U { int a, b; };
volatile int z;

__attribute__((noinline, noclone)) int
foo (int fd, union U x)
{
  int result = x.a != 0;
  if (fd != 0)
    result = x.a == 0;
  asm (NOP : : : "memory");	  /* { dg-final { gdb-test pr56154-1.c:17 "x.a" "4" } } */
  z = x.a;
  x.a = 6;
  asm (NOP : : : "memory");	  /* { dg-final { gdb-test pr56154-1.c:20 "x.a" "6" { xfail { aarch64*-*-* && { any-opts "-Og" } } } } } */
  return result;
}

void
test_main (void)
{
  union U u = { .a = 4 };
  foo (0, u);
}
