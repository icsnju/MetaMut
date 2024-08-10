/* PR debug/69244 */
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

union U { float f; int i; };
float a, b;

__attribute__((noinline, noclone)) void
foo (void)
{
  asm volatile ("" : : "g" (&a), "g" (&b) : "memory");
}

int
main ()
{
  float e = a;
  foo ();
  float d = e;
  union U p;
  p.f = d += 2;
  int c = p.i - 4;
  asm (NOP : : : "memory");
  b = c;
  return 0;
}

/* { dg-final { gdb-test 25 "c" "p.i-4" } } */
