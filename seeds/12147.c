/* PR debug/54519 */
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
f1 (int x, int y)
{
  asm volatile (NOP);	/* { dg-final { gdb-test .+1 "x" "2" { xfail { aarch64*-*-* && { any-opts "-Os" } } } } } */
  asm volatile (NOP);	/* { dg-final { gdb-test . "y" "0" } } */
}

static inline void
f2 (int z)
{
  f1 (z, 0);
  f1 (z, 1);
}

int
main ()
{
  f2 (2);
  f2 (3);
  return 0;
}
