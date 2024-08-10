/* PR debug/43329 */
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

static inline void
foo (int argx)
{
  int varx = argx;
  __asm__ volatile (NOP);			/* { dg-final { gdb-test .+1 "argx" "25" } } */
  __asm__ volatile (NOP : : "g" (varx));	/* { dg-final { gdb-test . "varx" "25" } } */
}

int i;

__attribute__((noinline))
void baz (int x)
{
  asm volatile ("" : : "r" (x) : "memory");
}

static inline void
bar (void)
{
  foo (25);
  i = i + 2;
  i = i * 2;
  i = i - 4;
  baz (i);
  i = i * 2;
  i = i >> 1;
  i = i << 6;
  baz (i);
  i = i + 2;
  i = i * 2;
  i = i - 4;
  baz (i);
  i = i * 2;
  i = i >> 6;
  i = i << 1;
  baz (i);
}

int
main (void)
{
  __asm__ volatile ("" : "=r" (i) : "0" (0));
  bar ();
  bar ();
  return i;
}
