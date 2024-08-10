/* { dg-do run } */
/* { dg-options "-fno-tree-scev-cprop -fno-tree-vectorize -g" } */

#if defined (__ia64__) || defined (__s390__) || defined (__s390x__)
#define NOP "nop 0"
#elif defined (__MMIX__)
#define NOP "swym 0"
#elif defined (__or1k__)
#define NOP "l.nop"
#else
#define NOP "nop"
#endif

void __attribute__((noipa,noinline))
foo (int n)
{
  if (n == 0)
    return;
  int i = 0;
  do
    {
      ++i; /* { dg-final { gdb-test . "i" "0" } } */
    }
  while (i < n);
  /* The following works only with final value replacement or with the NOP
     but not without (which means -Og).  Vectorization breaks it, so disable
     that.  At -O3 it currently fails, PR89983.  */
  __asm__ volatile (NOP : : "g" (i) : "memory"); /* { dg-final { gdb-test . "i" "1" { xfail { aarch64*-*-* && { any-opts "-ftracer" } } } } } */
}
int main() { foo(1); }
